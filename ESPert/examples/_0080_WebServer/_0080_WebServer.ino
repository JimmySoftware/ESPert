#include <ESPert.h>

ESPert espert;

ESP8266WebServer *server = NULL;

void init_server()
{
  server = new ESP8266WebServer(80);

  server->on("/", []() {
    int t = espert.DHT.getTemperature();
    int h = espert.DHT.getHumidity();

    char text[64];
    sprintf(text, "Humidity: %i %% <br>Temperature: %i C ", h, t);
    server->send(200, "text/html", text);
  });

  server->begin();
}

void setup() {
  espert.init();
  espert.DHT.init();
  espert.OLED.init();

  espert.OLED.clear();
  espert.OLED.println( espert.getId() );
  espert.OLED.println( "" );

  int mode = espert.wifi.init();
  if( mode == ESPERT_WIFI_MODE_CONNECT ) {
    espert.OLED.println( "WiFi: connected." );
    espert.OLED.print( "IP: " );
    espert.OLED.println( espert.wifi.getLocalIP() );

    init_server();
  }
  else if( mode == ESPERT_WIFI_MODE_DISCONNECT ) {
    espert.OLED.println( "WiFi: not connected." );
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  espert.loop();

  if (server) {
    server->handleClient();
  }

  delay(100);
}
