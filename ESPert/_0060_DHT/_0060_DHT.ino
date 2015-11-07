#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>

ESPert espert;

void setup() {
  // put your setup code here, to run once:
  espert.init();
  // Use espert.DHT.init( gpio, type );
  // Type can be DHT11 or DHT22
  espert.DHT.init();
  espert.OLED.init();

  espert.OLED.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  int t = espert.DHT.getTemperature();
  int h = espert.DHT.getHumidity();

  char text[64];
  sprintf(text, "Humidity: %i %%  \nTemperature: %i C  ", h, t);
  espert.OLED.println(text);

  espert.println(text);

  delay( 5000 );
}
