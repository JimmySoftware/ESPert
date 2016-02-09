#include <ESPert.h>

ESPert espert;

const char* mqtt_server = "mqtt.espert.io";

unsigned long loopTime = 0;

int currentSwitch = true;
String outTopic = "ESPert/" + String(espert.info.getChipId()) + "/Status";
String inTopic = "ESPert/" + String(espert.info.getChipId()) + "/Command";

int currentSwitch2 = true;
ESPert_Button button2;

const char* host = "maker.ifttt.com";
const int httpPort = 80;
String ifttt_key = "";

void callback(char* topic, byte* payload, unsigned int length) {
  String strPayload = String((char*)payload).substring(0, length);
  espert.println("Receive: " + strPayload);

  if (espert.json.init(strPayload)) {
    if (espert.json.containsKey("LED")) {
      String value = espert.json.get("LED");

      if (value == "0") {
        espert.led.off();
        espert.println("LED: Off");
      } else if (value == "1") {
        espert.led.on();
        espert.println("LED: On");
      } else if (value == "2") {
        if (espert.led.isOn()) {
          espert.led.off();
          espert.println("LED(Toglle): off");
        } else {
          espert.led.on();
          espert.println("LED(Toglle): On");
        }
      }

      String outString = "{\"LED\":\"" + String(espert.led.isOn() ? 1 : 0) + "\", ";
      outString += "\"name\":\"" + String(espert.info.getId()) + "\"}";
      espert.println("Send...: " + outString);
      espert.mqtt.publish(outTopic, outString);
    }

    if (espert.json.containsKey("Buzzer")) {
      String value = espert.json.get("Buzzer");
      int l = value.toInt();
      for (int i = 0; i < l; i++) {
        delay(200);
        espert.buzzer.beep(60, 200);
      }
    }

    if (espert.json.containsKey("OLED")) {
      String value = espert.json.get("OLED");
      espert.oled.clear();
      espert.oled.println(value);
    }

    if (espert.json.containsKey("IFTTT")) {
      String value = espert.json.get("IFTTT");
      ifttt_key = value;
      espert.eeprom.write(150, ifttt_key);

      String outString = "{\"IFTTT\":\"" + ifttt_key + "\", ";
      outString += "\"name\":\"" + String(espert.info.getId()) + "\"}";
      espert.println("Send...: " + outString);
      espert.mqtt.publish(outTopic, outString);
    }
  }
}

void setup() {
  espert.init();
  delay(100);
  espert.buzzer.init(15);
  espert.dht.init();
  espert.mqtt.init(mqtt_server, 1883, callback);

  ifttt_key = espert.eeprom.read(150, 80);

  button2.init(0);

  espert.oled.init();
  delay(2000);

  espert.println("Press USER button to switch Mode");

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

  loopTime = millis();
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
    String outString = "{\"button\":\"" + String(buttonPressed ? 1 : 0) + "\", ";
    outString += "\"name\":\"" + String(espert.info.getId()) + "\"}";
    espert.println("Send...: " + outString);
    espert.mqtt.publish(outTopic, outString);
    currentSwitch = buttonPressed;
  }

  buttonPressed = button2.isOn();

  if (buttonPressed != currentSwitch2) {
    espert.println("SW");

    if (buttonPressed) {
      String path = "/trigger/button/with/key/" + ifttt_key;
      espert.println(">>" + espert.wifi.postHTTP(host, path.c_str()) + "<<");
    }

    currentSwitch2 = buttonPressed;
  }

  if (millis() - loopTime >= 5000) {
    loopTime = millis();

    float t = espert.dht.getTemperature();
    float h = espert.dht.getHumidity();

    if (!isnan(t) && !isnan(h)) {
      String outString = "{\"temperature\":\"" + String(t) + "\", ";
      outString += "\"humidity\":\"" + String(h) + "\", ";
      outString += "\"name\":\"" + String(espert.info.getId()) + "\"}";
      espert.println(outString);
      espert.mqtt.publish(outTopic, outString);
    }
  }

  delay(100);
}
