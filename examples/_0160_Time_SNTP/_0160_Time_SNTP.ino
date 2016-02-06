#include <ESPert.h>
#include <time.h>

ESPert espert;

int timezone = 7;
int dst = 0;

void setup() {
  espert.init();

  espert.oled.init();
  delay(2000);

  espert.oled.clear();
  espert.oled.println(espert.info.getId());
  espert.oled.println();

  int mode = espert.wifi.init();

  if (mode == ESPERT_WIFI_MODE_CONNECT) {
    espert.println(">>> WiFi mode: connected.");
    espert.oled.clear();
    espert.oled.println(espert.info.getId());
    espert.oled.println();
    espert.oled.println("WiFi: connected.");
    espert.oled.print("IP..: ");
    espert.oled.println(espert.wifi.getLocalIP());

    configTime(timezone * 3600, dst, "pool.ntp.org", "time.nist.gov");
    Serial.print("\nWaiting for time...");

    while (!time(nullptr)) {
      Serial.print(".");
      delay(1000);
    }

    Serial.println();
  } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
    espert.println(">>> WiFi mode: disconnected.");
    espert.oled.println("WiFi: not connected.");
  }
}

void loop() {
  espert.loop();

  // The string result that is produced by ctime() contains exactly 26 characters and has the format:
  // "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n"
  // For example:
  // Mon Jul 16 02:03:55 1987\n\0

  time_t now = time(nullptr);
  String strTime = ctime(&now);
  espert.print(strTime);

  espert.oled.clear();
  espert.oled.println("Date:" + strTime.substring(0, 10) + ", " + strTime.substring(20, 24));
  espert.oled.println("Time:" + strTime.substring(11, 19));

  delay(1000);
}
