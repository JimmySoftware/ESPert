#include <ESP8266WiFi.h>
const int PIN_LED = 12; // ESPresso Lite

const int sleepTimeS = 5;

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_LED, OUTPUT);  
}

void loop() {
  Serial.println( "HIGH" );
  digitalWrite( PIN_LED, HIGH );
  delay( 1000 );
  Serial.println( "LOW" );
  digitalWrite( PIN_LED, LOW );
  delay( 1000 );

  Serial.println( "HIGH" );
  digitalWrite( PIN_LED, HIGH );
  delay( 1000 );
  Serial.println( "LOW" );
  digitalWrite( PIN_LED, LOW );
  delay( 1000 );

  ESP.deepSleep(sleepTimeS * 1000000);
}
