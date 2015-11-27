#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>
#include <ArduinoJSON.h>

ESPert espert;

int currentSwitch = true;

// Topics
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
  ESPERT_INFO_PRINTLN( "JSON: " + String(json) );    
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
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
    espert.mqtt.connect();
    espert.print( "MQTT:" );
    espert.println( "Connected" );
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


