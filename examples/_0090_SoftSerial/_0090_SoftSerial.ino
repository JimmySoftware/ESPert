#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();

  espert.swSerial.init(12, 14, 128);
  espert.swSerial.begin(9600);
  espert.println("Software Serial started.");
}

void loop() {
  espert.loop();

  String readString = espert.swSerial.readString();

  if (readString.length() > 0) {
    Serial.println(readString);
    readString = "";
  }

  if (Serial.available()) {
    espert.swSerial.write(Serial.read());
  }
}
