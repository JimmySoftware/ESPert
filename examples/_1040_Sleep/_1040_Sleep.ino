#include <ESPert.h>

// Include API-Headers
extern "C" {
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "cont.h"
}

ESPert_LED led;
ESPert espert;

const int sleepTimeS = 10;

void setup() {
  unsigned long rtc_time = system_get_rtc_time();
  unsigned long clock_time = system_get_time();
  rst_info* info = system_get_rst_info();
  //rst_info* info = ESP.getResetInfoPtr();
  int reason = info->reason;
  int cause = info->exccause;

  //system_deep_sleep_set_option(4);
  espert.init();
  espert.oled.init();
  espert.oled.clear();
  espert.oled.println("Reason: " + String(reason));
  espert.oled.println("Caurse: " + String(cause));
  espert.oled.println("RTC...: " + String(rtc_time));
  espert.oled.println("Clock.: " + String(clock_time));

  led.init(12);
}

void loop() {
  espert.println("ON");
  led.on();
  delay(1000);
  Serial.println("OFF");
  led.off();
  delay(1000);

  espert.println("ON");
  led.on();
  delay(1000);
  Serial.println("OFF");
  led.off();
  delay(1000);

  ESP.deepSleep(sleepTimeS * 1000000);
}
