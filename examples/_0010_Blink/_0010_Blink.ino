//#define ESPERT_BOARD_ESPRESSO_LITE2

#ifdef ESPERT_BOARD_ESPRESSO_LITE2
static const int PIN_LED = 2; // ESPresso Lite v2.0
#else
static const int PIN_LED = 16; // ESPresso Lite
#endif

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(PIN_LED, LOW);
  Serial.println("LED: On");
  delay(1000);

  digitalWrite(PIN_LED, HIGH);
  Serial.println("LED: Off");
  delay(1000);
}
