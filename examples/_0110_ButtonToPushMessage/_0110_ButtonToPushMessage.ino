#include <ESPert.h>

ESPert espert;

const char *host = "www.espert.io";
const int httpPort = 80;

String message = "Hello from ESPresso Lite!";
String smartphone_key = "0000000000000000";

bool currentSwitch = true;

void setup() {
  espert.init();

  espert.oled.init();
  delay(2000);

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
  } else if (mode == ESPERT_WIFI_MODE_SMARTCONFIG) {
    espert.println(">>> WiFi mode: smart config.");
  } else if (mode == ESPERT_WIFI_MODE_SETTINGAP) {
    espert.println(">>> WiFi mode: access point.");
  }
}

void loop() {
  espert.loop();

  bool buttonPressed = espert.button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.println("Button: On");
      espert.led.on();
      espert.println("Connecting...");
      message.replace(String(" "), String("%20"));
      String path = "/MySmartphone/send?key=" + smartphone_key + "&message=" + message;
      espert.println(">>" + espert.wifi.getHTTP(host, path.c_str()) + "<<");
    } else {
      espert.println("Button: Off");
      espert.led.off();
    }

    currentSwitch = buttonPressed;
  }

  delay(500);
}
