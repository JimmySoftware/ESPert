#include <ESPert.h>

ESPert espert;
bool currentSwitch = true;

void setup() {
  espert.init();
  espert.grove.button.init(12);
  espert.grove.led.init(14);
}

void loop() {
  bool buttonPressed = espert.grove.button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.grove.led.on();
      espert.println("Grove LED: On");
    } else {
      espert.grove.led.off();
      espert.println("Grove LED: Off");
    }

    currentSwitch = buttonPressed;
  }
}
