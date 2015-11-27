#include <ESPert.h>

ESPert espert;

bool currentSwitch = true;

void setup() {
  espert.init();
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


