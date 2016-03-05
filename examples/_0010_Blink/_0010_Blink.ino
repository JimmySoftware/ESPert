/* 
 * EPresso 1.0 LED_BUILTIN = 16;
 * EPresso 2.0 LED_BUILTIN = 2;
 */
 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED: On");
  delay(1000);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED: Off");
  delay(1000);
}