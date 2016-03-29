#include <ESPert.h>

#ifndef ESPRESSO_LITE_VERSION
	#define ESPRESSO_LITE_VERSION 2
#endif

ESPert espert;

void setup() {
  espert.init(ESPERT_BOARD_ESPRESSO_LITE2);
}

void loop() {
  espert.led.on();
  espert.println("LED: On");
  delay(1000);

  espert.led.off();
  espert.println("LED: Off");
  delay(1000);
}
