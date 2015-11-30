#include <ESPert.h>

ESPert espert;
ESP8266WebServer *server = NULL;
String uuid = "";
int major = 0;
int minor = 0;
int txpower = 0;

void init_server() {
  server = new ESP8266WebServer(80);

  server->on("/", []() {
    String str = "";
    str += "UUID: " + uuid + "<br>";
    str += "Major: " + String(major) + "<br>";
    str += "Minor: " + String(minor) + "<br>";
    str += "TX Power: " + String(txpower) + "<br>";

    server->send(200, "text/html", str);
  });

  server->begin();
}

void setup() {
  espert.init();

  espert.oled.init();
  delay(2000);

  espert.oled.clear();
  espert.oled.println(espert.getId());
  espert.oled.println();

  int mode = espert.wifi.init();

  if (mode == ESPERT_WIFI_MODE_CONNECT) {
    espert.oled.print("IP: ");
    espert.oled.println(espert.wifi.getLocalIP());

    init_server();
  } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
    espert.oled.println("WiFi: not connected.");
  }

  espert.swSerial.init(12, 14, 128);
  espert.swSerial.begin(9600);
  espert.println("Software Serial started.");

  if (espert.ble.init(&espert.swSerial)) {
    String v = espert.ble.getFirmwareVersion();
    Serial.println(v);
    espert.oled.println("FW: " + v);
  } else {
    espert.oled.println("BLE: Not ready.");
    espert.ble.on();
  }

  uuid = espert.ble.getUUID();
  espert.print("UUID: ");
  espert.println(uuid);
  espert.oled.print("UUID: ");
  espert.oled.println(uuid);

  major = espert.ble.getMajor();
  espert.print("Major: ");
  espert.print(major);
  espert.oled.print("Major: ");
  espert.oled.println(major );

  minor = espert.ble.getMinor();
  espert.print(" Minor: ");
  espert.print((int)minor);
  espert.oled.print("Minor: ");
  espert.oled.println(minor);

  txpower = espert.ble.getTXPower();
  espert.print(" TX Power: ");
  espert.println((int)txpower);
  espert.oled.print("TX Power: ");
  espert.oled.println(txpower);
}

void loop() {
  espert.loop();

  if (server) {
    server->handleClient();
  }

  String readString = espert.swSerial.readString();

  if (readString.length() > 0) {
    Serial.println(readString);
    readString = "";
  }

  if (Serial.available()) {
    espert.swSerial.write(Serial.read());
  }
}
