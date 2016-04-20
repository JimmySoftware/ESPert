// Game & Watch Octopus Simulator for ESPresso Lite
//
// Instructions
//
// ESPresso Lite V2.0
// Button 1 (GPIO0) : Game A, Move Left
// Button 2 (GPIO13): Game B, Move Right
//
// ESPresso Lite V1.0
// Button 1 (FLASH): Game A, Move Left
// Button 2 (USER) : Game B, Move Right
//
// Game A
// Divers try to recover treasure from sunken ship raise it to their boat.
// Giant octopus tries to prevent them.
// 1. Press button 1. Highest previous score will be displayed. Game A begins when key is released.
// 2. Press operation button 2, and one diver goes into the water.
// 3. By pressing operation button 1 and 2, diver moves left and right to evade moving tentacles.
//    When diver reaches treasure, press operation button 2 to place gems into bag.
//   *Press button once, diver bags one gem, press button twice, two gems, ...
// 4. One gem equals one point. Reach the boat, and earn 3 points.
//   *If diver fails to pick up treasure, he can not return to boat.
// 5. If octopus tentacle touches diver, score one miss! Diver vanishes from screen.
//    When all three divers are gone, game ends.
// 6. When player reaches a bonus score (200 or 500 points), any lost divers at the time are replaced in the boat.
//    Tentacles move at increasing speed, then return to original speed every 100 points.
//
// Game B
// Tentacles move faster than in Game A. In other respects, this game is same as Game A.

#include <ESPert.h>
ESPert espert;

// for reading current time from internet
static const char* wifiSSID = ""; // your WiFI network name
static const char* wifiPassword = ""; // your WiFi password
float timeZone = +7.00f; // your time zone (+7.00 is UTC+07:00 Bangkok)
bool isTimeVisibled = true;
int hours = 0;
int minutes = 0;
int seconds = 0;

// button pin
#ifdef ARDUINO_ESP8266_ESPRESSO_LITE_V1
byte buttonPin[2] = {0, 2};
#else
byte buttonPin[2] = {0, 13};
#endif
byte gamepadPin[2] = {14, 1};
bool isGamepadEnabled = false;

// sound (buzzer)
static const byte buzzerPin = 12;
float buzzerDuration = 0.0f;
bool isSoundEnabled = true;

// OLED bitmaps - Image to Adafruit OLED Bitmap Converter http://www.majer.ch/lcd/adf_bitmap.php
const uint8_t backgroundBitmap[] PROGMEM = { // background.png
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xC0, 0xC0, 0xC0, 0xC0, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x30, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0x70, 0xB0, 0xB0, 0xB0, 0xB0,
  0x70, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0x70, 0x30, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0x60, 0x60, 0x60, 0x60, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x20,
  0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x30, 0x30, 0x18, 0x98, 0x98, 0x98, 0x90, 0x10, 0x00, 0x08, 0x08, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x10, 0x90, 0x80, 0x90, 0xD0, 0xD8, 0xD8, 0xCC, 0xCC,
  0xCC, 0x8C, 0x88, 0x88, 0x10, 0x40, 0x60, 0x20, 0x20, 0x30, 0xB0, 0x98, 0x98, 0xD8, 0x98, 0x90, 0x10, 0x20, 0x20, 0x60, 0x40, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x30, 0x30, 0x60, 0x60, 0x60,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x1C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
  0x03, 0x03, 0x03, 0x00, 0x04, 0x04, 0x06, 0x06, 0x03, 0x01, 0x01, 0x03, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x60, 0x70, 0xF0, 0xF0, 0xB0, 0xB8, 0xF8, 0x7C, 0x5E, 0x7F, 0x3F, 0xBF,
  0xBF, 0xDF, 0xDF, 0xDF, 0xFF, 0xFF, 0xBE, 0xBC, 0xBF, 0x75, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xE0, 0xE0, 0xF8, 0x5C, 0x6E, 0xEE, 0xFE, 0xFF, 0xDF, 0x9F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x78, 0x00, 0x00, 0x00, 0x60, 0x63, 0xE6, 0xFC, 0xE7, 0x61, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x02, 0x02, 0x03, 0x03, 0x03,
  0x07, 0x07, 0x07, 0x87, 0xC7, 0xE7, 0xFF, 0xFF, 0xEF, 0xF7, 0xF7, 0xFF, 0xFF, 0xFF, 0xF8, 0xEF, 0xEE, 0xF3, 0xFF, 0xFF, 0xEF, 0xEF, 0xCE, 0xBC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0x7F,
  0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xDD, 0xFE, 0xBE, 0xBE, 0xBE, 0xBE, 0xFE, 0xFF, 0xFF, 0xFF, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x38, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x1F, 0x33, 0x3F, 0x3F, 0x3F, 0x3F, 0x03, 0x03, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F,
  0x3E, 0x3D, 0x3D, 0x7D, 0x5D, 0x7E, 0x76, 0x6E, 0x6E, 0x66, 0x66, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0x7C, 0x78, 0x78, 0xF8, 0xF8, 0xD8, 0xD8, 0xD8, 0xD8, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x46, 0x86, 0x9E, 0xE6, 0x83, 0xE0, 0xE0, 0x80, 0xC0, 0x80, 0x00, 0x60, 0x18, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xC0, 0xC0, 0xC0, 0x20, 0xF0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x06, 0x0C,
  0x1C, 0x38, 0x78, 0xF8, 0xF8, 0x98, 0x3B, 0xB3, 0xB3, 0xB7, 0xB7, 0xA7, 0xE7, 0x7F, 0x7F, 0xFF, 0xC7, 0x8F, 0x8F, 0x9F, 0x1E, 0x1F, 0x0D, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xF0, 0xF0, 0xF1, 0xF1, 0xF2, 0xF2, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00,
  0x00, 0x4C, 0xD9, 0xFB, 0xEF, 0x4C, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xE0, 0xD0, 0x48, 0xDF, 0xBF, 0xF4, 0xE0, 0xE6, 0x96, 0x37, 0xB7, 0xB5, 0xBD, 0xAD, 0xEF, 0x6B, 0xDF, 0xD7, 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFE, 0xFE, 0xFC, 0xFF, 0xF0, 0xE0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0xFE, 0x23, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xD0, 0xB8, 0xDE,
  0xC9, 0xDE, 0xF3, 0xF6, 0x9C, 0xFF, 0xE7, 0xAB, 0xBF, 0xFF, 0xE0, 0xE5, 0xFD, 0xF9, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t fpsBitmap[] PROGMEM { // fps.png
  0xE0, 0xFA, 0xFE, 0xFF, 0xE0, 0xFA, 0xF8, 0xFF, 0xE8, 0xEA, 0xE2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t gameTypeBitmap[2][32] PROGMEM {
  {0xFF, 0xC3, 0xBD, 0xBD, 0xAD, 0xCB, 0xFF, 0xDF, 0xAB, 0xAB, 0x87, 0xFF, 0x87, 0xFB, 0x87, 0xFB, 0x87, 0xFF, 0xC7, 0xAB, 0xAB, 0xA7, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xEB, 0xED, 0xEB, 0x87, 0xFF}, // gameA.png
  {0xFF, 0xC3, 0xBD, 0xBD, 0xAD, 0xCB, 0xFF, 0xDF, 0xAB, 0xAB, 0x87, 0xFF, 0x87, 0xFB, 0x87, 0xFB, 0x87, 0xFF, 0xC7, 0xAB, 0xAB, 0xA7, 0xFF, 0xFF, 0xFF, 0xFF, 0xBD, 0x81, 0xB5, 0xB5, 0xCB, 0xFF}  // gameB.png
};

const uint8_t onTheBoatBitmap[2][32] PROGMEM {
  {0xFF, 0xFF, 0x63, 0x41, 0x14, 0x00, 0x94, 0xC1, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFF, 0xFC, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // diverOnTheBoat0.png
  {0xFF, 0xFF, 0x63, 0x41, 0x14, 0x00, 0x94, 0xC1, 0xA3, 0x9F, 0xCF, 0x23, 0x13, 0x0F, 0x3F, 0xFF, 0xFC, 0xFC, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFE, 0xFF}  // diverOnTheBoat1.png
};

const uint8_t remainderBitmap[] PROGMEM { // diverOnTheBoat2.png
  0xE3, 0xC1, 0x94, 0x00, 0x94, 0xC1, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFE, 0xFC, 0xFC, 0xFC, 0xFE, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t amBitmap[] PROGMEM { // am.png
  0xF1, 0xFA, 0xF1, 0xFF, 0xF1, 0xFE, 0xF1, 0xFE, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t pmBitmap[] PROGMEM { // pm.png
  0xF0, 0xFA, 0xF8, 0xFF, 0xF1, 0xFE, 0xF1, 0xFE, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t colonBitmap[] PROGMEM { // colon.png
  0x99, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t numberBitmap[10][8] PROGMEM {
  {0x89, 0x7E, 0x7E, 0x7E, 0x89, 0xFF, 0xFF, 0xFF}, // number0.png
  {0xFF, 0xFF, 0xFF, 0xFF, 0x89, 0xFF, 0xFF, 0xFF}, // number1.png
  {0x8F, 0x76, 0x76, 0x76, 0xF9, 0xFF, 0xFF, 0xFF}, // number2.png
  {0xFF, 0x76, 0x76, 0x76, 0x89, 0xFF, 0xFF, 0xFF}, // number3.png
  {0xF9, 0xF7, 0xF7, 0xF7, 0x89, 0xFF, 0xFF, 0xFF}, // number4.png
  {0xF9, 0x76, 0x76, 0x76, 0x8F, 0xFF, 0xFF, 0xFF}, // number5.png
  {0x89, 0x76, 0x76, 0x76, 0x8F, 0xFF, 0xFF, 0xFF}, // number6.png
  {0xF9, 0xFE, 0xFE, 0xFE, 0x89, 0xFF, 0xFF, 0xFF}, // number7.png
  {0x89, 0x76, 0x76, 0x76, 0x89, 0xFF, 0xFF, 0xFF}, // number8.png
  {0xF9, 0x76, 0x76, 0x76, 0x89, 0xFF, 0xFF, 0xFF}  // number9.png
};

const uint8_t missDiverBitmap[] PROGMEM { // missDiver.png
  0x1F, 0x0F, 0xA7, 0x07, 0xA4, 0x08, 0x17, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t missArmBitmap[2][8] PROGMEM {
  {0xFF, 0xFF, 0xE7, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF}, // missArm0.png
  {0xF3, 0xF1, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF}  // missArm1.png
};

const uint8_t missLegsBitmap[2][32] PROGMEM {
  {0xC7, 0x97, 0x37, 0x27, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // missLegs0.png
  {0x9B, 0x3B, 0xF9, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // missLegs1.png
};

const uint8_t tentacleBitmap[5][5][8] PROGMEM {
  {
    {0xF3, 0xF1, 0xF0, 0xF2, 0xF2, 0xF0, 0xF9, 0xF9}, // tentacle00.png
    {0xF3, 0xF1, 0xF1, 0xF0, 0xF0, 0xF8, 0xFC, 0xFE}, // tentacle01.png
    {0xFE, 0xFC, 0xF9, 0xF1, 0xE1, 0xE3, 0xF3, 0xFB}, // tentacle02.png
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  },
  {
    {0xF3, 0xF1, 0xF0, 0xF2, 0xF2, 0xF0, 0xF9, 0xF9}, // tentacle10.png
    {0xF7, 0xF3, 0xF1, 0xF0, 0xF4, 0xF2, 0xF8, 0xFC}, // tentacle11.png
    {0xF3, 0xF1, 0xF0, 0xF0, 0xFA, 0xFC, 0xFE, 0xFF}, // tentacle12.png
    {0xDF, 0xE7, 0xE0, 0xF0, 0xF8, 0xFC, 0xFF, 0xFF}, // tentacle13.png
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  },
  {
    {0xF3, 0xF1, 0xF0, 0xF0, 0xF4, 0xF2, 0xF8, 0xFC}, // tentacle20.png
    {0xFD, 0xF8, 0xF0, 0xC4, 0xE2, 0xF8, 0xFC, 0xFF}, // tentacle21.png
    {0xE0, 0xC1, 0x83, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF}, // tentacle22.png
    {0xE0, 0xC1, 0x83, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF}, // tentacle23.png
    {0xDF, 0xE7, 0xE0, 0xF1, 0xFB, 0xFF, 0xFF, 0xFF}  // tentacle24.png
  },
  {
    {0xF1, 0xF0, 0xF2, 0xF8, 0xF8, 0xFC, 0xFC, 0xFE}, // tentacle30.png
    {0xF3, 0xE3, 0xE3, 0xF3, 0xF1, 0xF8, 0xFF, 0xFF}, // tentacle31.png
    {0xC7, 0xC3, 0xE1, 0xF1, 0xF0, 0xFF, 0xFF, 0xFF}, // tentacle32.png
    {0xDF, 0xC3, 0xE1, 0xF0, 0xF8, 0xFF, 0xFF, 0xFF}, // tentacle33.png
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  },
  {
    {0xFE, 0xFC, 0xF8, 0xFA, 0xF8, 0xF9, 0xFB, 0xFF}, // tentacle40.png
    {0xFE, 0xFC, 0xF8, 0xFA, 0xF8, 0xF9, 0xFB, 0xFF}, // tentacle41.png
    {0xFC, 0xC0, 0xE0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF}, // tentacle42.png
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  }
};

const uint8_t diverBitmap[17][32] PROGMEM {
  {0x9F, 0x9F, 0xA3, 0x41, 0x14, 0x00, 0x14, 0x41, 0x63, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xF9, 0xFB, 0xFB, 0xFC, 0xFC, 0xF8, 0xE3, 0xE7, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // diver0.png
  {0x9F, 0x9F, 0xA3, 0x41, 0x14, 0x00, 0x14, 0x41, 0x63, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xF9, 0xFB, 0xFB, 0xFC, 0xFC, 0xF8, 0xE3, 0xE7, 0xE7, 0xF0, 0xE0, 0xE1, 0xE3, 0xFF, 0xFF}, // diver1.png
  {0x4F, 0x4F, 0xDF, 0xA3, 0x01, 0x00, 0x0A, 0x80, 0x45, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xCD, 0xCD, 0xC4, 0xF8, 0xFC, 0xFF, 0xFF, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // diver2.png
  {0x4F, 0x4F, 0x9F, 0xA3, 0x01, 0x00, 0x0A, 0x80, 0x45, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xCD, 0xCD, 0xC4, 0xF8, 0xFC, 0xC7, 0x83, 0x80, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // diver3.png
  {0x7F, 0xCF, 0xCF, 0xDF, 0xA3, 0x09, 0x02, 0x10, 0x04, 0x41, 0x63, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFD, 0xFD, 0xFC, 0xC0, 0xCC, 0xDE, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF}, // diver4.png
  {0x7F, 0xCF, 0xCF, 0xDF, 0xA3, 0x09, 0x02, 0x10, 0x04, 0x41, 0x63, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFD, 0xFD, 0xFC, 0xC0, 0xCC, 0xDE, 0xFF, 0xFF, 0xFE, 0xE0, 0xC1, 0xC3, 0xC7, 0xFF}, // diver5.png
  {0xFF, 0xFF, 0x63, 0x41, 0x04, 0x10, 0x02, 0x49, 0x63, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x9C, 0x9E, 0x87, 0xE0, 0xF0, 0xF3, 0xF3, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // diver6.png
  {0xFF, 0xFF, 0x63, 0x41, 0x04, 0x10, 0x02, 0x49, 0x63, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x9C, 0x9E, 0x87, 0xE0, 0xF0, 0xF3, 0xF3, 0xF1, 0xFF, 0xE0, 0xC0, 0xC1, 0xC7, 0xFF, 0xFF}, // diver7.png
  {0xFF, 0xFF, 0xFF, 0xCF, 0xCF, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0xA0, 0xC9, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xF3, 0xF9, 0xF0, 0x80, 0x9E, 0x9C, 0xF3, 0xF3, 0xFF, 0xFF, 0xFF}, // diver8.png
  {0x7F, 0x3F, 0x1F, 0x0F, 0xCF, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0xA0, 0xC9, 0xE3, 0xFF, 0xFF, 0xFF, 0xF8, 0xF8, 0xE4, 0xE7, 0xE7, 0xF3, 0xF9, 0xF0, 0x80, 0x9E, 0x9C, 0xF3, 0xF3, 0xFF, 0xFF, 0xFF}, // diver9.png
  {0xFF, 0xFF, 0xFF, 0xCF, 0xCF, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0x20, 0x49, 0x63, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xF3, 0xF9, 0xF0, 0x80, 0x9E, 0x9F, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF}, // diver10.png
  {0x7F, 0x3F, 0x1F, 0x0F, 0xCF, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0x20, 0x49, 0x63, 0x7F, 0x7F, 0xFF, 0xF8, 0xF8, 0xE4, 0xE7, 0xE7, 0xF3, 0xF9, 0xF0, 0x80, 0x9E, 0x9F, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF}, // diver11.png
  {0xFF, 0xFF, 0xFF, 0xCF, 0xCF, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0xA0, 0xC9, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xF3, 0xF9, 0xF0, 0x80, 0x9E, 0x9C, 0xF3, 0xF3, 0xFF, 0xFF, 0xFF}, // diver12.png
  {0x7F, 0x3F, 0x1F, 0x0F, 0xCF, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0xA0, 0xC9, 0xE3, 0xFF, 0xFF, 0xFF, 0xF8, 0xF8, 0xE4, 0xE7, 0xE7, 0xF3, 0xF9, 0xF0, 0x80, 0x9E, 0x9C, 0xF3, 0xF3, 0xFF, 0xFF, 0xFF}, // diver13.png
  {0x7F, 0x3F, 0x1F, 0x0F, 0x4F, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0xA0, 0xC9, 0xE3, 0xFF, 0xFF, 0xFF, 0xF8, 0xF8, 0xE4, 0xE7, 0xE6, 0xF2, 0xF8, 0xF0, 0x80, 0x9E, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // diver14.png
  {0x7F, 0x3F, 0x1F, 0x0F, 0x4F, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0xA0, 0xC9, 0xE3, 0xFF, 0xFF, 0xFF, 0xF8, 0xF8, 0xE4, 0xE7, 0xE6, 0xF2, 0xF8, 0xF0, 0x80, 0x9E, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // diver15.png
  {0x7F, 0x3F, 0x1F, 0x0F, 0x4F, 0x9F, 0xA3, 0x01, 0x10, 0x04, 0xA0, 0xC9, 0xE3, 0xFF, 0xFF, 0xFF, 0xF8, 0xF8, 0xE4, 0xE7, 0xE6, 0xF2, 0xF8, 0xF0, 0x80, 0x9E, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // diver16.png
};

struct Point {
  byte x;
  byte y;
};

Point tentaclePosition[5][5] = {
  {{45, 23}, {37, 23}, {29, 21}, { 0,  0}, { 0,  0}},
  {{45, 23}, {38, 28}, {34, 34}, {31, 39}, { 0,  0}},
  {{55, 27}, {52, 32}, {51, 35}, {51, 41}, {49, 48}},
  {{67, 34}, {68, 37}, {69, 41}, {69, 46}, { 0,  0}},
  {{84, 39}, {87, 43}, {91, 47}, { 0,  0}, { 0,  0}}
};

const uint8_t* diverStepImage[6] = {NULL};
bool isDiverStepVisibled[6] = {false};
Point diverStepPosition[6] = {{16, 1}, {19, 18}, {21, 38}, {38, 50}, {59, 49}, {78, 50}};

bool isRemainderVisibled[2] = {true};
Point remainderPosition[2] = {{40, 1}, {31, 1}};

const uint8_t* hourDigitImage[2] = {NULL};
bool isHourDigitVisibled[2] = {false};
Point hourDigitPosition[2] = {{60, 1}, {67, 1}};

const uint8_t* minuteDigitImage[2] = {NULL};
bool isMinuteDigitVisibled[2] = {false};
Point minuteDigitPosition[2] = {{77, 1}, {84, 1}};

const uint8_t* fpsDigitImage[2] = {NULL};
bool isFPSDigitVisibled[2] = {false};
Point fpsDigitPosition[2] = {{115, 1}, {122, 1}};

bool isMissDiverVisibled = false;
Point missDiverPosition = {59, 30};

const uint8_t* missArmImage = NULL;
bool isMissArmVisibled = false;
Point missArmPosition = {63, 39};

const uint8_t* missLegsImage = NULL;
bool isMissLegsVisibled = false;
Point missLegsPosition = {75, 33};

const uint8_t* gameTypeImage = NULL;
bool isGameTypeVisibled = false;

unsigned long lastFrameTime = 0l;
float elapsedTime = 0.0f;
unsigned long frameCount = 0l;
unsigned long frameRate = 0l;
unsigned long fpsLastFrameTime = 0l;
unsigned long timeSyncLastFrameTime;
bool isFPSVisibled = false;

String eepromKey = "Octopus";
short eepromAddress = 512 - eepromKey.length() - 6;
short gameAHighScore = 0;
short gameBHighScore = 0;
static const int gameASpeed = 220;
static const int gameBSpeed = 170;
static const int diverSpeed = 300;
int gameSpeed = 50;
bool isSecondChanged = false;
unsigned long gameLoopTime = 0l;
byte gameType = 0;
bool isBagVisibled = false;
short score = 0;
byte miss = 0;
bool isPlaying = false;
byte add3Score = 0;
short secondCount = 0;
bool isScoreVisibled = false;
byte onTheBoatCount = 10;
byte caught = 0;
byte clearMissCount = 0;
byte diverPosition = 0;
bool isJustStarted = true;
bool isShowResetScreen = true;
byte showHighScoreTime = 0;
bool isColonVisibled = false;
bool isAMVisibled = false;
bool isPMVisibled = false;

bool isRandomTentacle = false;
byte currentTentacle = 0;
bool isEndOfTentacle[5] = {false};
byte tentacleDirection[5] = {0};
byte tentacleCount[5] = {0};
byte maxTentacleCount[5] = {3, 4, 5, 4, 3};
bool isTentacleVisibled[5][5] = {false};
byte tentacle = 0;
byte diverPositionIndex = 0;
bool isNextTentacle = false;

ESPert_Button button[2];
static const byte maxButtonDelay = 50; // milliseconds
float buttonDelay = maxButtonDelay;
bool isButtonPressed[2] = {false};
byte pressedButton = 0;

// function prototypes
void update();
void render();
void initGame();
void resetGame();
void startGame();
void tryAgain();
void demoScreen();
void autoMove();
void showGameType(byte gameTypeValue);
void showScore(short value);
void showHighScore(byte gameTypeValue);
void showFPS(short value);
void showTime();
void showResetScreen(byte gameTypeValue = 0);
void checkSpeed();
void checkDiver();
void checkMiss();
void checkHighScore();
bool checkShowHighScoreTime();
void checkTentacle();
void checkClearMiss();
void checkCaught();
void checkButtons();
void gotCaught();
void check3Score();
void addScore();
void moveLeft();
void moveRight();
void readHighScores();
void writeHighScores();
String intToString(int value, int length, String prefixChar = "0");
void playSound(int index);

void setup() {
  espert.init();
  espert.oled.init();
  espert.buzzer.init(buzzerPin);
  randomSeed(analogRead(0));

  for (int i = 0; i < 2; i++) {
    if (isGamepadEnabled) {
      buttonPin[i] = gamepadPin[i];
    }

    button[i].init(buttonPin[i]);
  }

  initGame();
}

void loop() {
  update();
  render();
}

void update() {
  // game time
  unsigned long frameTime = millis();
  elapsedTime = frameTime - lastFrameTime;
  lastFrameTime = frameTime;

  // frame rate
  frameCount++;
  if (frameTime - fpsLastFrameTime >= 1000l) {
    frameRate = frameCount;
    frameCount = 0l;
    fpsLastFrameTime = frameTime;
  }

  // time sync
  unsigned long t = frameTime - timeSyncLastFrameTime;
  if (t >= 1000l) {
    timeSyncLastFrameTime = frameTime - (t - 1000l);
    isSecondChanged = true;

    if (isTimeVisibled) {
      if (++seconds > 59) {
        seconds = 0;

        if (++minutes > 59) {
          minutes = 0;

          if (++hours > 23) {
            hours = 0;
          }
        }
      }
    }

    if (isFPSVisibled) {
      showFPS(frameRate);
    }
  }

  // button
  buttonDelay += elapsedTime;

  if (buttonDelay >= maxButtonDelay) {
    buttonDelay = 0.0f;
    checkButtons();
  }

  // sound
  if (buzzerDuration > 0.0f) {
    buzzerDuration -= elapsedTime;

    if (buzzerDuration <= 0.0f) {
      buzzerDuration = 0.0f;
      espert.buzzer.off();
    }
  }

  // game loop
  if (!isShowResetScreen) {
    gameLoopTime += elapsedTime;

    if (gameLoopTime >= gameSpeed) {
      gameLoopTime -= gameSpeed;

      if (!isPlaying) {
        demoScreen();
      } else {
        if (isSecondChanged) {
          isSecondChanged = false;

          if (miss == 4 && ++secondCount > 3) {
            showHighScoreTime = 0;
            resetGame();
            tryAgain();
          }

          if (caught == 0 && clearMissCount == 0 && onTheBoatCount > 0) {
            secondCount++;

            if (--onTheBoatCount == 0) {
              moveRight();
              secondCount = 0;
            }
          }
        }

        if (caught == 0) {
          check3Score();

          if (clearMissCount == 0) {
            checkTentacle();

            if (diverPosition > 5) {
              moveRight();
            }
          } else {
            checkClearMiss();
          }
        } else if (caught > 0) {
          checkCaught();
        }
      }
    }
  }
}

void render() {
  espert.oled.clear(false);
  espert.oled.setColor(ESPERT_WHITE);
  espert.oled.drawBitmap(0, 0, 128, 64, backgroundBitmap, false);

  if (isGameTypeVisibled) {
    espert.oled.drawBitmap(0, 56, 32, 8, gameTypeImage, false);
  }

  espert.oled.setColor(ESPERT_BLACK);

  for (int i = 0; i < 6; i++) {
    if (isDiverStepVisibled[i]) {
      espert.oled.drawBitmap(diverStepPosition[i].x, diverStepPosition[i].y, 16, 16, diverStepImage[i], false);
    }

    for (int j = 0; j < 5; j++) {
      if (i < maxTentacleCount[j] && isTentacleVisibled[j][i]) {
        espert.oled.drawBitmap(tentaclePosition[j][i].x, tentaclePosition[j][i].y, 8, 8, tentacleBitmap[j][i], false);
      }
    }

    if (i < 2) {
      if (isRemainderVisibled[i]) {
        espert.oled.drawBitmap(remainderPosition[i].x, remainderPosition[i].y, 16, 16, remainderBitmap, false);
      }

      if (isHourDigitVisibled[i]) {
        espert.oled.drawBitmap(hourDigitPosition[i].x, hourDigitPosition[i].y, 8, 8, hourDigitImage[i], false);
      }

      if (isMinuteDigitVisibled[i]) {
        espert.oled.drawBitmap(minuteDigitPosition[i].x, minuteDigitPosition[i].y, 8, 8, minuteDigitImage[i], false);
      }

      if (isFPSVisibled) {
        if (i == 0) {
          espert.oled.drawBitmap(101, 4, 16, 8, fpsBitmap, false);
        }

        if (isFPSDigitVisibled[i]) {
          espert.oled.drawBitmap(fpsDigitPosition[i].x, fpsDigitPosition[i].y, 8, 8, fpsDigitImage[i], false);
        }
      }

      if (i == 0) {
        if (isColonVisibled) {
          espert.oled.drawBitmap(74, 1, 8, 8, colonBitmap, false);
        }

        if (isAMVisibled) {
          espert.oled.drawBitmap(52, 1, 16, 8, amBitmap, false);
        }

        if (isPMVisibled) {
          espert.oled.drawBitmap(52, 5, 16, 8, pmBitmap, false);
        }

        if (isMissDiverVisibled) {
          espert.oled.drawBitmap(missDiverPosition.x, missDiverPosition.y, 16, 16, missDiverBitmap, false);
        }

        if (isMissArmVisibled) {
          espert.oled.drawBitmap(missArmPosition.x, missArmPosition.y, 8, 8, missArmImage, false);
        }

        if (isMissLegsVisibled) {
          espert.oled.drawBitmap(missLegsPosition.x, missLegsPosition.y, 16, 16, missLegsImage, false);
        }

        if (isShowResetScreen) {
          espert.oled.drawBitmap(diverStepPosition[0].x, diverStepPosition[0].y, 16, 16, onTheBoatBitmap[0], false);
          espert.oled.drawBitmap(diverStepPosition[5].x, diverStepPosition[5].y, 16, 16, diverBitmap[11], false);
          espert.oled.drawBitmap(diverStepPosition[5].x, diverStepPosition[5].y, 16, 16, diverBitmap[15], false);
          espert.oled.drawBitmap(missArmPosition.x, missArmPosition.y, 8, 8, missArmBitmap[0], false);
          espert.oled.drawBitmap(missLegsPosition.x, missLegsPosition.y, 16, 16, missLegsBitmap[0], false);
        }
      }
    }
  }

  espert.oled.update();
}

bool checkShowHighScoreTime() {
  bool finished = true;

  if (showHighScoreTime > 0 && pressedButton != 0) {
    if (++showHighScoreTime > 3) {
      showHighScoreTime = 0;
      isGameTypeVisibled = false;
    } else {
      showHighScore(pressedButton);
      finished = false;
    }
  }

  return finished;
}

void showResetScreen(byte gameTypeValue) {
  isShowResetScreen = true;

  if (gameTypeValue == 1 || gameTypeValue == 2) {
    isGameTypeVisibled = true;
    gameTypeImage = gameTypeBitmap[gameTypeValue - 1];
    showScore((gameTypeValue == 1) ? gameAHighScore : gameBHighScore);
  } else {
    isGameTypeVisibled = false;
    isAMVisibled = true;
    isPMVisibled = false;
    isColonVisibled = true;
    memset(&isHourDigitVisibled, true, sizeof(isHourDigitVisibled));
    memset(&isMinuteDigitVisibled, true, sizeof(isMinuteDigitVisibled));

    for (int i = 0; i < 2; i++) {
      hourDigitImage[i] = numberBitmap[i + 1];
      minuteDigitImage[i] = numberBitmap[0];
    }

    if (isFPSVisibled) {
      showFPS(0);
    }
  }

  memset(&isDiverStepVisibled, true, sizeof(isDiverStepVisibled));
  memset(&isRemainderVisibled, true, sizeof(isRemainderVisibled));
  memset(&isTentacleVisibled, true, sizeof(isTentacleVisibled));
  isMissDiverVisibled = true;
  isMissArmVisibled = true;
  isMissLegsVisibled = true;
  missArmImage = missArmBitmap[1];
  missLegsImage = missLegsBitmap[1];

  for (int i = 0; i < 6; i++) {
    diverStepImage[i] = (i == 0) ? onTheBoatBitmap[1] : diverBitmap[((i - 1) * 2) + 1];
  }
}

void startGame() {
  if (!isPlaying) {
    gameLoopTime = 0l;
    showGameType(gameType);
    isMissDiverVisibled = false;
    isMissArmVisibled = false;
    isMissLegsVisibled = false;
    isPlaying = true;
    isBagVisibled = false;
    clearMissCount = 0;
    isRandomTentacle = false;
    memset(&isEndOfTentacle, false, sizeof(isEndOfTentacle));
    memset(&tentacleDirection, 0, sizeof(tentacleDirection));

    for (int i = 0; i < 5; i++) {
      isDiverStepVisibled[i + 1] = false;

      if (tentacleCount[i] > 0) {
        for (int j = tentacleCount[i] - 1; j >= 0; j--) {
          isTentacleVisibled[i][j] = false;
        }
      }

      tentacleCount[i] = 0;
    }

    currentTentacle = 0;
    add3Score = 0;
    diverPosition = 0;
    onTheBoatCount = 10;
    secondCount = 0;
    score = 0;
    caught = 0;
    showScore(score);
    checkSpeed();
    miss = 0;
    checkDiver();
    checkMiss();
  }
}

void showGameType(byte gameTypeValue) {
  isGameTypeVisibled = false;

  if (gameTypeValue == 1 || gameTypeValue == 2) {
    isGameTypeVisibled = true;
    gameTypeImage = gameTypeBitmap[gameTypeValue - 1];
  }
}

void showScore(short value) {
  String scoreString = intToString(value, 3, "0");
  isScoreVisibled = true;
  isAMVisibled = false;
  isPMVisibled = false;
  isColonVisibled = false;
  isHourDigitVisibled[0] = false;

  if (scoreString.charAt(0) != '0') {
    hourDigitImage[1] = numberBitmap[scoreString.charAt(0) - '0'];
    isHourDigitVisibled[1] = true;
    minuteDigitImage[0] = numberBitmap[scoreString.charAt(1) - '0'];
    isMinuteDigitVisibled[0] = true;
  } else {
    isHourDigitVisibled[1] = false;

    if (scoreString.charAt(1) != '0') {
      minuteDigitImage[0] = numberBitmap[scoreString.charAt(1) - '0'];
      isMinuteDigitVisibled[0] = true;
    } else {
      isMinuteDigitVisibled[0] = false;
    }
  }

  minuteDigitImage[1] = numberBitmap[scoreString.charAt(2) - '0'];
  isMinuteDigitVisibled[1] = true;
}

void showHighScore(byte gameTypeValue) {
  isGameTypeVisibled = true;
  gameTypeImage = gameTypeBitmap[gameTypeValue - 1];
  showScore((gameTypeValue == 1) ? gameAHighScore : gameBHighScore);
}

void showFPS(short value) {
  value = constrain(value, 0, 99);
  String fpsString = intToString(value, 2, "0");
  memset(&isFPSDigitVisibled, isFPSVisibled, sizeof(isFPSDigitVisibled));

  for (int i = 0; i < 2; i++) {
    fpsDigitImage[i] = numberBitmap[fpsString.charAt(i) - '0'];
  }

  if (value > 0 && value < 10) {
    isFPSDigitVisibled[0] = false;
  }
}

void showTime() {
  if (checkShowHighScoreTime()) {
    showHighScoreTime = 0;

    if (isTimeVisibled) {
      isColonVisibled = (seconds % 2 == 0) ? true : false;
      isAMVisibled = (hours < 12) ? true : false;
      isPMVisibled = !isAMVisibled;
      int h = hours % 12;
      String hh = intToString((h == 0) ? 12 : h, 2, "0");
      String mm = intToString(minutes, 2, "0");
      memset(&isHourDigitVisibled, true, sizeof(isHourDigitVisibled));
      memset(&isMinuteDigitVisibled, true, sizeof(isMinuteDigitVisibled));

      for (int i = 0; i < 2; i++) {
        hourDigitImage[i] = numberBitmap[hh.charAt(i) - '0'];
        minuteDigitImage[i] = numberBitmap[mm.charAt(i) - '0'];
      }

      if (hh.charAt(0) == '0') {
        isHourDigitVisibled[0] = false;
      }
    }
  }
}

String intToString(int value, int length, String prefixChar) {
  String stringValue = String(value);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += prefixChar;
  }

  return prefix + stringValue;
}

void checkSpeed() {
  if (caught > 0 || add3Score > 0 || clearMissCount > 0) {
    gameSpeed = diverSpeed;
  } else {
    gameSpeed = 50;

    switch (gameType) {
      case 1: // game A
        gameSpeed = gameASpeed;

        if (gameSpeed > 30 && ((score >= 70 && score < 100) || (score >= 160 && score < 190) || (score >= 250 && score < 280) || (score >= 330 && score < 360) || (score >= 430 && score < 460) || (score >= 520 && score < 560) || (score >= 610 && score < 650) || (score >= 710 && score < 740) || (score >= 810 && score < 830) || (score >= 930 && score < 970))) {
          gameSpeed -= 30;
        } else if (gameSpeed > 60 && ((score >= 190 && score < 200) || (score >= 280 && score < 300) || (score >= 560 && score < 600) || (score >= 650 && score < 690) || (score >= 740 && score < 780))) {
          gameSpeed -= 60;
        } else if (gameSpeed > 50 && ((score >= 360 && score < 390) || (score >= 460 && score < 490) || (score >= 830 && score < 870))) {
          gameSpeed -= 50;
        } else if (gameSpeed > 80 && ((score >= 390 && score < 400) || (score >= 490 && score < 500) || (score >= 690 && score < 700) || (score >= 780 && score < 800) || (score >= 870 && score < 900) || (score >= 970 && score <= 999))) {
          gameSpeed -= 80;
        }
        break;

      case 2: // game B
        gameSpeed = gameBSpeed;

        if (gameSpeed > 30 && ((score >= 30 && score < 70) || (score >= 130 && score < 160) || (score >= 220 && score < 250) || (score >= 330 && score < 360) || (score >= 430 && score < 460) || (score >= 520 && score < 560) || (score >= 610 && score < 650) || (score >= 710 && score < 740) || (score >= 830 && score < 870) || (score >= 930 && score < 970))) {
          gameSpeed -= 30;
        } else if (gameSpeed > 60 && ((score >= 70 && score < 100) || (score >= 160 && score < 190) || (score >= 250 && score < 280) || (score >= 360 && score < 390) || (score >= 460 && score < 490) || (score >= 560 && score < 600) || (score >= 650 && score < 690) || (score >= 740 && score < 780) || (score >= 870 && score < 900))) {
          gameSpeed -= 60;
        } else if (gameSpeed > 80 && ((score >= 190 && score < 200) || (score >= 280 && score < 300) || (score >= 390 && score < 400) || (score >= 490 && score < 500) || (score >= 690 && score < 700) || (score >= 780 && score < 800) || (score >= 970 && score < 999))) {
          gameSpeed -= 80;
        }
        break;
    }

    if (gameSpeed < 0) {
      gameSpeed = 0;
    }
  }
}

void checkMiss() {
  if (miss <= 2) {
    diverStepImage[0] = onTheBoatBitmap[0];
    isDiverStepVisibled[0] = true;
  }

  switch (miss) {
    case 0:
      isRemainderVisibled[0] = true;
      isRemainderVisibled[1] = true;
      break;

    case 1:
      isRemainderVisibled[0] = false;
      isRemainderVisibled[1] = true;
      tryAgain();
      break;

    case 2:
      isRemainderVisibled[0] = false;
      isRemainderVisibled[1] = false;
      tryAgain();
      break;

    default:
      checkHighScore();

      if (isPlaying) {
        caught = 7;
        miss = 4;
        secondCount = 0;
      } else {
        if (caught == 0) {
          isMissDiverVisibled = false;
          isMissArmVisibled = false;
          isMissLegsVisibled = false;
          isBagVisibled = false;
          onTheBoatCount = 10;
          secondCount = 0;
          isRemainderVisibled[0] = true;
          isRemainderVisibled[1] = true;
          diverStepImage[0] = onTheBoatBitmap[0];
          isDiverStepVisibled[0] = true;
        }

        add3Score = 0;
        miss = 0;
        tryAgain();
      }
      break;
  }
}

void checkDiver() {
  if (miss == 1 || miss == 2) {
    isRemainderVisibled[1] = false;
    diverStepImage[0] = onTheBoatBitmap[0];
    isDiverStepVisibled[0] = true;
  }
}

void tryAgain() {
  for (int i = 1; i < 6; i++) {
    isDiverStepVisibled[i] = false;
  }

  diverPosition = 0;
}

void checkHighScore() {
  if (gameType == 1 && score > gameAHighScore) {
    gameAHighScore = score;
    writeHighScores();
  } else if (gameType == 2 && score > gameBHighScore) {
    gameBHighScore = score;
    writeHighScores();
  }
}

void check3Score() {
  if (add3Score > 0) {
    onTheBoatCount = 10;

    if (--add3Score > 0) {
      addScore();

      if (clearMissCount == 0) {
        playSound(1);
      }
    } else {
      isBagVisibled = false;
      checkSpeed();
    }

    if (add3Score == 2) {
      diverStepImage[diverPosition] = onTheBoatBitmap[1];
    } else if (add3Score == 3 || add3Score <= 1) {
      diverStepImage[diverPosition] = onTheBoatBitmap[0];
    }
  }
}

void checkTentacle() {
  bool isPlaySound = false;
  tentacle = 0;
  diverPositionIndex = 0;
  isNextTentacle = false;

  if ((currentTentacle > 1 && ((tentacleCount[currentTentacle] == 0 && random(5) <= 2) || tentacleCount[currentTentacle] > 0)) || (currentTentacle <= 1 && ((tentacleCount[0] == 0 && tentacleCount[1] == 0 && random(5) <= 2) || (tentacleCount[currentTentacle] > 0)))) {
    isNextTentacle = false;

    if (currentTentacle == 0 || currentTentacle == 1) {
      if (tentacleCount[0] == 0 && tentacleCount[1] == 0 && isRandomTentacle) {
        currentTentacle = random(2);
        isRandomTentacle = false;;
      } else if (tentacleCount[0] == 0 && tentacleCount[1] == 0 && !isRandomTentacle) {
        isNextTentacle = true;
        isRandomTentacle = true;
      }
    }

    tentacle = currentTentacle;

    if (isNextTentacle) {
      isPlaySound = (add3Score == 0) ? true : false;
    } else if (!isNextTentacle && tentacleDirection[tentacle] == 0) {
      isTentacleVisibled[tentacle][tentacleCount[tentacle]] = true;
      isPlaySound = (add3Score == 0) ? true : false;

      if (++tentacleCount[tentacle] == maxTentacleCount[tentacle]) {
        isEndOfTentacle[tentacle] = true;
        diverPositionIndex = (diverPosition > 5) ? 5 : diverPosition;

        if (diverPositionIndex == tentacle + 1) {
          diverStepImage[diverPositionIndex] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];
          isPlaySound = false;
          gotCaught();
        }

        tentacleDirection[tentacle] = 1;
      } else {
        isEndOfTentacle[tentacle] = false;
      }
    } else if (!isNextTentacle) {
      if (gameType == 2 && tentacleDirection[tentacle] == 1 && tentacleCount[tentacle] == 1 && random(10) == 0) {
        tentacleDirection[tentacle] = 0;
        isPlaySound = (add3Score == 0) ? true : false;
      } else {
        tentacleCount[tentacle]--;
        isTentacleVisibled[tentacle][tentacleCount[tentacle]] = false;
        isEndOfTentacle[tentacle] = false;
        isPlaySound = (add3Score == 0) ? true : false;

        if (tentacle < 2) {
          isRandomTentacle = false;
        }

        if (tentacleCount[tentacle] == 0) {
          tentacleDirection[tentacle] = 0;
        }
      }
    }
  } else {
    tentacle = currentTentacle;
    isPlaySound = (add3Score == 0) ? true : false;
  }

  if (isPlaySound) {
    playSound(0);
  }

  currentTentacle = tentacle + 1;

  if (currentTentacle > 4) {
    currentTentacle = 0;
  }
}

void moveLeft() {
  if (diverPosition >= 1) {
    if (diverPosition > 5) {
      diverPosition = 5;
    }

    if (diverPosition > 1) {
      isDiverStepVisibled[diverPosition] = false;
      diverPosition--;
      diverStepImage[diverPosition] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];
      isDiverStepVisibled[diverPosition] = true;

      if (isEndOfTentacle[diverPosition - 1]) {
        gotCaught();
      }
    } else if (isBagVisibled && clearMissCount == 0) {
      isDiverStepVisibled[diverPosition] = false;
      diverPosition--;
      add3Score = 4;
      diverStepImage[diverPosition] = onTheBoatBitmap[1];
      isDiverStepVisibled[diverPosition] = true;
    }
  }
}

void moveRight() {
  onTheBoatCount = 0;

  if (diverPosition < 5) {
    isDiverStepVisibled[diverPosition] = false;
    diverPosition++;
    diverStepImage[diverPosition] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];
    isDiverStepVisibled[diverPosition] = true;

    if (isEndOfTentacle[diverPosition - 1]) {
      gotCaught();
    }
  } else if (diverPosition < 10 && clearMissCount == 0) {
    if (++diverPosition == 9) {
      diverPosition = 5;
    } else if (diverPosition == 8) {
      isBagVisibled = true;

      if (isPlaying && caught == 0) {
        playSound(1);
        addScore();
      }
    }

    diverStepImage[5] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];

    if (isEndOfTentacle[5 - 1]) {
      gotCaught();
    }
  }
}

void addScore() {
  if (isPlaying && caught == 0) {
    if (score == 999) {
      checkHighScore();
      score = 1;
    } else {
      score++;
    }

    showScore(score);

    if (score == 200 || score == 500) {
      if (miss == 1) {
        isRemainderVisibled[0] = true;
      } else if (miss == 2) {
        isRemainderVisibled[0] = true;
        isRemainderVisibled[1] = true;
      }

      playSound(3);
      clearMissCount = 16;
    }

    checkSpeed();
  }
}

void checkCaught() {
  if (caught > 0) {
    if (--caught == 10) {
      playSound(2);
      checkSpeed();
    } else if (caught == 8) {
      miss++;
      checkDiver();
      isDiverStepVisibled[(diverPosition < 5) ? diverPosition : 5] = false;
      isMissDiverVisibled = true;
      missArmImage = missArmBitmap[0];
      missLegsImage = missLegsBitmap[0];
      isMissArmVisibled = true;
      isMissLegsVisibled = true;
      isTentacleVisibled[2][0] = true;
      isTentacleVisibled[2][1] = true;
      isTentacleVisibled[2][2] = false;
      isTentacleVisibled[2][3] = false;
      isTentacleVisibled[2][4] = false;
      tentacleCount[2] = 2;
      isEndOfTentacle[2] = false;

      if (tentacleCount[3] < 2) {
        isTentacleVisibled[3][0] = true;
        isTentacleVisibled[3][1] = true;
        tentacleCount[3] = 2;
        isEndOfTentacle[3] = false;
      }
    } else if (caught > 0 && caught < 8 && (caught % 2) == 0) {
      missArmImage = missArmBitmap[0];
      missLegsImage = missLegsBitmap[0];

      if (miss <= 3) {
        playSound(1);
      }

      if (caught == 4 && miss == 1) {
        isRemainderVisibled[0] = false;
        isRemainderVisibled[1] = true;
      }
    } else if (caught > 0 && caught < 8 && (caught % 2) == 1) {
      missArmImage = missArmBitmap[1];
      missLegsImage = missLegsBitmap[1];

      if (miss == 4) {
        caught = 7;
      } else {
        playSound(1);
      }
    } else if (caught == 0) {
      if (miss < 3) {
        isMissDiverVisibled = false;
        isMissArmVisibled = false;
        isMissLegsVisibled = false;
        isBagVisibled = false;
        onTheBoatCount = 10;
        secondCount = 0;
      }

      checkMiss();
      checkSpeed();
    }
  }
}

void checkClearMiss() {
  if (clearMissCount > 0) {
    clearMissCount--;

    if (miss == 1) {
      isRemainderVisibled[0] = !isRemainderVisibled[0];
    } else if (miss == 2) {
      isRemainderVisibled[0] = !isRemainderVisibled[0];
      isRemainderVisibled[1] = !isRemainderVisibled[1];
    }

    if (isRemainderVisibled[0]) {
      playSound(3);
    }

    if (clearMissCount == 0) {
      miss = 0;
    }
  }
}

void demoScreen() {
  if (isSecondChanged) {
    isSecondChanged = false;
    showTime();

    if (caught > 0) {
      checkCaught();
    } else {
      autoMove();
    }
  }
}

void autoMove() {
  if (!isBagVisibled || (isBagVisibled && diverPosition == 8) || (isBagVisibled && diverPosition == 0 && add3Score == 1)) {
    if (isBagVisibled && diverPosition == 0 && add3Score == 1) {
      add3Score = 0;
      isBagVisibled = false;
    }

    moveRight();
  } else {
    if (add3Score > 1) {
      check3Score();
    } else {
      moveLeft();
    }
  }

  for (int i = 0; i < 4; i++) {
    checkTentacle();
  }
}

void checkButtons() {
  for (int i = 0; i < 2; i++) {
    bool isPressed = (digitalRead(buttonPin[i]) == LOW) ? true : false;

    if (isPressed != isButtonPressed[i]) {
      isButtonPressed[i] = isPressed;

      if (isPressed) {
        if (pressedButton == 0 && (add3Score == 0 || !isPlaying)) {
          pressedButton = i + 1;

          if (!isPlaying || (isPlaying && (miss == 4 || (caught == 0 && (i == 0 || (i == 1 && ((clearMissCount > 0 && diverPosition > 0 && diverPosition <= 5) || (clearMissCount == 0 && diverPosition <= 5)))))))) {
            if (!isPlaying || miss == 4) {
              showHighScoreTime = 1;

              if (!isPlaying || (isPlaying && miss == 4)) {
                showHighScore(pressedButton);
              }
            } else if (i == 0) {
              moveLeft();
            } else if (i == 1) {
              moveRight();
            }
          }
        }
      } else {
        if (showHighScoreTime > 0) {
          showHighScoreTime = 0;
          isGameTypeVisibled = false;

          if (pressedButton == i + 1) {
            resetGame();
            gameType = i + 1;
            startGame();
          }
        }

        pressedButton = 0;
      }

      break;
    }
  }
}

void resetGame() {
  isShowResetScreen = false;
  gameSpeed = 50;
  isSecondChanged = false;
  gameLoopTime = 0l;
  gameType = 0;
  isBagVisibled = false;
  score = 0;
  miss = 0;
  isPlaying = false;
  add3Score = 0;
  secondCount = 0;
  isScoreVisibled = false;
  onTheBoatCount = 10;
  caught = 0;
  clearMissCount = 0;
  diverPosition = 0;
  isRandomTentacle = false;
  currentTentacle = 0;
  isMissDiverVisibled = false;
  isMissArmVisibled = false;
  isMissLegsVisibled = false;
  isGameTypeVisibled = false;
  isColonVisibled = false;
  isAMVisibled = false;
  isPMVisibled = false;
  memset(&isDiverStepVisibled, false, sizeof(isDiverStepVisibled));
  memset(&isRemainderVisibled, true, sizeof(isRemainderVisibled));
  memset(&isHourDigitVisibled, false, sizeof(isHourDigitVisibled));
  memset(&isMinuteDigitVisibled, false, sizeof(isMinuteDigitVisibled));
  memset(&isEndOfTentacle, false, sizeof(isEndOfTentacle));
  memset(&tentacleDirection, 0, sizeof(tentacleDirection));
  memset(&tentacleCount, 0, sizeof(tentacleCount));
  memset(&isTentacleVisibled, false, sizeof(isTentacleVisibled));
  diverStepImage[0] = onTheBoatBitmap[0];
  isDiverStepVisibled[0] = true;
  showTime();

  if (isFPSVisibled) {
    showFPS(frameRate);
  }
}

void gotCaught() {
  if (clearMissCount == 0) {
    caught = isPlaying ? 11 : 9;
  }
}

void readHighScores() {
  short i = eepromAddress;

  if (espert.eeprom.read(i, eepromKey.length()) == eepromKey) {
    i += eepromKey.length();
    bool invalid = false;
    String data = espert.eeprom.read(i, 3);
    gameAHighScore = data.toInt();
    if (gameAHighScore < 0 || gameAHighScore > 999) {
      gameAHighScore = constrain(gameAHighScore, 0, 999);
      invalid = true;
    }

    i += 3;
    data = espert.eeprom.read(i, 3);
    gameBHighScore = data.toInt();
    if (gameBHighScore < 0 || gameBHighScore > 999) {
      gameBHighScore = constrain(gameBHighScore, 0, 999);
      invalid = true;
    }

    if (invalid) {
      writeHighScores();
    }
  }
}

void writeHighScores() {
  short i = eepromAddress;
  espert.eeprom.write(i, eepromKey);

  i += eepromKey.length();
  espert.eeprom.write(i, intToString(gameAHighScore, 3, "0"));

  i += 3;
  espert.eeprom.write(i, intToString(gameBHighScore, 3, "0"));
}

void initGame() {
  resetGame();
  readHighScores();
  showResetScreen();
  render();

  int showResetScreenTime = 3000; // milliseconds

  if (isTimeVisibled) {
    String internetTime = "";

    if (wifiSSID && strlen(wifiSSID) > 0) {
      showResetScreenTime = 0;
      espert.print("Connecting to WiFi ");
      espert.print(wifiSSID);
      WiFi.begin(wifiSSID, wifiPassword);

      int retry = 20;
      while (WiFi.status() != WL_CONNECTED && --retry > 0) {
        espert.print(".");
        delay(500);
      }

      espert.println();

      if (retry > 0) {
        espert.print("Connected, IP address: ");
        espert.println(WiFi.localIP());

        retry = 3;
        WiFiClient client;
        espert.println("Connecting to google.com to read current time...");
        while (!!!client.connect("google.com", 80) && --retry > 0) {
          espert.println("Retrying...");
        }

        if (retry > 0) {
          client.print("HEAD / HTTP/1.1\r\n\r\n");

          while (!!!client.available()) {
            yield();
          }

          while (client.available()) {
            if (client.read() == '\n') {
              if (client.read() == 'D') {
                if (client.read() == 'a') {
                  if (client.read() == 't') {
                    if (client.read() == 'e') {
                      if (client.read() == ':') {
                        client.read();
                        internetTime = client.readStringUntil('\r');
                      }
                    }
                  }
                }
              }
            }
          }
        } else {
          espert.println("Failed!");
        }

        client.stop();
        espert.println(internetTime); // ddd, dd mmm yyyy hh:mm:ss GMT
      } else {
        espert.println("Failed!");
      }

      WiFi.disconnect();
    }

    if (internetTime.length() > 0) {
      String hh = internetTime.substring(17, 19);
      String mm = internetTime.substring(20, 22);
      String ss = internetTime.substring(23, 25);

      int h = (int)timeZone;
      int m = round((timeZone - h) * 100.0f);
      hours = hh.toInt() + h;
      minutes = mm.toInt() + m;

      if (minutes < 0) {
        minutes += 60;
        hours--;
      } else if (minutes >= 60) {
        minutes -= 60;
        hours++;
      }

      if (hours < 0) {
        hours += 24;
      } else if (hours >= 24) {
        hours -= 24;
      }

      seconds = ss.toInt();
    }
  }

  delay(showResetScreenTime);
  resetGame();
  gameType = 0;
  lastFrameTime = millis();
  fpsLastFrameTime = lastFrameTime;
  timeSyncLastFrameTime = lastFrameTime;
}

void playSound(int index) {
  if (isSoundEnabled && isPlaying) {
    int frequency = 5;

    switch (index) {
      case 0:
        buzzerDuration = 11.0f;
        break;

      case 1:
        buzzerDuration = 97.0f;
        break;

      case 2:
        buzzerDuration = 428.0f;
        break;

      case 3:
        buzzerDuration = 191.0f;
        break;

      default:
        buzzerDuration = 0.0f;
        frequency = 0;
        break;
    }

    espert.buzzer.on(frequency);
  }
}
