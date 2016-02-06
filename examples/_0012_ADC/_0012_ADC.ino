//#define ESPERT_BOARD_ESPRESSO_LITE2

#ifdef ESPERT_BOARD_ESPRESSO_LITE2
static const int PIN_LED = 2; // ESPresso Lite v2.0
#else
static const int PIN_LED = 16; // ESPresso Lite
#endif

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(A0, INPUT);
  Serial.println("ADC init");
}

void loop() {
  float out = analogRead(A0);
  Serial.println("ADC: " + String(out));

  delay(1000);
}
