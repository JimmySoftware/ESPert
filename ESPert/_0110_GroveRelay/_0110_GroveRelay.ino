#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>

ESPert espert;

bool currentSwitch = true;
ESPert_GroveButton groveButton;
ESPert_GroveRelay groveRelay;

void setup() {
  espert.init();
  groveButton.init(12);
  groveRelay.init(14);
}

void loop() {
  bool buttonPressed = groveButton.isOn();
  //espert.println( "Button 0: " + String(buttonPressed) );

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      groveRelay.on();
    } else {
      groveRelay.off();
    }

    currentSwitch = buttonPressed;
  }

  delay( 100 );
}
