#include <ESPert.h>

#ifndef ESPRESSO_LITE_VERSION
  #define ESPRESSO_LITE_VERSION 2
#endif

ESPert espert;
bool currentSwitch = true;

void setup() {
  espert.init(ESPRESSO_LITE_VERSION);
  espert.println("Press USER button to turn on LED and buzzer.");
  espert.buzzer.init(15);
  delay(200);
  espert.buzzer.beep(60, 200);
  delay(200);
  espert.buzzer.beep(60, 200);
  delay(200);
  espert.buzzer.beep(60, 200);
  delay(200);
}

void loop() {
  bool buttonPressed = espert.button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.println("Button: On");
      espert.led.on();
      espert.buzzer.on(60);
    } else {
      espert.println("Button: Off");
      espert.led.off();
      espert.buzzer.off();
    }

    currentSwitch = buttonPressed;
  }
}
