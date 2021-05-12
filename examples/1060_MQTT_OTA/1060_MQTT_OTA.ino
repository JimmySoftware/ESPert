#include <ESPert.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

ESPert espert;

const char* mqtt_server = "mqtt.espert.io";

int currentSwitch = true;
String inTopic = "ESPert/" + String(espert.info.getChipId()) + "/Command";

void callback(char* topic, byte* payload, unsigned int length) {
  String strPayload = String((char*)payload).substring(0, length);
  espert.println("Receive: " + strPayload);
  if (espert.json.init(strPayload)) {
    if (espert.json.containsKey("OTA")) {
      String value = espert.json.get("OTA");
      Serial.print("OTA: ");
      Serial.println(value);
      espert.oled.clear();
      espert.oled.println(espert.info.getId());
      espert.oled.println();
      espert.oled.printf("[%lu]\r\n\r\n", millis());
      espert.oled.println("OTA...");
      t_httpUpdate_return ret = ESPhttpUpdate.update(value.c_str());
      switch(ret) {
          case HTTP_UPDATE_FAILED:
              espert.oled.clear();
              Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
              espert.oled.println("HTTP_UPDATE_FAILD Error");
              break;

          case HTTP_UPDATE_NO_UPDATES:
              espert.oled.clear();
              Serial.println("HTTP_UPDATE_NO_UPDATES");
              espert.oled.println("HTTP_UPDATE_NO_UPDATES");
              break;

          case HTTP_UPDATE_OK:
              espert.oled.clear();
              espert.oled.println("HTTP_UPDATE_OK");
              Serial.println("HTTP_UPDATE_OK");
              break;
      }
    }
  }
  else {
    Serial.println("PARSE FAILED!");
    espert.oled.clear();
    espert.oled.println(espert.info.getId());
    espert.oled.println();
    espert.oled.printf("[%lu]\r\n\r\n", millis());
    espert.oled.println("PARSE FAILED.");
  }

}

void setup() {
  espert.init();
  espert.mqtt.init(mqtt_server, 1883, callback);

  espert.oled.init();
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
  if (espert.mqtt.connect()) {
    espert.println("MQTT: Connected");
    espert.mqtt.subscribe(inTopic);
    Serial.println("MQTT: Subscribed " + inTopic);
  }
}
