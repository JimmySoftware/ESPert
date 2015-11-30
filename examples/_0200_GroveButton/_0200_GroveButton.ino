#include <ESPert.h>

ESPert espert;
bool currentSwitch = true;

void setup() {
  espert.init();
  espert.grove.button.init(12);
}

void loop() {
  bool buttonPressed = espert.grove.button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.led.on();
      espert.println("Grove Button: On");
    } else {
      espert.led.off();
      espert.println("Grove Button: Off");
    }

    currentSwitch = buttonPressed;
  }
}
