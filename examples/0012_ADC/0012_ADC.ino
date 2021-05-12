/* 
 * EPresso 1.0 LED_BUILTIN = 16;
 * EPresso 2.0 LED_BUILTIN = 2;
 */

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  Serial.println("ADC init");
}

void loop() {
  float out = analogRead(A0);
  Serial.println("ADC: " + String(out));

  delay(1000);
}
