#include <ESPert.h>

#ifndef ESPRESSO_LITE_VERSION
  #define ESPRESSO_LITE_VERSION 2
#endif

ESPert espert;

void setup() {
  espert.init(ESPRESSO_LITE_VERSION);

  espert.oled.init();

  // Use espert.dht.init(gpio, type);
  // Type can be DHT11 or DHT22
  // Defualt will use DHT22 on GPIO12
  espert.dht.init();

  delay(2000);
}

void loop() {
  bool isFarenheit = false;
  float t = espert.dht.getTemperature(isFarenheit);
  float h = espert.dht.getHumidity();

  if (!isnan(t) && !isnan(h)) {
    String dht = "Temperature: " + String(t) + (isFarenheit ?  " F" : " C") + "\n";
    dht += "Humidity...: " + String(h) + " %\n";

    espert.oled.clear();
    espert.oled.println(dht);
    espert.println(dht);
  }

  delay(5000);
}
