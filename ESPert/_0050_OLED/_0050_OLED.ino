#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();
  espert.OLED.init();

  delay(2000);

  // Clear the buffer.
  espert.OLED.clear();

  /*espert.OLED.setTextSize(1);
  espert.OLED.setTextColor(espert_WHITE);
  espert.OLED.setCursor(0, 0);

  espert.OLED.println("espert ID : ");
  espert.OLED.println(espert.getId());

  espert.OLED.println("espert Firmware : ");
  espert.OLED.println(espert.getFirmwareVersion());

  espert.OLED.println("espert Library : ");
  espert.OLED.println(espert.getLibraryVersion());

  espert.OLED.println("FreeHeap : ");
  espert.OLED.println(espert.getFreeHeap());*/

  espert.OLED.println("12345678901234567890");
  espert.OLED.println("2                  2");
  espert.OLED.println("3                  3");
  espert.OLED.println("4                  4");
  espert.OLED.println("5                  5");
  espert.OLED.println("6                  6");
  espert.OLED.println("7                  7");
  espert.OLED.println("8                  8");
  espert.OLED.println("9                  9");
  espert.OLED.println("0                  0");
  espert.OLED.setCursor(6, 8);
  espert.OLED.println("234567890123456789 1");
}

void loop() {
}

