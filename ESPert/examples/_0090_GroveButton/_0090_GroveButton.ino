#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>

ESPert espert;

bool currentSwitch = true;
ESPert_GroveButton groveButton;

void setup() {
  espert.init();
  groveButton.init(12);
}

void loop() {
  bool buttonPressed = groveButton.isOn();
  //espert.println( "Button 0: " + String(buttonPressed) );

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
