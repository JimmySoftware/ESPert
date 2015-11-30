#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();
  espert.OLED.init();

  espert.OLED.clear();
  espert.OLED.println( espert.getId() );
  espert.OLED.println( "" );

  int mode = espert.wifi.init();
  if( mode == ESPERT_WIFI_MODE_CONNECT ) {
    espert.println( ">>> WiFi mode: connected." );
    espert.OLED.println( "WiFi: connected." );
    espert.OLED.print( "IP: " );
    espert.OLED.println( espert.wifi.getLocalIP() );
  }
  else if( mode == ESPERT_WIFI_MODE_DISCONNECT ) {
    espert.println( ">>> WiFi mode: disconnected." );
    espert.OLED.println( "WiFi: not connected." );
  }
  else if( mode == ESPERT_WIFI_MODE_SMARTCONFIG ) {
    espert.println( ">>> WiFi mode: smart config." );
  }
  else if( mode == ESPERT_WIFI_MODE_SETTINGAP ) {
    espert.println( ">>> WiFi mode: access point." );
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  espert.loop();
}
