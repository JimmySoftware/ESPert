#include <ESPert.h>

ESPert espert;
bool currentSwitch = true;

void setup() {
  espert.init(ESPERT_BOARD_ESPRESSO_LITE2);
  espert.println("Press USER button to turn on LED.");
}

void loop() {
  bool buttonPressed = espert.button.isOn();

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
