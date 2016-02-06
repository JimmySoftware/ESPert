#include <ESPert.h>

String indexPage =
  "<!DOCTYPE HTML>\n"
  "<html>\n"
  "<head>\n"
  "<meta charset=\"utf-8\">\n"
  "<title>ESP-DHT By IOXhop</title>\n"
  "<script>\n"
  "function loadXMLDoc() {\n"
  "  var xmlhttp;\n"
  "  if (window.XMLHttpRequest) { \n"
  "    xmlhttp=new XMLHttpRequest();\n"
  "  } else { \n"
  "    xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");\n"
  "  }\n"
  "  \n"
  "  xmlhttp.onreadystatechange = function() {\n"
  "    if (xmlhttp.readyState==4 && xmlhttp.status==200) {\n"
  "      var d = new Date();\n"
  "      document.getElementById(\"update\").innerHTML = d.getHours() + \":\" + d.getMinutes() + \":\" + d.getSeconds();\n"
  "      document.getElementsByTagName(\"output\")[0].innerHTML=xmlhttp.responseText;\n"
  "      setTimeout(function(){ loadXMLDoc() }, 1000);\n"
  "    }\n"
  "  }\n"
  "  xmlhttp.open(\"GET\", \"./temperature\", true);\n"
  "  xmlhttp.send();\n"
  "}\n"
  "\n"
  "setTimeout(function(){ loadXMLDoc() }, 1000);\n"
  "</script>\n"
  "</head>\n"
  "\n"
  "<body style=\"font-family: Arial, Helvetica, sans-serif;\">\n"
  "<h1 style=\"text-align: center;\">ESP-DHT By IOXhop</h1>\n"
  "<h2 style=\"text-align: center;\">Temperature</h2>\n"
  "<output style=\"text-align: center; font-size: 220px; display: block;\">0</output>\n"
  "<p style=\"text-align: center;\">Update: <span id=\"update\">00:00</span></p>\n"
  "</body>\n"
  "</html>\n";

ESPert espert;
ESP8266WebServer* server = NULL;
float temperature = 0.0f;

void init_server() {
  server = new ESP8266WebServer(80);

  server->on("/temperature", []() {
    float t = espert.dht.getTemperature();

    if (!isnan(t)) {
      temperature = t;
    }

    server->send(200, "text/html", (String)((int)temperature) + "°C");
  });

  server->on("/", []() {
    server->send(200, "text/html", indexPage);
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
    espert.oled.println();
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
