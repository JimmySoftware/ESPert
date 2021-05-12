#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();
}

void loop() {
  espert.led.on();
  espert.println("LED: On");
  delay(1000);

  espert.led.off();
  espert.println("LED: Off");
  delay(1000);
}
