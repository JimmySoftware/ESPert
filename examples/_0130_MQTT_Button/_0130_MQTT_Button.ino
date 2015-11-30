#include <ESPert.h>

ESPert espert;
int currentSwitch = true;
String outTopic = "ESPert/" + String(espert.getChipId()) + "/Button";

void setup() {
  espert.init(ESPERT_BOARD_ESPRESSO_LITE);
  espert.mqtt.init("192.168.77.1", 1883);

  espert.oled.init();
  delay(2000);

  espert.println("Press USER button to turn on LED.");

  espert.oled.clear();
  espert.oled.println(espert.getId());
  espert.oled.println();

  int mode = espert.wifi.init();

  if (mode == ESPERT_WIFI_MODE_CONNECT) {
    espert.println(">>> WiFi mode: connected.");
    espert.oled.println("WiFi: connected.");
    espert.oled.print("IP..: ");
    espert.oled.println(espert.wifi.getLocalIP());
    espert.button.disableLongPress();
  } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
    espert.println(">>> WiFi mode: disconnected.");
    espert.oled.println("WiFi: not connected.");
  }
}

void loop() {
  espert.loop();

  if (espert.mqtt.connect()) {
    espert.println("MQTT: Connected");
    espert.println("MQTT: Out Topic " + outTopic);
  }

  bool buttonPressed = espert.button.isOn();

  if (buttonPressed != currentSwitch) {
    if (buttonPressed) {
      espert.led.on();
    } else {
      espert.led.off();
    }

    String outString  = "{\"button\":\"" + String(buttonPressed ? 1 : 0) + "\", ";
    outString += "\"name\":\"" + String(espert.getId()) + "\"}";
    espert.println(outString);
    espert.mqtt.publish(outTopic, outString);
    currentSwitch = buttonPressed;
  }
}
