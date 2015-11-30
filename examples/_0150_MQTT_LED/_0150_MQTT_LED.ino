#include <ESPert.h>

ESPert espert;
int currentSwitch = true;
String outTopic = "ESPert/" + String(espert.getChipId()) + "/Button";
String inTopic = outTopic;

void mqttCallback(const MQTT::Publish& pub) {
  espert.println("Receive: " + pub.payload_string());
  String key = "button";

  if (espert.json.init(pub.payload_string()) && espert.json.containsKey(key)) {
    String value = espert.json.get(key);

    if (value == "0") {
      espert.led.off();
      espert.println("LED: Off");
    } else if (value == "1") {
      espert.led.on();
      espert.println("LED: On");
    }

    espert.println();
  }
}

void setup() {
  espert.init();
  espert.mqtt.init("192.168.77.1", 1883, mqttCallback);

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
    espert.mqtt.subscribe(inTopic);
    Serial.println("MQTT: Subscribed " + inTopic);
  }

  bool buttonPressed = espert.button.isOn();

  if (buttonPressed != currentSwitch) {
    String outString  = "{\"button\":\"" + String(buttonPressed ? 1 : 0) + "\", ";
    outString += "\"name\":\"" + String(espert.getId()) + "\"}";
    espert.println("Send...: " + outString);
    espert.mqtt.publish(outTopic, outString);
    currentSwitch = buttonPressed;
  }
}
