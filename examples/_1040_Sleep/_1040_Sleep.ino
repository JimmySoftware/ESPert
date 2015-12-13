#include <ESPert.h>

ESPert_LED led;
ESPert espert;

const int sleepTimeS = 10;

void setup() {
  int status = digitalRead(16);
  espert.init();
  espert.oled.init();
  espert.oled.clear();
  espert.oled.println( "Status: "+String(status) ); 

  led.init( 12 );
}

void loop() {
  
  espert.println( "ON" );
  led.on();
  delay( 1000 );
  Serial.println( "OFF" );
  led.off();
  delay( 1000 );

  espert.println( "ON" );
  led.on();
  delay( 1000 );
  Serial.println( "OFF" );
  led.off();
  delay( 1000 );

  ESP.deepSleep(sleepTimeS * 1000000);
}
