#include <ESPert.h>

ESPert espert;
bool currentSwitch = true;

void setup() {
  espert.init();
  espert.grove.button.init(12);
  espert.grove.relay.init(14);
}

void loop() {
  bool buttonPressed = espert.grove.button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.grove.relay.on();
      espert.println("Grove Relay: On");
    } else {
      espert.grove.relay.off();
      espert.println("Grove Relay: Off");
    }

    currentSwitch = buttonPressed;
  }
}
