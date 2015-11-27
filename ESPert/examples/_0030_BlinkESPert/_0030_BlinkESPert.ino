
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();

  // In case using other GPIO than 16, un-comment following line
  // espert.LED.init( 12 );
}

void loop() {
  // put your main code here, to run repeatedly:
  espert.LED.on();
  espert.println( "LED on" );
  delay( 1000 );

  espert.LED.off();
  espert.println( "LED off" );
  delay( 1000 );
}


