#include <ESPert.h>

ESPert espert;
ESP8266WebServer *server = NULL;

void init_server() {
  server = new ESP8266WebServer(80);

  server->on("/", []() {
    bool isFarenheit = false;
    float t = espert.dht.getTemperature(isFarenheit);
    float h = espert.dht.getHumidity();
    String dht = "";

    if (!isnan(t) && !isnan(h)) {
      dht += "Temperature: " + String(t) + (isFarenheit ?  " F" : " C") + "<br>";
      dht += "Humidity: " + String(h) + " %";
    } else {
      dht = "Failed to read temperature and humidity from DHT sensor!";
    }

    server->send(200, "text/html", dht);
  });

  server->begin();
}

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
    espert.oled.print("IP..: ");
    espert.oled.println(espert.wifi.getLocalIP());
    espert.oled.println("");
    init_server();
  } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
    espert.oled.println("WiFi: not connected.");
  }
}

void loop() {
  espert.loop();

  if (server) {
    server->handleClient();
  }
}
