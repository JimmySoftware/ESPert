#include <ESPert.h>

const char* mqtt_server = "mqtt.espert.io";

ESPert espert;
String outTopic = "ESPert/" + String(espert.info.getChipId()) + "/DHT";

void setup() {
  espert.init();
  espert.dht.init();

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
    espert.mqtt.init(mqtt_server, 1883);
  } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
    espert.println(">>> WiFi mode: disconnected.");
    espert.oled.println("WiFi: not connected.");
  }
}

void loop() {
  if (espert.mqtt.connect()) {
    espert.println("MQTT: Connected");
    espert.println("MQTT: Out Topic " + outTopic);
  }

  float t = espert.dht.getTemperature();
  float h = espert.dht.getHumidity();

  if (!isnan(t) && !isnan(h)) {
    String outString = "{\"temperature\":\"" + String(t) + "\", ";
    outString += "\"humidity\":\"" + String(h) + "\", ";
    outString += "\"name\":\"" + String(espert.info.getId()) + "\"}";
    espert.println(outString);
    espert.mqtt.publish(outTopic, outString);
  }

  delay(5000);
}
