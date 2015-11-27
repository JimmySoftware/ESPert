#include <ESPert.h>
#include <ArduinoJson.h>

ESPert espert;

int currentSwitch = true;

// Topics
String inTopic     = "ESPert/" + String(espert.getChipId()) + "/LED"; 
String outTopic     = "ESPert/" + String(espert.getChipId()) + "/Button"; 

// MQTT server address
IPAddress mqtt_server(192,168,77,1);
//char* mqtt_server = "mqtt.tespa.io";

bool bWebOK = false;

void MQTT_callback(const MQTT::Publish& pub) {
  espert.println( "Callback" );
  // Copy the payload content into a char*
  char* json;
  json = (char*) malloc(pub.payload_len() + 1);
  memcpy(json, pub.payload(), pub.payload_len());
  json[pub.payload_len()] = '\0';
  espert.println( "JSON: " + String(json) );    
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
  }


  if( root.containsKey("cmd") ) {
    const char* led = root["cmd"];
    espert.println( "LED: " + String(led) );

    int v = 0;
    if( atoi(led) == 0 ) {
      espert.LED.off();
      espert.println( "LED: OFF" );
    }
    else if( atoi(led) == 1 ) {
      espert.LED.on();
      espert.println( "LED: ON" );
      v = 1;
    }
    else if( atoi(led) == 2 ) {
      espert.println( "LED: TOGGLE" );
      if( espert.LED.get() != 0 ) {
        espert.LED.off();
        espert.println( "LED: OFF" );
      }
      else {
        espert.LED.on();
        espert.println( "LED: ON" );
        v = 1;
      }
    }
    // Messages for MQTT
        String outString  = "{\"status\": \""+String(v)+"\",";
        outString = outString + "\"name\": \""+String(espert.getId())+"\"  }";

    // Publish temperature
    espert.mqtt.publish(inTopic, outString );     
  }
  free(json);
}
 
void setup() {
  espert.init();
  espert.println( "Press USER button to turn on LED." );
  espert.mqtt.init( mqtt_server, 1883 );
  
  espert.OLED.init();

  espert.OLED.clear();
  espert.OLED.println( espert.getId() );
  espert.OLED.println( "" );

  int mode = espert.wifi.init();
  if( mode == ESPERT_WIFI_MODE_CONNECT ) {
    espert.println( ">>> WiFi mode: connected." );
    espert.OLED.println( "WiFi: connected." );
    espert.OLED.print( "IP: " );
    espert.OLED.println( espert.wifi.getLocalIP() );
    espert.mqtt.setCallback(MQTT_callback);
    espert.mqtt.connect();
    espert.print( "MQTT:" );
    espert.println( "Connected" );
    espert.mqtt.subscribe(inTopic ); // topic subscription
    espert.println("Subscribed");
    bWebOK = true;
  }
  else if( mode == ESPERT_WIFI_MODE_DISCONNECT ) {
    espert.println( ">>> WiFi mode: disconnected." );
    espert.OLED.println( "WiFi: not connected." );
  }
}

void loop() {
  espert.loop();

  bool buttonPressed = espert.button.isOn();
  if( buttonPressed != currentSwitch ) {
    if( buttonPressed ) {
      espert.LED.on();
      
      if( bWebOK )
      {
        String outString  = "{\"button\": \"1\",";
        outString = outString + "\"name\": \""+String(espert.getId())+"\"  }";
    
        // Publish temperature
        espert.mqtt.publish(outTopic, outString);
      }
    }
    else {
      espert.LED.off();
      if( bWebOK )
      {
        String outString  = "{\"button\": \"0\",";
        outString = outString + "\"name\": \""+String(espert.getId())+"\"  }";
    
        // Publish temperature
        espert.mqtt.publish(outTopic, outString);
      }
    }
    currentSwitch = buttonPressed;
  }
  

  delay(200);
}


