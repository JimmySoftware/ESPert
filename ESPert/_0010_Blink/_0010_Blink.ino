const int PIN_LED = 16; // ESPresso Lite

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
}
