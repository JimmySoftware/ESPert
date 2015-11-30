#include <ESPert.h>

ESPert espert;

// Topics
String outTopic     = "ESPert/" + String(espert.getChipId()) + "/DHT"; 

// MQTT server address
IPAddress mqtt_server(192,168,77,1);
//char* mqtt_server = "mqtt.espert.io";

bool bWebOK = false;
 
void setup() {
  espert.init();
  espert.println( "Press USER button to turn on LED." );
  espert.mqtt.init( mqtt_server, 1883 );

  espert.DHT.init(12,DHT22);
  
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

  // put your main code here, to run repeatedly:
  int t = espert.DHT.getTemperature();
  int h = espert.DHT.getHumidity();

  String outString  = "{\"temperature\": \""+String(t)+"\",";
  outString  = outString + "\"humidity\": \""+String(h)+"\",";
  outString  = outString + "\"lattitude\": \"1.2786564\",";
  outString  = outString + "\"longitude\": \"103.8428243\",";

  outString = outString + "\"name\": \""+String(espert.getId())+"\"  }";

  espert.println( outString );
  // Publish temperature
  espert.mqtt.publish(outTopic, outString);


  delay(5000);
}


