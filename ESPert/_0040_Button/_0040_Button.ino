#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>

ESPert espert;

bool currentSwitch = true;

void setup() {
  espert.init();

  // In case using other GPIO than 16, un-comment following line
  espert.LED.init( 12 );
}

void loop() {
  bool buttonPressed = espert.button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.LED.on();
    } else {
      espert.LED.off();
    }

    currentSwitch = buttonPressed;
  }

  delay( 100 );
}
