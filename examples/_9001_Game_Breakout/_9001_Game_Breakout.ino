// Breakout for ESPresso Lite

#include <ESPert.h>
ESPert espert;

// button pin
#ifdef ARDUINO_ESP8266_ESPRESSO_LITE_V1
int buttonPin[2] = {0, 2};
#else
int buttonPin[2] = {0, 13};
#endif
int gamepadPin[2] = {14, 1};
bool isGamepadEnabled = false;

// sound (buzzer)
static const int buzzerPin = 12;
float buzzerDuration = 0.0f;
bool isSoundEnabled = true;

// level
static const int numberOfLevels = 6;
static const int numberOfRows = 5;
static const int numberOfColumns = 11;

// xx = brickType + itemType, for examples 10 = brick 1 + no item, 21 = brick 2 + item 1, 00 = empty
const int levelTable[numberOfLevels][numberOfRows][numberOfColumns] = {
  { // level 01
    {10, 10, 10, 10, 10, 12, 10, 10, 10, 10, 10},
    {10, 10, 11, 10, 10, 10, 10, 10, 13, 10, 10},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}
  },
  { // level 02
    {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
    {13, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}
  },
  { // level 03
    {23, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11},
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}
  },
  { // level 04
    {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
    {20, 20, 22, 20, 20, 20, 20, 20, 23, 20, 20},
    {11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}
  },
  { // level 05
    {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
    {20, 20, 20, 20, 20, 23, 20, 20, 20, 20, 20},
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11},
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}
  },
  { // level 06
    {20, 20, 20, 20, 20, 22, 20, 20, 20, 20, 20},
    {23, 20, 20, 20, 20, 21, 20, 20, 20, 20, 20},
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 13},
    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    {11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}
  }
};

// OLED bitmaps - Image to Adafruit OLED Bitmap Converter http://www.majer.ch/lcd/adf_bitmap.php
const uint8_t ballBitmap[] PROGMEM = { // ball.png
  0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t brickBitmap[4][16] PROGMEM {
  {0xF9, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // brick0.png
  {0xF9, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // brick1.png
  {0xF9, 0xF0, 0xF0, 0xF0, 0xF6, 0xF6, 0xF0, 0xF0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // brickItem0.png
  {0xF9, 0xF6, 0xF6, 0xF6, 0xF0, 0xF0, 0xF6, 0xF6, 0xF6, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // brickItem1.png
};

const uint8_t breakoutBitmap[] PROGMEM = { // breakout.png
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x0B, 0x15, 0xAB,
  0xD5, 0xEA, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0x01, 0x01, 0x31, 0x39, 0x39, 0x39, 0x19, 0x01, 0x43, 0xC3, 0xFF, 0x7F, 0x0F, 0x01, 0x01, 0x71, 0x79, 0x79, 0x79,
  0x39, 0x01, 0x01, 0xC3, 0xFF, 0x7F, 0x0F, 0x01, 0x01, 0x31, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0xF9, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x83, 0xC1, 0xF1, 0x01, 0x01, 0x0F, 0xFF, 0xFF, 0x7F, 0x0F,
  0x01, 0x01, 0x71, 0x3F, 0x1F, 0x0F, 0x07, 0xE3, 0xF1, 0xF9, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x07, 0x03, 0x63, 0xF1, 0xF1, 0x61, 0x01, 0x02, 0x02, 0x04, 0x18, 0xF0, 0xFC, 0x1E, 0x01, 0x01,
  0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x01, 0x01, 0xE1, 0xFF, 0xFB, 0xF1, 0xF1, 0x11, 0x01, 0x01, 0xE1, 0xF1, 0xF1, 0xF1, 0xF9, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xC0, 0xC0, 0x40, 0x48, 0x4F, 0x4F, 0x4F, 0x4F, 0x47, 0x40, 0x60, 0x70, 0x7F, 0x40, 0x40, 0x40, 0x78, 0x7E, 0x7E, 0x7C, 0x70,
  0x41, 0x43, 0x4F, 0x7F, 0x7F, 0x40, 0x40, 0x40, 0x48, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x7F, 0x5F, 0x47, 0x41, 0x60, 0x78, 0x78, 0x79, 0x79, 0x79, 0x78, 0x40, 0x40, 0x43, 0x7F, 0x40, 0x40,
  0x40, 0x78, 0x7C, 0x7E, 0x7F, 0x7C, 0x70, 0xC0, 0xC3, 0x4F, 0xBF, 0x1F, 0x0F, 0x07, 0x03, 0x06, 0x88, 0xD0, 0x70, 0x60, 0x60, 0x64, 0x62, 0x70, 0x70, 0x78, 0x7E, 0x7F, 0x7F, 0x70, 0x60, 0x40,
  0x4F, 0x4F, 0x4F, 0x4F, 0x63, 0x60, 0x70, 0x7C, 0x7F, 0x7F, 0x7F, 0x47, 0x40, 0x40, 0x40, 0x78, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4,
  0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4,
  0xF4, 0xF4, 0xF4, 0xFC, 0xFC, 0xFE, 0xEF, 0xD5, 0xEA, 0xF5, 0xEA, 0xF5, 0xFA, 0xFC, 0xF6, 0xF7, 0xF5, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4,
  0xF4, 0xF4, 0xF4, 0xF4, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t bulletBitmap[2][8] PROGMEM {
  {0xF1, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // bullet0.png
  {0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // bullet1.png
};

const uint8_t fpsBitmap[] PROGMEM { // fps.png
  0xE0, 0xFA, 0xFE, 0xFF, 0xE0, 0xFA, 0xF8, 0xFF, 0xE8, 0xEA, 0xE2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t gameOverBitmap[] PROGMEM = { // gameOver.png
  0xE0, 0xEA, 0xE2, 0xFF, 0xE0, 0xFA, 0xE0, 0xFF, 0xE0, 0xFD, 0xE0, 0xFF, 0xE0, 0xEA, 0xEA, 0xFF, 0xFF, 0xE0, 0xEE, 0xE0, 0xFF, 0xF0, 0xEF, 0xF0, 0xFF, 0xE0, 0xEA, 0xEA, 0xFF, 0xE0, 0xFA, 0xE4
};

const uint8_t getReadyBitmap[] PROGMEM = { // getReady.png
  0xE0, 0xEA, 0xE2, 0xFF, 0xE0, 0xEA, 0xEA, 0xFF, 0xFE, 0xE0, 0xFE, 0xFF, 0xFF, 0xE0, 0xFA, 0xE4, 0xFF, 0xE0, 0xEA, 0xEA, 0xFF, 0xE0, 0xFA, 0xE0, 0xFF, 0xE0, 0xEE, 0xF1, 0xFF, 0xF8, 0xE3, 0xF8
};

const uint8_t goBitmap[] PROGMEM = { // go.png
  0xE0, 0xEA, 0xE2, 0xFF, 0xE0, 0xEE, 0xE0, 0xFF
};

const uint8_t itemBitmap[3][8] PROGMEM {
  {0xE3, 0xC9, 0x88, 0x88, 0x88, 0xC9, 0xE3, 0xFF}, // item0.png
  {0xE3, 0xC1, 0x8C, 0x8C, 0x90, 0xC1, 0xE3, 0xFF}, // item1.png
  {0xE3, 0xC1, 0x80, 0xBE, 0x80, 0xC1, 0xE3, 0xFF}  // item2.png
};

const uint8_t levelClearedBitmap[] PROGMEM = { // levelCleared.png
  0xE0, 0xEF, 0xEF, 0xFF, 0xE0, 0xEA, 0xEA, 0xFF, 0xF0, 0xEF, 0xF0, 0xFF, 0xE0, 0xEA, 0xEA, 0xFF, 0xE0, 0xEF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xEE, 0xEE, 0xFF, 0xE0, 0xEF, 0xEF, 0xFF,
  0xE0, 0xEA, 0xEA, 0xFF, 0xE0, 0xFA, 0xE0, 0xFF, 0xE0, 0xFA, 0xE4, 0xFF, 0xE0, 0xEA, 0xEA, 0xFF, 0xE0, 0xEE, 0xF1, 0xFF, 0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t numberBitmap[10][8] PROGMEM {
  {0xE0, 0xEE, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number0.png
  {0xFF, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number1.png
  {0xE2, 0xEA, 0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number2.png
  {0xEA, 0xEA, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number3.png
  {0xF8, 0xFB, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number4.png
  {0xE8, 0xEA, 0xE2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number5.png
  {0xE0, 0xEA, 0xE2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number6.png
  {0xFE, 0xFE, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number7.png
  {0xE0, 0xEA, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // number8.png
  {0xE8, 0xEA, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // number9.png
};

const uint8_t paddleBitmap[6][32] PROGMEM {
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xF0, 0xE2, 0xC2, 0xE2, 0xFA, 0xFA, 0xF2, 0xE2, 0xE2, 0xE2, 0xE2, 0xF2, 0xFA, 0xFA, 0xE2, 0xC2, 0xE2, 0xF0, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // paddle0.png
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xF0, 0xE2, 0xE2, 0xC2, 0xE2, 0xFA, 0xFA, 0xF2, 0xE2, 0xE2, 0xE2, 0xE2, 0xF2, 0xFA, 0xFA, 0xE2, 0xC2, 0xE2, 0xE2, 0xF0, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF}, // paddle1.png
  {0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xF0, 0xE2, 0xE2, 0xE2, 0xC2, 0xE2, 0xFA, 0xFA, 0xF2, 0xE2, 0xE2, 0xE2, 0xE2, 0xF2, 0xFA, 0xFA, 0xE2, 0xC2, 0xE2, 0xE2, 0xE2, 0xF0, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF}, // paddle2.png
  {0xFF, 0xFF, 0xFC, 0xF8, 0xF0, 0xE2, 0xE2, 0xF2, 0xE2, 0xC2, 0xE2, 0xFA, 0xFA, 0xF2, 0xE2, 0xE2, 0xE2, 0xE2, 0xF2, 0xFA, 0xFA, 0xE2, 0xC2, 0xE2, 0xF2, 0xE2, 0xE2, 0xF0, 0xF8, 0xFC, 0xFF, 0xFF}, // paddle3.png
  {0xFF, 0xFC, 0xF8, 0xF0, 0xE2, 0xE2, 0xFA, 0xF2, 0xE2, 0xC2, 0xE2, 0xFA, 0xFA, 0xF2, 0xE2, 0xE2, 0xE2, 0xE2, 0xF2, 0xFA, 0xFA, 0xE2, 0xC2, 0xE2, 0xF2, 0xFA, 0xE2, 0xE2, 0xF0, 0xF8, 0xFC, 0xFF}, // paddle4.png
  {0xFF, 0xF8, 0xF0, 0xE0, 0xC2, 0xC2, 0xFA, 0xF2, 0xE2, 0xC2, 0xE2, 0xFA, 0xFA, 0xF2, 0xE2, 0xE2, 0xE2, 0xE2, 0xF2, 0xFA, 0xFA, 0xE2, 0xC2, 0xE2, 0xF2, 0xFA, 0xC2, 0xC2, 0xE0, 0xF0, 0xF8, 0xFF}  // paddle5.png
};

const uint8_t statusBarBitmap[2][128] PROGMEM = {
  { // statusBar0.png
    0xFF, 0xC1, 0xDF, 0xDF, 0xFF, 0xC1, 0xD5, 0xD5, 0xFF, 0xE1, 0xDF, 0xE1, 0xFF, 0xC1, 0xD5, 0xD5, 0xFF, 0xC1, 0xDF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1, 0xD5, 0xEB, 0xFF, 0xC1, 0xF5, 0xC1, 0xFF, 0xC1, 0xDF, 0xDF, 0xFF, 0xC1, 0xDF,
    0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD1, 0xD5, 0xC5, 0xFF, 0xC1, 0xDD, 0xDD,
    0xFF, 0xC1, 0xDD, 0xC1, 0xFF, 0xC1, 0xF5, 0xC9, 0xFF, 0xC1, 0xD5, 0xD5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
  },
  { // statusBar1.png
    0xFF, 0xC1, 0xDF, 0xDF, 0xFF, 0xC1, 0xD5, 0xD5, 0xFF, 0xE1, 0xDF, 0xE1, 0xFF, 0xC1, 0xD5, 0xD5, 0xFF, 0xC1, 0xDF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xC1, 0xD5, 0xEB, 0xFF, 0xC1, 0xF5, 0xC1, 0xFF, 0xC1, 0xDF, 0xDF, 0xFF, 0xC1, 0xDF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD1,
    0xD5, 0xC5, 0xFF, 0xC1, 0xDD, 0xDD, 0xFF, 0xC1, 0xDD, 0xC1, 0xFF, 0xC1, 0xF5, 0xC9, 0xFF, 0xC1, 0xD5, 0xD5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1, 0xF5, 0xFD, 0xFF, 0xC1, 0xF5, 0xF1, 0xFF, 0xD1, 0xD5, 0xC5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
  }
};

const uint8_t statusBarBackgroundBitmap[] PROGMEM = { // statusBarBackground.png
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};

const uint8_t titleBitmap[] PROGMEM = { // title.png
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xBF, 0x0F, 0xFF, 0x0F, 0xFF, 0xBF, 0xFF, 0x8F, 0xAF, 0x2F, 0xFF, 0x0F, 0xEF,
  0xEF, 0xFF, 0x0F, 0xEF, 0x0F, 0xFF, 0x0F, 0xAF, 0x4F, 0xFF, 0x0F, 0xAF, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFF, 0xFE, 0xFE,
  0xFE, 0xFF, 0xFE, 0xFE, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x81, 0xA5, 0x18, 0x18, 0xA5, 0x81, 0xC3, 0xFF, 0xFF, 0x83, 0xFB, 0xC7, 0xFB, 0x83, 0xFF, 0x83, 0xEB, 0x83, 0xFF, 0x83, 0xEF, 0x93,
  0xFF, 0x83, 0xAB, 0xAB, 0xFF, 0x83, 0xEB, 0x93, 0xFF, 0x83, 0xEB, 0x83, 0xFF, 0xA3, 0xAB, 0x8B, 0xFF, 0x83, 0xFF, 0x83, 0xEB, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t wallBitmap[] PROGMEM = { // wall.png
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

enum {
  GAME_MODE_TITLE,
  GAME_MODE_TITLE_BLINK,
  GAME_MODE_GET_READY,
  GAME_MODE_PLAY,
  GAME_MODE_MISS,
  GAME_MODE_LEVEL_CLEARED,
  GAME_MODE_GAME_OVER
};

enum {
  SOUND_LAUNCH_BALL,
  SOUND_HIT_PADDLE,
  SOUND_HIT_BORDER,
  SOUND_HIT_BLOCK,
  SOUND_MISS
};

enum {
  ITEM_TYPE_NONE,
  ITEM_TYPE_EXPAND,
  ITEM_TYPE_BALL,
  ITEM_TYPE_BULLET
};

struct Point {
  float x;
  float y;
};

// button
ESPert_Button button[2];
static const int maxButtonDelay = 10; // milliseconds
float buttonDelay = 0.0f;
bool isButtonPressed[2] = {false};
int pressedButton = 0;

// game time
unsigned long lastFrameTime = 0l;
float elapsedTime = 0.0f;
unsigned long frameCount = 0l;
unsigned long frameRate = 0l;
unsigned long fpsLastFrameTime = 0l;
bool isFPSVisibled = false;
const uint8_t* fpsDigitImage[2] = {NULL};
bool isFPSDigitVisibled[2] = {false};
Point fpsDigitPosition[2] = {{120, 1}, {124, 1}};

// game
static const float maxTitleBlinkTime = 3000.0f;
static const float maxGetReadyTime = 6000.0f;
static const float maxMissTime = 3000.0f;
static const float maxGameOverTimeOut = 10000.0f;
static const float maxLevelClearedTime = 3000.0f;
int screenRect[4] = {1, 7, 126, 63}; // left, top, right, bottom
int gameMode = GAME_MODE_TITLE;
float titleBlinkTime = 0.0f;
float getReadyTime = 0.0f;
float missTime = 0.0f;
bool miss = false;
float gameOverTimeOut = 0.0f;
float levelClearedTime = 0.0f;
bool isReadyToPlay = false;
int infoY = 8;

// paddle
static const float paddleSpeed = 150.0f / 1000.0f;
float paddleExpandSpeed = 7.0f / 1000.0f;
static const float maxPaddleExpandTimeOut = 10000.0f;
float paddleExpandTimeOut = 0.0f;
static const int numberOfPaddleTypes = 6;
Point paddleSize[numberOfPaddleTypes] = {{22, 6}, {24, 6}, {26, 6}, {28, 6}, {30, 6}, {30, 6}};
float paddleType = 0.0f;
bool isPaddleExpand = false;
Point paddlePosition = {0.0f};
bool isUpdatePaddlePosition = false;

// ball
static const int mainBall = 0;
static const int newGameBallLeft = 3;
static const float defaultBallSpeed = 20.0f / 1000.0f;
Point ballSize = {2, 2};
int ballLeft = 0;
const uint8_t* ballDigitImage = NULL;
Point ballDigitPosition = {69, 1};
static const int maxBall = 3;
int numberOfBalls = 0;
bool isBallCollide[maxBall][2] = {false};
float ballPosition[maxBall][2] = {0.0f};
int ballDirection[maxBall][2] = {0};
float ballSpeed[maxBall] = {defaultBallSpeed};
int ballHitCount[maxBall] = {0};
int ballHitRowFirstTimeFlag[maxBall][numberOfRows] = {0};

// brick
static const int numberOfBrickType = 2;
static const int defaultBrickPower[numberOfBrickType] = {1, 2};
const uint8_t* brickImage[numberOfRows][numberOfColumns] = {NULL};
int brickScore[numberOfRows] = {0};
Point brickPosition[numberOfRows][numberOfColumns] = {0};
int brickPower[numberOfRows][numberOfColumns] = {0};
static const Point brickSize = {10, 4};
static const Point brickGap = {1, 1};
static const Point brickStartPosition = {4, 14};
int brickCount = 0;
float rowFirstHitSpeed[numberOfRows] = {0.0f};

// level
int level = 0;
int currentLevelTable[numberOfRows][numberOfColumns] = {0};
const uint8_t* levelDigitImage[2] = {NULL};
Point levelDigitPosition[2] = {{24, 1}, {28, 1}};
int rowCount = 0;
bool isUpdateRowCounter = false;

// score
String eepromKey = "Breakout";
int eepromAddress = 512 - eepromKey.length() - 4;
int highScore = 0;
int score = 0;
const uint8_t* scoreDigitImage[4] = {NULL};
const uint8_t* highScoreDigitImage[4] = {NULL};

// bullet
static const float maxBulletTimeOut = 10000.0f;
float bulletTimeOut = 0.0f;
static const int numberOfBullets = 8;
const uint8_t* bulletImage[numberOfBullets] = {NULL};
bool isBulletCollide[numberOfBullets] = {false};
Point bulletSize = {2, 5};
static const float maxBulletSpawnTime = 1000.0f;
float bulletSpawnTime = 0.0f;
static const float bulletSpeed = 40.0f / 1000.0f;
Point bulletPosition[numberOfBullets] = {0.0f};
bool isBulletEnabled = false;

// item
int itemCount = 0;
Point itemSize = {7, 7};
static const float itemSpeed = 10.0f / 1000.0f;
static const int numberOfItems = 5;
const uint8_t* itemImage[numberOfItems] = {NULL};
int itemType[numberOfItems] = {ITEM_TYPE_NONE};
Point itemPosition[numberOfItems] = {0.0f};
bool isItemCollide[numberOfItems] = {false};
static const float maxAddBallBlinkTime = 3000.0f;
float addBallBlinkTime = 0.0f;

// function prototypes
void update();
void render();
void pressButtons();
void checkButtons();
void checkHighScore();
void readHighScores();
void writeHighScores();
String intToString(int value, int length, String prefixChar = "0");
void playSound(int index);
void setScoreImages();
void setHighScoreImages();
void setLevelImages();
void setBallImages();
void initGame();
void playLoop();
void changeGameMode(int mode);
void loadLevel();
void updatePaddle();
bool isHitBrick(int x, int y, int width, int height, int directionX, int directionY, int ball = -1);
void addScore(int value);
void addBall(int value);
void deleteItem(int i);

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
    setFPSImages(frameRate);
  }

  // button
  buttonDelay += elapsedTime;
  if (buttonDelay >= maxButtonDelay) {
    buttonDelay = 0.0f;
    pressButtons();
  }
  checkButtons();

  // sound
  if (buzzerDuration > 0.0f) {
    buzzerDuration -= elapsedTime;

    if (buzzerDuration <= 0.0f) {
      buzzerDuration = 0.0f;
      espert.buzzer.off();
    }
  }

  switch (gameMode) {
    case GAME_MODE_TITLE:
      break;

    case GAME_MODE_TITLE_BLINK:
      if (titleBlinkTime < maxTitleBlinkTime) {
        titleBlinkTime += elapsedTime;

        if (titleBlinkTime >= maxTitleBlinkTime) {
          changeGameMode(GAME_MODE_GET_READY);
        }
      }
      break;

    case GAME_MODE_GET_READY:
      if (getReadyTime < maxGetReadyTime) {
        getReadyTime += elapsedTime;

        if (rowCount < numberOfRows && getReadyTime <= 1500.0f) {
          if ((int)getReadyTime % 300 < 150) {
            if (!isUpdateRowCounter) {
              isUpdateRowCounter = true;
              rowCount++;
            }
          } else {
            isUpdateRowCounter = false;
          }
        } else if (!isReadyToPlay && getReadyTime > 1500.0f) {
          isReadyToPlay = true;

          if (miss) {
            if (ballLeft > 0) {
              ballLeft--;
            }

            setBallImages();
            miss = false;
          }
        } else if (getReadyTime >= maxGetReadyTime) {
          changeGameMode(GAME_MODE_PLAY);
        }
      }
      break;

    case GAME_MODE_PLAY:
      playLoop();
      break;

    case GAME_MODE_MISS:
      if (missTime < maxMissTime) {
        missTime += elapsedTime;

        if (missTime >= maxMissTime) {
          changeGameMode((ballLeft == 0) ? GAME_MODE_GAME_OVER : GAME_MODE_GET_READY);
        }
      }
      break;

    case GAME_MODE_LEVEL_CLEARED:
      if (levelClearedTime < maxLevelClearedTime) {
        levelClearedTime += elapsedTime;

        if (levelClearedTime >= maxLevelClearedTime) {
          if (++level >= numberOfLevels) {
            level = 0;
          }

          loadLevel();
          changeGameMode(GAME_MODE_GET_READY);
        }
      }
      break;

    case GAME_MODE_GAME_OVER:
      if (gameOverTimeOut < maxGameOverTimeOut) {
        gameOverTimeOut += elapsedTime;

        if (gameOverTimeOut >= maxGameOverTimeOut) {
          changeGameMode(GAME_MODE_TITLE);
        }
      }
      break;
  }

  if (gameMode == GAME_MODE_GET_READY || gameMode == GAME_MODE_PLAY || gameMode == GAME_MODE_MISS || gameMode == GAME_MODE_LEVEL_CLEARED) {
    updatePaddle();

    if (addBallBlinkTime > 0.0f) {
      addBallBlinkTime -= elapsedTime;

      if (addBallBlinkTime <= 0.0f) {
        addBallBlinkTime = 0.0f;
      }
    }

    if (gameMode == GAME_MODE_PLAY && brickCount == 0 && itemCount == 0) {
      changeGameMode(GAME_MODE_LEVEL_CLEARED);
    }
  }

  ESP.wdtFeed();
}

void render() {
  espert.oled.clear(false);
  espert.oled.setColor(ESPERT_WHITE);

  switch (gameMode) {
    case GAME_MODE_TITLE:
    case GAME_MODE_TITLE_BLINK:
      espert.oled.drawBitmap(0, 0, 128, 64, titleBitmap, false);

      for (int i = 0; i < 4; i++) {
        espert.oled.drawBitmap(56 + (i * 4), 43, 8, 8, highScoreDigitImage[i], false);
      }

      if (gameMode == GAME_MODE_TITLE || (gameMode == GAME_MODE_TITLE_BLINK && ((int)titleBlinkTime % 500 < 250))) {
        espert.oled.drawBitmap(0, 0, 128, 32, breakoutBitmap, false);
      }
      break;

    case GAME_MODE_GET_READY:
    case GAME_MODE_PLAY:
    case GAME_MODE_MISS:
    case GAME_MODE_LEVEL_CLEARED:
    case GAME_MODE_GAME_OVER:
      if (gameMode != GAME_MODE_LEVEL_CLEARED) {
        for (int row = 0; row < rowCount; row++) {
          for (int column = 0; column < numberOfColumns; column++) {
            if (brickImage[row][column]) {
              espert.oled.drawBitmap(brickPosition[row][column].x, brickPosition[row][column].y, 16, 8, brickImage[row][column], false);
            }
          }
        }

        for (int i = 0; i < numberOfBullets; i++) {
          if (bulletImage[i]) {
            espert.oled.drawBitmap(bulletPosition[i].x, bulletPosition[i].y, 8, 8, bulletImage[i], false);
          }
        }
      }

      for (int i = 0; i < numberOfItems; i++) {
        if (itemImage[i]) {
          espert.oled.drawBitmap(itemPosition[i].x, itemPosition[i].y, 8, 8, itemImage[i], false);
        }
      }

      espert.oled.drawBitmap(paddlePosition.x - 16, paddlePosition.y, 32, 8, paddleBitmap[(int)paddleType], false);

      if (((gameMode == GAME_MODE_GET_READY && isReadyToPlay && miss) || (gameMode == GAME_MODE_GET_READY && !miss) || gameMode == GAME_MODE_PLAY || gameMode == GAME_MODE_LEVEL_CLEARED) || (gameMode == GAME_MODE_MISS && ((int)missTime % 500) < 250)) {
        for (int ball = 0; ball < numberOfBalls; ball++) {
          if (ballDirection[ball][0] != 0 && ballDirection[ball][1] != 0) {
            espert.oled.drawBitmap(ballPosition[ball][0], ballPosition[ball][1], 8, 8, ballBitmap, false);
          }
        }
      }

      // status bar
      espert.oled.drawBitmap(0, 0, 128, 8, statusBarBackgroundBitmap, false);

      if (gameMode == GAME_MODE_GET_READY) {
        if (getReadyTime <= 1500.0f) {
          espert.oled.drawBitmap(48, infoY, 32, 8, getReadyBitmap, false);
        }
        else if (getReadyTime >= 2000.0f && getReadyTime <= 2500.0f) {
          espert.oled.drawBitmap(62, infoY, 8, 8, numberBitmap[3], false);
        }
        else if (getReadyTime >= 3000.0f && getReadyTime <= 3500.0f) {
          espert.oled.drawBitmap(62, infoY, 8, 8, numberBitmap[2], false);
        }
        else if (getReadyTime >= 4000.0f && getReadyTime <= 4500.0f) {
          espert.oled.drawBitmap(62, infoY, 8, 8, numberBitmap[1], false);
        }
        else if (getReadyTime >= 5000.0f) {
          espert.oled.drawBitmap(60, infoY, 8, 8, goBitmap, false);
        }
      } else if (gameMode == GAME_MODE_GAME_OVER && ((int)gameOverTimeOut % 500) < 250) {
        espert.oled.drawBitmap(48, infoY, 32, 8, gameOverBitmap, false);
      } else if (gameMode == GAME_MODE_LEVEL_CLEARED && ((int)levelClearedTime % 500) < 250) {
        espert.oled.drawBitmap(37, infoY, 64, 8, levelClearedBitmap, false);
      }

      espert.oled.drawBitmap(0, 0, 8, 64, wallBitmap, false);
      espert.oled.drawBitmap(127, 0, 8, 64, wallBitmap, false);
      espert.oled.setColor(ESPERT_BLACK);
      espert.oled.drawBitmap(0, 0, 128, 8, isFPSVisibled ? statusBarBitmap[1] : statusBarBitmap[0], false);

      if (((int)addBallBlinkTime % 200) < 100) {
        espert.oled.drawBitmap(ballDigitPosition.x - (isFPSVisibled ? 14 : 0), ballDigitPosition.y, 8, 8, ballDigitImage, false);
      }

      for (int i = 0; i < 4; i++) {
        espert.oled.drawBitmap(112 - (isFPSVisibled ? 26 : 0) + (i * 4), 1, 8, 8, scoreDigitImage[i], false);

        if (i < 2) {
          espert.oled.drawBitmap(levelDigitPosition[i].x, levelDigitPosition[i].y, 8, 8, levelDigitImage[i], false);

          if (isFPSVisibled && isFPSDigitVisibled[i]) {
            espert.oled.drawBitmap(fpsDigitPosition[i].x, fpsDigitPosition[i].y, 8, 8, fpsDigitImage[i], false);
          }
        }
      }
      break;
  }

  espert.oled.update();
  ESP.wdtFeed();
}

void setFPSImages(int value) {
  if (isFPSVisibled) {
    value = constrain(value, 0, 99);
    String fpsString = intToString(value, 2, "0");

    for (int i = 0; i < 2; i++) {
      fpsDigitImage[i] = numberBitmap[fpsString.charAt(i) - '0'];
      isFPSDigitVisibled[i] = isFPSVisibled;
    }

    if (value > 0 && value < 10) {
      isFPSDigitVisibled[0] = false;
    }
  }
}

void pressButtons() {
  for (int i = 0; i < 2; i++) {
    bool isPressed = (digitalRead(buttonPin[i]) == LOW) ? true : false;

    if (isPressed != isButtonPressed[i]) {
      isButtonPressed[i] = isPressed;

      if (isPressed) {
        pressedButton = i + 1;
      } else if (pressedButton == i + 1) {
        pressedButton = 0;

        if (gameMode == GAME_MODE_TITLE) {
          changeGameMode(GAME_MODE_TITLE_BLINK);
        } else if (gameMode == GAME_MODE_GAME_OVER) {
          changeGameMode(GAME_MODE_TITLE);
        }
      }

      break;
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

void readHighScores() {
  int i = eepromAddress;

  if (espert.eeprom.read(i, eepromKey.length()) == eepromKey) {
    i += eepromKey.length();
    String data = espert.eeprom.read(i, 4);
    highScore = data.toInt();

    if (highScore < 0 || highScore > 9999) {
      highScore = constrain(highScore, 0, 9999);
      writeHighScores();
    }
  }
}

void writeHighScores() {
  int i = eepromAddress;
  espert.eeprom.write(i, eepromKey);

  i += eepromKey.length();
  espert.eeprom.write(i, intToString(highScore, 4, "0"));
}

void playSound(int index) {
  if (isSoundEnabled) {
    int frequency = 0;

    switch (index) {
      case SOUND_LAUNCH_BALL:
      case SOUND_HIT_PADDLE:
        frequency = 5;
        buzzerDuration = 20.0f;
        break;

      case SOUND_HIT_BORDER:
        frequency = 10;
        buzzerDuration = 20.0f;
        break;

      case SOUND_HIT_BLOCK:
        frequency = 20;
        buzzerDuration = 20.0f;
        break;

      case SOUND_MISS:
        frequency = 5;
        buzzerDuration = 428.0f;
        break;

      default:
        frequency = 0;
        buzzerDuration = 0.0f;
        break;
    }

    espert.buzzer.on(frequency);
  }
}

void setScoreImages() {
  String string = intToString(score, 4, "0");

  for (int i = 0; i < 4; i++) {
    scoreDigitImage[i] = numberBitmap[string.charAt(i) - '0'];
  }
}

void setHighScoreImages() {
  String string = intToString(highScore, 4, "0");

  for (int i = 0; i < 4; i++) {
    highScoreDigitImage[i] = numberBitmap[string.charAt(i) - '0'];
  }
}

void setLevelImages() {
  String string = intToString(level + 1, 2, "0");

  for (int i = 0; i < 2; i++) {
    levelDigitImage[i] = numberBitmap[string.charAt(i) - '0'];
  }
}

void setBallImages() {
  String string = intToString((ballLeft > 0) ? ballLeft : 0, 1, "0");
  ballDigitImage = numberBitmap[string.charAt(0) - '0'];
}

void checkButtons() {
  switch (gameMode) {
    case GAME_MODE_TITLE:
    case GAME_MODE_TITLE_BLINK:
      break;

    case GAME_MODE_GET_READY:
    case GAME_MODE_PLAY:
    case GAME_MODE_MISS:
    case GAME_MODE_LEVEL_CLEARED:
      if (pressedButton == 1) {
        paddlePosition.x -= elapsedTime * paddleSpeed;
        isUpdatePaddlePosition = true;
      } else if (pressedButton == 2) {
        paddlePosition.x += elapsedTime * paddleSpeed;
        isUpdatePaddlePosition = true;
      }
      break;

    case GAME_MODE_GAME_OVER:
      break;
  }
}

void playLoop() {
  for (int ball = 0; ball < numberOfBalls; ball++) {
    for (int i = 0; i < 2; i++) {
      if (isBallCollide[ball][i]) {
        isBallCollide[ball][i] = false;
        ballDirection[ball][i] *= -1;
      }
    }

    if (ballDirection[ball][0] != 0 && ballDirection[ball][1] != 0) {
      float speed = elapsedTime * ballSpeed[ball];
      Point newPosition = {ballPosition[ball][0] + (ballDirection[ball][0] * speed), ballPosition[ball][1] + (ballDirection[ball][1] * speed)};

      // collision
      while ((int)ballPosition[ball][0] != (int)newPosition.x && (int)ballPosition[ball][1] != (int)newPosition.y && !isBallCollide[ball][0] && !isBallCollide[ball][1]) {
        ballPosition[ball][0] += ballDirection[ball][0];
        ballPosition[ball][1] += ballDirection[ball][1];

        // paddle
        if ((int)ballPosition[ball][1] >= paddlePosition.y - 1 && (int)ballPosition[ball][1] <= paddlePosition.y + 1) {
          int paddleWidth = paddleSize[(int)paddleType].x;
          int paddleX = paddlePosition.x - (paddleWidth * 0.5f);

          if ((int)ballPosition[ball][0] + ballSize.x - 1 >= paddleX && (int)ballPosition[ball][0] < paddleX + paddleWidth) {
            isBallCollide[ball][1] = true;
            ballPosition[ball][1] = paddlePosition.y - 1.0f;
            playSound(SOUND_HIT_PADDLE);

            if ((int)ballPosition[ball][0] + ballSize.x - 1 <= paddleX + 4) { // left corner
              if (ballDirection[ball][0] == 1) {
                isBallCollide[ball][0] = true;
              }
            } else if ((int)ballPosition[ball][0] >= paddleX + paddleWidth - 4) { // right corner
              if (ballDirection[ball][0] == -1) {
                isBallCollide[ball][0] = true;
              }
            }
          }
        }

        // border
        for (int i = 0; i < 2 && !isBallCollide[ball][0] && !isBallCollide[ball][1]; i++) {
          if ((int)ballPosition[ball][i] < screenRect[i]) { // left and top
            ballPosition[ball][i] = screenRect[i];
            isBallCollide[ball][i] = true;
            playSound(SOUND_HIT_BORDER);
          } else { // right and bottom
            float pos = screenRect[2 + i] - (i == 0 ? ballSize.x : ballSize.y) + 1;

            if ((int)ballPosition[ball][i] > pos) {
              ballPosition[ball][i] = pos;
              isBallCollide[ball][i] = true;

              if (i == 0) { // right
                playSound(SOUND_HIT_BORDER);
              } else { // bottom
                if (numberOfBalls == 1) {
                  changeGameMode(GAME_MODE_MISS);
                } else if (numberOfBalls > 1) {
                  numberOfBalls--;
                  ballDirection[ball][0] = 0;
                  ballDirection[ball][1] = 0;
                }
              }
            }
          }
        }

        // brick
        if (!isBallCollide[ball][0] && !isBallCollide[ball][1]) {
          bool isCollide = isHitBrick(ballPosition[ball][0], ballPosition[ball][1], ballSize.x, ballSize.y, ballDirection[ball][0], ballDirection[ball][1], ball);

          // increase ball speed
          if (isCollide) {
            if (++ballHitCount[ball] == 4) { // after four hits
              ballSpeed[ball] += 0.005f;
            } else if (ballHitCount[ball] == 12) { // after twelve hits
              ballSpeed[ball] += 0.005f;
            } else { // first hit a brick in the row
              for (int i = 0; i < numberOfRows; i++) {
                if (ballHitRowFirstTimeFlag[ball][i] == 1) {
                  ballHitRowFirstTimeFlag[ball][i] = 2;
                  ballSpeed[ball] += rowFirstHitSpeed[i];
                }
              }
            }
          }
        }
      }

      if (!isBallCollide[ball][0] && !isBallCollide[ball][1]) {
        ballPosition[ball][0] = newPosition.x;
        ballPosition[ball][1] = newPosition.y;
      }
    }
  }
}

void updatePaddle() {
  if (isPaddleExpand) {
    if (paddleType != numberOfPaddleTypes - 1) {
      isUpdatePaddlePosition = true;
      paddleType += elapsedTime * paddleExpandSpeed;

      if (paddleType >= numberOfPaddleTypes - 1) {
        paddleType = numberOfPaddleTypes - 1;
        paddleExpandTimeOut = 0.0f;
      }
    } else if (gameMode == GAME_MODE_PLAY) {
      paddleExpandTimeOut += elapsedTime;

      if (paddleExpandTimeOut >= maxPaddleExpandTimeOut) {
        paddleExpandTimeOut = 0.0f;
        isPaddleExpand = false;
      }
    }
  } else {
    if (paddleType != 0.0f) {
      isUpdatePaddlePosition = true;
      paddleType -= elapsedTime * paddleExpandSpeed;

      if (paddleType <= 0.0f) {
        paddleType = 0.0f;
        paddleExpandTimeOut = 0.0f;
      }
    }
  }

  int paddleHalfSize = paddleSize[(int)paddleType].x * 0.5f;

  if (isUpdatePaddlePosition) {
    isUpdatePaddlePosition = false;
    float leftPosition = screenRect[0] + paddleHalfSize;

    if (paddlePosition.x < leftPosition) {
      paddlePosition.x = leftPosition;
    } else {
      float rightPosition = screenRect[2] - paddleHalfSize + 1;
      if (paddlePosition.x > rightPosition) {
        paddlePosition.x = rightPosition;
      }
    }
  }

  if (gameMode == GAME_MODE_GET_READY || gameMode == GAME_MODE_LEVEL_CLEARED) {
    ballPosition[mainBall][0] = paddlePosition.x - (ballSize.x * 0.5f);
    ballPosition[mainBall][1] = paddlePosition.y - ballSize.y - 1;
  }

  if (gameMode == GAME_MODE_PLAY) {
    // bullet
    if (isBulletEnabled) {
      bulletSpawnTime += elapsedTime;

      if (bulletSpawnTime >= maxBulletSpawnTime) {
        bulletSpawnTime = 0.0f;
        int emptyCount = 0;
        int emptyIndex[2] = {0};
        for (int i = 0; i < numberOfBullets && emptyCount < 2; i++) {
          if (bulletImage[i] == NULL) {
            emptyIndex[emptyCount] = i;
            emptyCount++;
          }
        }

        if (emptyCount == 2) {
          for (int i = 0; i < emptyCount; i++) {
            bulletImage[emptyIndex[i]] = bulletBitmap[i];
            bulletPosition[emptyIndex[i]].x = paddlePosition.x + ((i == 0) ? -paddleHalfSize + 2 : (paddleHalfSize - (bulletSize.x * 0.5f) - 3.0f));
            bulletPosition[emptyIndex[i]].y = paddlePosition.y - 4.0f;
          }
        }
      }

      bulletTimeOut += elapsedTime;

      if (bulletTimeOut >= maxBulletTimeOut) {
        bulletTimeOut = 0.0f;
        isBulletEnabled = false;
      }
    }

    for (int i = 0; i < numberOfBullets; i++) {
      if (bulletImage[i]) {
        if (isBulletCollide[i]) {
          isBulletCollide[i] = false;
          bulletImage[i] = NULL;
        } else {
          float newBulletPosition = bulletPosition[i].y - (elapsedTime * bulletSpeed);
          float topPosition = screenRect[1] - bulletSize.y;

          while ((int)bulletPosition[i].y != (int)newBulletPosition && !isBulletCollide[i]) {
            bulletPosition[i].y -= 1.0f;

            if ((int)bulletPosition[i].y <= topPosition) {
              bulletPosition[i].y = topPosition;
              isBulletCollide[i] = true;
            }

            if (!isBulletCollide[i]) {
              isBulletCollide[i] = isHitBrick(bulletPosition[i].x, bulletPosition[i].y, bulletSize.x, bulletSize.y, 0, -1, -1);
            }
          }

          if (!isBulletCollide[i]) {
            bulletPosition[i].y = newBulletPosition;
          }
        }
      }
    }

    // item
    for (int i = 0; i < numberOfItems; i++) {
      if (itemImage[i]) {
        if (isItemCollide[i]) {
          deleteItem(i);

          switch (itemType[i]) {
            case ITEM_TYPE_EXPAND:
              isPaddleExpand = true;
              paddleExpandTimeOut = 0.0f;
              bulletSpawnTime = maxBulletSpawnTime;
              break;

            case ITEM_TYPE_BALL:
              addBall(3);
              addBallBlinkTime = maxAddBallBlinkTime;
              break;

            case ITEM_TYPE_BULLET:
              isBulletEnabled = true;
              bulletTimeOut = 0.0f;
              bulletSpawnTime = maxBulletSpawnTime;
              break;
          }
        } else {
          float newItemPosition = itemPosition[i].y + (elapsedTime * itemSpeed);

          while ((int)itemPosition[i].y != (int)newItemPosition && !isItemCollide[i]) {
            itemPosition[i].y += 1.0f;
            int y = (int)itemPosition[i].y + itemSize.y - 1;

            if (y == paddlePosition.y - 1) {
              int paddleWidth = paddleSize[(int)paddleType].x;
              int paddleX = paddlePosition.x - (paddleWidth * 0.5f);

              if ((int)itemPosition[i].x + itemSize.x - 1 >= paddleX && (int)itemPosition[i].x < paddleX + paddleWidth) {
                isItemCollide[i] = true;
                playSound(SOUND_HIT_PADDLE);
              }
            }

            if (!isItemCollide[i] && itemPosition[i].y > screenRect[3]) { // miss item
              deleteItem(i);
            }
          }

          if (!isItemCollide[i] && itemImage[i]) {
            itemPosition[i].y = newItemPosition;
          }
        }
      }
    }
  }
}

bool isHitBrick(int x, int y, int width, int height, int directionX, int directionY, int ball) {
  bool isCollide = false;

  for (int row = 0; row < numberOfRows && !isCollide; row++) {
    for (int column = 0; column < numberOfColumns && !isCollide; column++) {
      if (brickImage[row][column]) {
        float w = (width + brickSize.x) * 0.5f;
        float h = (height + brickSize.y) * 0.5f;
        Point center = {x + (width * 0.5f), y + (height * 0.5f)};
        Point centerBrick = {brickPosition[row][column].x + (brickSize.x * 0.5f), brickPosition[row][column].y + (brickSize.y * 0.5f)};

        float dx = center.x - centerBrick.x;
        float dy = center.y - centerBrick.y;

        if (abs(dx) <= w && abs(dy) <= h) {
          isCollide = true;
          float wy = w * dy;
          float hx = h * dx;

          if (wy > hx) {
            if (wy > -hx) { // hit top
              isBallCollide[ball][1] = true;
            } else { // hit left
              isBallCollide[ball][0] = true;
            }
          } else {
            if (wy > -hx) { // hit right
              isBallCollide[ball][0] = true;
            } else { // hit bottom
              isBallCollide[ball][1] = true;
            }
          }

          playSound(SOUND_HIT_BLOCK);

          if (--brickPower[row][column] == 0) {
            brickImage[row][column] = NULL;
            int brickType = (currentLevelTable[row][column] / 10) - 1;
            addScore(brickScore[row]);

            if (ball != -1 && ballHitRowFirstTimeFlag[ball][row] == 0) {
              ballHitRowFirstTimeFlag[ball][row] = 1;
            }

            if (--brickCount <= 0) {
              brickCount = 0;
            }

            // spawn item
            int typ = (currentLevelTable[row][column] % 10);
            if (typ != ITEM_TYPE_NONE) {
              for (int i = 0; i < numberOfItems; i++) {
                if (itemImage[i] == NULL) {
                  itemImage[i] = itemBitmap[typ - 1];
                  itemType[i] = typ;
                  itemPosition[i].x = brickPosition[row][column].x + ((brickSize.x - itemSize.x) * 0.5f);
                  itemPosition[i].y = brickPosition[row][column].y + ((brickSize.y - itemSize.y) * 0.5f);
                  itemCount++;
                  break;
                }
              }
            }
          }
        }
      }
    }
  }

  return isCollide;
}

void changeGameMode(int mode) {
  switch (mode) {
    case GAME_MODE_TITLE:
      resetGame();
      setHighScoreImages();
      break;

    case GAME_MODE_TITLE_BLINK:
      titleBlinkTime = 0.0f;
      break;

    case GAME_MODE_GET_READY:
      if (gameMode == GAME_MODE_TITLE_BLINK) {
        loadLevel();
      }

      isReadyToPlay = false;
      setScoreImages();
      setBallImages();
      getReadyTime = 0.0f;
      missTime = 0.0f;
      ballPosition[mainBall][0] = paddlePosition.x - (ballSize.x * 0.5f);
      ballPosition[mainBall][1] = paddlePosition.y - ballSize.y - 1;
      ballDirection[mainBall][0] = 1;
      ballDirection[mainBall][1] = -1;
      memset(&isBallCollide, false, sizeof(isBallCollide));
      break;

    case GAME_MODE_PLAY:
      playSound(SOUND_LAUNCH_BALL);
      getReadyTime = maxGetReadyTime;
      break;

    case GAME_MODE_MISS:
      miss = true;
      missTime = 0.0f;
      playSound(SOUND_MISS);
      checkHighScore();
      break;

    case GAME_MODE_LEVEL_CLEARED:
      levelClearedTime = 0.0f;
      memset(bulletImage, NULL, sizeof(bulletImage));
      memset(&isBulletCollide, false, sizeof(isBulletCollide));
      memset(itemImage, NULL, sizeof(itemImage));
      memset(&isItemCollide, false, sizeof(isItemCollide));
      checkHighScore();
      break;

    case GAME_MODE_GAME_OVER:
      gameOverTimeOut = 0.0f;
      pressedButton = 0;
      addBallBlinkTime = 0.0f;
      break;
  }

  gameMode = mode;
}

void checkHighScore() {
  if (score > highScore) {
    highScore = score;
    writeHighScores();
  }
}

void addScore(int value) {
  score = constrain(score + value, 0, 9999);
  setScoreImages();
}

void addBall(int value) {
  ballLeft = constrain(ballLeft + value, 0, 9);
  setBallImages();
}

void deleteItem(int i) {
  isItemCollide[i] = false;
  itemImage[i] = NULL;

  if (--itemCount < 0) {
    itemCount = 0;
  }
}

void initGame() {
  readHighScores();
  resetGame();
  changeGameMode(GAME_MODE_TITLE);
  lastFrameTime = millis();
  fpsLastFrameTime = lastFrameTime;
}

void resetGame() {
  titleBlinkTime = 0.0f;
  getReadyTime = 0.0f;
  missTime = 0.0f;
  levelClearedTime = 0.0f;
  gameOverTimeOut = 0.0f;
  paddleType = 0.0f;
  paddleExpandTimeOut = 0.0f;
  isPaddleExpand = false;
  isUpdatePaddlePosition = false;
  paddlePosition.x = 128.0f * 0.5f;
  paddlePosition.y = (float)(screenRect[3] - paddleSize[(int)paddleType].y) + 1.0f;
  ballLeft = newGameBallLeft;
  setBallImages();
  level = 0;
  setLevelImages();
  score = 0;
  setScoreImages();
  setHighScoreImages();
  buttonDelay = 0.0f;
  pressedButton = 0;
  isBulletEnabled = false;
  miss = true;
  bulletTimeOut = maxBulletTimeOut;

  for (int ball = 0; ball < maxBall; ball++) {
    ballSpeed[ball] = defaultBallSpeed;
    ballPosition[ball][0] = paddlePosition.x - (ballSize.x * 0.5f);
    ballPosition[ball][1] = paddlePosition.y - ballSize.y - 1;
    ballDirection[ball][0] = (ball == mainBall) ? 1 : 0;
    ballDirection[ball][1] = (ball == mainBall) ? -1 : 0;
  }

  memset(&isBallCollide, false, sizeof(isBallCollide));
  memset(&ballHitCount, 0, sizeof(ballHitCount));
  memset(&isButtonPressed, false, sizeof(isButtonPressed));
  memset(bulletImage, NULL, sizeof(bulletImage));
  memset(&bulletPosition, 0.0f, sizeof(bulletPosition));
  memset(&isBulletCollide, false, sizeof(isBulletCollide));
  memset(itemImage, NULL, sizeof(itemImage));
  memset(&isItemCollide, false, sizeof(isItemCollide));
}

void loadLevel() {
  numberOfBalls = 1;
  brickCount = 0;
  itemCount = 0;
  setLevelImages();
  rowCount = 0;
  isUpdateRowCounter = false;
  addBallBlinkTime = 0.0f;

  memcpy(&currentLevelTable, levelTable[level], sizeof(currentLevelTable));
  memset(&ballHitCount, 0, sizeof(ballHitCount));
  memset(&isBallCollide, false, sizeof(isBallCollide));
  memset(&itemType, ITEM_TYPE_NONE, sizeof(itemType));
  memset(bulletImage, NULL, sizeof(bulletImage));
  memset(&isBulletCollide, false, sizeof(isBulletCollide));
  memset(itemImage, NULL, sizeof(itemImage));
  memset(&isItemCollide, false, sizeof(isItemCollide));
  memset(&rowFirstHitSpeed, 0.0f, sizeof(rowFirstHitSpeed));
  memset(&ballHitRowFirstTimeFlag, 0, sizeof(ballHitRowFirstTimeFlag));

  int count = 0;
  int brickInRowCount[numberOfRows] = {0};

  for (int row = 0; row < numberOfRows; row++) {
    for (int column = 0; column < numberOfColumns; column++) {
      brickImage[row][column] = NULL;

      if (currentLevelTable[row][column] > 0) {
        brickCount++;
        brickInRowCount[row]++;
        int brickType = (currentLevelTable[row][column] / 10) - 1;
        int typ = (currentLevelTable[row][column] % 10);
        brickImage[row][column] = brickBitmap[brickType + (typ == 0 ? 0 : numberOfBrickType)];
        brickPosition[row][column].x = brickStartPosition.x + (column * (brickSize.x + brickGap.x));
        brickPosition[row][column].y = brickStartPosition.y + (row * (brickSize.y + brickGap.y));
        brickPower[row][column] = defaultBrickPower[brickType];
      }
    }

    if (brickInRowCount[row] > 0) {
      count++;
    }
  }

  for (int row = 0; row < numberOfRows; row++) {
    if (brickInRowCount[row] > 0) {
      brickScore[row] = count--;
      rowFirstHitSpeed[row] = count * 0.002f;
    }
  }

  for (int ball = 0; ball < maxBall; ball++) {
    ballSpeed[ball] = defaultBallSpeed;
  }
}
