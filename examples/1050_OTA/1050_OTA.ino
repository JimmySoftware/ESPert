#include <ESPert.h>

ESPert espert;


int currentSwitch = true;

void setup() {
  espert.init();

  espert.oled.init();
  espert.ota.init();

  // config callbacks
  espert.ota.on_progress([](unsigned int progress, unsigned int total){
    Serial.printf("_CALLBACK_ Progress: %u/%u\r\n", progress,  total);
  });
  delay(2000);

  espert.println("Press USER button to turn on LED.");

  espert.oled.clear();
  espert.oled.println(espert.info.getId());
  espert.oled.println();

  int mode = espert.wifi.init();

  if (mode == ESPERT_WIFI_MODE_CONNECT) {
    espert.println(">>> WiFi mode: connected.");
    espert.oled.println("WiFi: connected.");
    espert.oled.print("IP..: ");
    espert.oled.println(espert.wifi.getLocalIP());
  } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
    espert.println(">>> WiFi mode: disconnected.");
    espert.oled.println("WiFi: not connected.");
  }
}

void loop() {
  espert.loop();
}
