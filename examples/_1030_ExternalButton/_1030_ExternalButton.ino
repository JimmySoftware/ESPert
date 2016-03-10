#include <ESPert.h>

#ifndef ESPRESSO_LITE_VERSION
  #define ESPRESSO_LITE_VERSION 2
#endif

ESPert espert;
bool currentSwitch = true;

ESPert_Button button;

void setup() {
  espert.init(ESPRESSO_LITE_VERSION);
  button.init(14);
  espert.println("Press the external button to turn on LED.");
}

void loop() {
  bool buttonPressed = button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.println("Button: On");
      espert.led.on();
    } else {
      espert.println("Button: Off");
      espert.led.off();
    }

    currentSwitch = buttonPressed;
  }
}
