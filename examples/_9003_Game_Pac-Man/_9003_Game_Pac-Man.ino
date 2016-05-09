// Pac-Man Simulator for ESPresso Lite

#include <ESPert.h>
ESPert espert;

// OLED bitmaps - Image to Adafruit OLED Bitmap Converter http://www.majer.ch/lcd/adf_bitmap.php
const uint8_t titleBitmap[] PROGMEM { // title.png
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xE7, 0xFB, 0x1D,
  0x0D, 0xE6, 0x26, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6,
  0xA6, 0xE6, 0x86, 0x06, 0x06, 0x06, 0x06, 0x06, 0xC6, 0x26,
  0xC6, 0x46, 0x86, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
  0x86, 0x86, 0x46, 0x46, 0xC6, 0xC6, 0xC6, 0x86, 0x86, 0x86,
  0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
  0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0xE6, 0x46, 0x86,
  0xE6, 0xC6, 0x86, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x86,
  0x46, 0xA6, 0xC6, 0x86, 0x06, 0x06, 0x06, 0x06, 0xC6, 0x26,
  0xC6, 0x46, 0x86, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0xE6,
  0x46, 0xE6, 0xC6, 0x86, 0x06, 0xE6, 0x26, 0xA6, 0xA6, 0xA6,
  0xA6, 0xA6, 0xE6, 0x86, 0x06, 0x06, 0x0D, 0x1D, 0xFB, 0xE7,
  0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
  0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
  0x80, 0x61, 0x02, 0xFC, 0xFC, 0x33, 0x0C, 0x03, 0xC0, 0xC0,
  0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0xF8, 0x06, 0xF1, 0x0C, 0x02,
  0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xB1, 0x19,
  0x0E, 0x04, 0x00, 0x00, 0x3C, 0xF4, 0x94, 0x94, 0x94, 0x94,
  0x94, 0x9C, 0xF0, 0xE0, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x01,
  0x03, 0x07, 0x0E, 0x1C, 0x1C, 0x0A, 0x05, 0x02, 0x01, 0x00,
  0xFF, 0xFF, 0xC0, 0x30, 0xCC, 0x33, 0x0C, 0x03, 0xC0, 0xC0,
  0x03, 0x0F, 0x3C, 0xF0, 0xC0, 0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x01, 0x03, 0x07, 0x0F, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1F, 0x10, 0x7F,
  0xC0, 0xD0, 0xD0, 0xD0, 0xDF, 0xC1, 0xFD, 0xFD, 0x2C, 0x7E,
  0xCF, 0xC3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xD0,
  0xD4, 0xD8, 0xD3, 0xCF, 0xFC, 0xF3, 0xEC, 0x50, 0x60, 0x60,
  0x40, 0xC0, 0xD0, 0xD2, 0xD4, 0xC9, 0xE2, 0x65, 0x7A, 0x3C,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x00, 0x3F, 0x20, 0x7F, 0xC0, 0xC0, 0xC0, 0xC0,
  0xC0, 0xC0, 0xC0, 0xD0, 0xD0, 0xD0, 0xDE, 0xC0, 0xFF, 0xFF,
  0xCC, 0xC3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xD0,
  0xD4, 0xD8, 0xD3, 0xCF, 0xFC, 0xFF, 0xF0, 0xFF, 0xC0, 0xC0,
  0xC0, 0xC0, 0xC0, 0xC0, 0xD0, 0xD0, 0xD0, 0xDE, 0xC0, 0xFF,
  0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFC, 0xFB, 0xF7, 0xF6, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
  0xF6, 0xF7, 0xFB, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x81, 0xA5,
  0x18, 0x18, 0xA5, 0x81, 0xC3, 0xFF, 0xFF, 0x83, 0xFB, 0xC7,
  0xFB, 0x83, 0xFF, 0x83, 0xEB, 0x83, 0xFF, 0x83, 0xEF, 0x93,
  0xFF, 0x83, 0xAB, 0xAB, 0xFF, 0x83, 0xEB, 0x93, 0xFF, 0x83,
  0xEB, 0x83, 0xFF, 0xA3, 0xAB, 0x8B, 0xFF, 0x83, 0xFF, 0x83,
  0xEB, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t characterNicknameBitmap[] PROGMEM { // characterNickname.png
  0xF1, 0xEE, 0xEE, 0xEE, 0xF5, 0xFF, 0xE0, 0xFB, 0xFB, 0xE0,
  0xFF, 0xE1, 0xFA, 0xFA, 0xE1, 0xFF, 0xE0, 0xFA, 0xF2, 0xED,
  0xFF, 0xE1, 0xFA, 0xFA, 0xE1, 0xFF, 0xF1, 0xEE, 0xEE, 0xEE,
  0xF5, 0xFF, 0xFE, 0xE0, 0xFE, 0xFF, 0xE0, 0xEA, 0xEA, 0xEE,
  0xFF, 0xE0, 0xFA, 0xF2, 0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xE0, 0xF9, 0xF3, 0xE0, 0xFF, 0xEE, 0xE0,
  0xEE, 0xFF, 0xF1, 0xEE, 0xEE, 0xEE, 0xF5, 0xFF, 0xE0, 0xFB,
  0xF1, 0xE4, 0xFF, 0xE0, 0xF9, 0xF3, 0xE0, 0xFF, 0xE1, 0xFA,
  0xFA, 0xE1, 0xFF, 0xE0, 0xFD, 0xFB, 0xFD, 0xE0, 0xFF, 0xE0,
  0xEA, 0xEA, 0xEE, 0xFF
};

static const int numberOfGhosts = 4;
const uint8_t characterBitmap[numberOfGhosts][40 * 8 / 8] PROGMEM {
  {0xFB, 0xFB, 0xFB, 0xFF, 0xFF, 0xED, 0xEA, 0xEA, 0xF6, 0xFF, 0xE0, 0xFB, 0xFB, 0xE0, 0xFF, 0xE1, 0xFA, 0xFA, 0xE1, 0xFF, 0xE0, 0xEE, 0xEE, 0xF1, 0xFF, 0xF1, 0xEE, 0xEE, 0xEE, 0xF1, 0xFF, 0xE0, 0xF7, 0xFB, 0xF7, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF}, // shadow.png
  {0xFB, 0xFB, 0xFB, 0xFF, 0xFF, 0xED, 0xEA, 0xEA, 0xF6, 0xFF, 0xE0, 0xFA, 0xFA, 0xFD, 0xFF, 0xE0, 0xEA, 0xEA, 0xEE, 0xFF, 0xE0, 0xEA, 0xEA, 0xEE, 0xFF, 0xE0, 0xEE, 0xEE, 0xF1, 0xFF, 0xFC, 0xFB, 0xE3, 0xFB, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // speedy.png
  {0xFB, 0xFB, 0xFB, 0xFF, 0xFF, 0xE0, 0xEA, 0xEA, 0xF5, 0xFF, 0xE1, 0xFA, 0xFA, 0xE1, 0xFF, 0xED, 0xEA, 0xEA, 0xF6, 0xFF, 0xE0, 0xFB, 0xFB, 0xE0, 0xFF, 0xE0, 0xFA, 0xFA, 0xFE, 0xFF, 0xF0, 0xEF, 0xEF, 0xEF, 0xF0, 0xFF, 0xE0, 0xEF, 0xEF, 0xEF}, // bashful.png
  {0xFB, 0xFB, 0xFB, 0xFF, 0xFF, 0xE0, 0xFA, 0xFA, 0xFD, 0xFF, 0xF1, 0xEE, 0xEE, 0xEE, 0xF1, 0xFF, 0xE0, 0xFB, 0xF1, 0xE4, 0xFF, 0xE0, 0xEA, 0xEA, 0xEE, 0xFF, 0xFC, 0xFB, 0xE3, 0xFB, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pokey.png
};

const uint8_t nicknameBitmap[numberOfGhosts][40 * 8 / 8] PROGMEM {
  {0xFC, 0xFF, 0xFC, 0xFF, 0xE0, 0xEA, 0xEA, 0xF5, 0xFF, 0xE0, 0xEF, 0xEF, 0xEF, 0xFF, 0xEE, 0xE0, 0xEE, 0xFF, 0xE0, 0xF9, 0xF3, 0xE0, 0xFF, 0xE0, 0xFB, 0xF1, 0xE4, 0xFF, 0xFC, 0xFB, 0xE3, 0xFB, 0xFC, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // blinky.png
  {0xFC, 0xFF, 0xFC, 0xFF, 0xE0, 0xFA, 0xFA, 0xFD, 0xFF, 0xEE, 0xE0, 0xEE, 0xFF, 0xE0, 0xF9, 0xF3, 0xE0, 0xFF, 0xE0, 0xFB, 0xF1, 0xE4, 0xFF, 0xFC, 0xFB, 0xE3, 0xFB, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pinky.png
  {0xFC, 0xFF, 0xFC, 0xFF, 0xEE, 0xE0, 0xEE, 0xFF, 0xE0, 0xF9, 0xF3, 0xE0, 0xFF, 0xE0, 0xFB, 0xF1, 0xE4, 0xFF, 0xFC, 0xFB, 0xE3, 0xFB, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // inky.png
  {0xFC, 0xFF, 0xFC, 0xFF, 0xF1, 0xEE, 0xEE, 0xEE, 0xF5, 0xFF, 0xE0, 0xEF, 0xEF, 0xEF, 0xFF, 0xFC, 0xFB, 0xE3, 0xFB, 0xFC, 0xFF, 0xE0, 0xEE, 0xEE, 0xF1, 0xFF, 0xE0, 0xEA, 0xEA, 0xEE, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // clyde.png
};

const uint8_t ptsBitmap[] PROGMEM { // pts.png
  0xFF, 0xF3, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xED, 0xE0, 0xEF, 0xFF, 0xF1, 0xEE, 0xEE, 0xEE, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xFA, 0xFA, 0xFD, 0xFF, 0xFE, 0xE0, 0xFE, 0xFF, 0xED, 0xEA, 0xEA, 0xF6, 0xFF, 0xFF,
  0xF9, 0xF0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE8, 0xEA, 0xEA, 0xF6, 0xFF, 0xF1, 0xEE, 0xEE, 0xEE, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xFA, 0xFA, 0xFD, 0xFF, 0xFE, 0xE0, 0xFE, 0xFF, 0xED, 0xEA, 0xEA, 0xF6, 0xFF, 0xFF
};

const uint8_t numberBitmap[10][8] PROGMEM {
  {0xFF, 0xC1, 0xDD, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF}, // number0.png
  {0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF}, // number1.png
  {0xFF, 0xC5, 0xD5, 0xD1, 0xFF, 0xFF, 0xFF, 0xFF}, // number2.png
  {0xFF, 0xD5, 0xD5, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF}, // number3.png
  {0xFF, 0xF1, 0xF7, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF}, // number4.png
  {0xFF, 0xD1, 0xD5, 0xC5, 0xFF, 0xFF, 0xFF, 0xFF}, // number5.png
  {0xFF, 0xC1, 0xD5, 0xC5, 0xFF, 0xFF, 0xFF, 0xFF}, // number6.png
  {0xFF, 0xFD, 0xFD, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF}, // number7.png
  {0xFF, 0xC1, 0xD5, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF}, // number8.png
  {0xFF, 0xD1, 0xD5, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF}  // number9.png
};

const uint8_t numberMaskBitmap[] PROGMEM { // numberMask.png
  0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0xFF
};

static const byte numberOfDieFrames = 14;
const uint8_t dieBitmap[numberOfDieFrames][16 * 16 / 8] PROGMEM {
  {0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die00.png
  {0xE3, 0xC3, 0x83, 0x87, 0x8F, 0x87, 0x83, 0xC3, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die01.png
  {0xC3, 0x83, 0x87, 0x87, 0x8F, 0x87, 0x87, 0x83, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die02.png
  {0xC7, 0x87, 0x87, 0x8F, 0x8F, 0x8F, 0x87, 0x87, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die03.png
  {0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die04.png
  {0x9F, 0x9F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die05.png
  {0xBF, 0x1F, 0x1F, 0x0F, 0x8F, 0x0F, 0x1F, 0x1F, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die06.png
  {0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die07.png
  {0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die08.png
  {0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die09.png
  {0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die10.png
  {0xBB, 0xD7, 0xFE, 0x7D, 0xFF, 0x7D, 0xFE, 0xD7, 0xBB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die11.png
  {0xBB, 0xD7, 0xFE, 0x7D, 0xFF, 0x7D, 0xFE, 0xD7, 0xBB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // die11.png
  {0xBB, 0xD7, 0xFE, 0x7D, 0xFF, 0x7D, 0xFE, 0xD7, 0xBB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // die11.png
};

static const byte numberOfDirections = 4;
static const byte numberOfPacManFrames = 4;
const uint8_t pacManBitmap[numberOfDirections][numberOfPacManFrames][16 * 16 / 8] PROGMEM {
  {
    {0xFF, 0x77, 0x73, 0x71, 0x21, 0x21, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManLeft0.png
    {0xFF, 0xFF, 0xFF, 0xF9, 0x71, 0x21, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManLeft1.png
    {0xFF, 0x77, 0x73, 0x71, 0x21, 0x21, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManLeft2.png
    {0xFF, 0x07, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManLeft3.png
  },
  {
    {0xFF, 0x07, 0x03, 0x01, 0x01, 0x21, 0x21, 0x71, 0x73, 0x77, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManRight0.png
    {0xFF, 0x07, 0x03, 0x01, 0x01, 0x21, 0x71, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManRight1.png
    {0xFF, 0x07, 0x03, 0x01, 0x01, 0x21, 0x21, 0x71, 0x73, 0x77, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManRight2.png
    {0xFF, 0x07, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManRight3.png
  },
  {
    {0xFF, 0x07, 0x03, 0x01, 0x0F, 0x3F, 0x0F, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManUp0.png
    {0xFF, 0x07, 0x07, 0x0F, 0x1F, 0x3F, 0x1F, 0x0F, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManUp1.png
    {0xFF, 0x07, 0x03, 0x01, 0x0F, 0x3F, 0x0F, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManUp2.png
    {0xFF, 0x07, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManUp3.png
  },
  {
    {0xFF, 0x07, 0x03, 0x01, 0x81, 0xE1, 0x81, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFF, 0xFF, 0xFF, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManDown0.png
    {0xFF, 0x07, 0x03, 0x81, 0xC1, 0xE1, 0xC1, 0x81, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManDown1.png
    {0xFF, 0x07, 0x03, 0x01, 0x81, 0xE1, 0x81, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFF, 0xFF, 0xFF, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManDown2.png
    {0xFF, 0x07, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManDown3.png
  }
};

const uint8_t pacManMaskBitmap[numberOfDirections][numberOfPacManFrames][16 * 16 / 8] PROGMEM {
  {
    {0x77, 0x23, 0x21, 0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManLeftMask0.png
    {0xFF, 0xFF, 0xF9, 0x70, 0x20, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManLeftMask1.png
    {0x77, 0x23, 0x21, 0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManLeftMask2.png
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManLeftMask3.png
  },
  {
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0x21, 0x23, 0x77, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManRightMask0.png
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x20, 0x70, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManRightMask1.png
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0x21, 0x23, 0x77, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManRightMask2.png
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManRightMask3.png
  },
  {
    {0x07, 0x03, 0x01, 0x00, 0x01, 0x0F, 0x01, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManUpMask0.png
    {0x07, 0x03, 0x03, 0x07, 0x0F, 0x1F, 0x0F, 0x07, 0x03, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManUpMask1.png
    {0x07, 0x03, 0x01, 0x00, 0x01, 0x0F, 0x01, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManUpMask2.png
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManUpMask3.png
  },
  {
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xFC, 0xFF, 0xFC, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManDownMask0.png
    {0x07, 0x03, 0x01, 0x00, 0x80, 0xC0, 0x80, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManDownMask1.png
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xFC, 0xFF, 0xFC, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // pacManDownMask2.png
    {0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // pacManDownMask3.png
  }
};

static const byte numberOfGhostFrames = 2;
const uint8_t ghostBitmap[numberOfGhostFrames][16 * 16 / 8] PROGMEM {
  {0xFF, 0x0F, 0x07, 0x03, 0x01, 0x01, 0x01, 0x03, 0x07, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFE, 0xFC, 0xFC, 0xFE, 0xFC, 0xFC, 0xFE, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghost0.png
  {0xFF, 0x0F, 0x07, 0x03, 0x01, 0x01, 0x01, 0x03, 0x07, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFE, 0xFC, 0xFC, 0xFC, 0xFE, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // ghost1.png
};

const uint8_t ghostMaskBitmap[numberOfGhostFrames][16 * 16 / 8] PROGMEM {
  {0x07, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xFC, 0xF8, 0xF8, 0xFC, 0xF8, 0xF8, 0xFC, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostMask0.png
  {0x07, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xF8, 0xFC, 0xF8, 0xF8, 0xF8, 0xFC, 0xF8, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // ghostMask1.png
};

const uint8_t ghostEyeBitmap[numberOfDirections][16 * 16 / 8] PROGMEM {
  {0xFF, 0xD7, 0xC7, 0xC7, 0xFF, 0xD7, 0xC7, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostEyeLeft.png
  {0xFF, 0xFF, 0xFF, 0xC7, 0xC7, 0xD7, 0xFF, 0xC7, 0xC7, 0xD7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostEyeRight.png
  {0xFF, 0xFF, 0xC7, 0xCF, 0xC7, 0xFF, 0xC7, 0xCF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostEyeUp.png
  {0xFF, 0xFF, 0xC7, 0xE7, 0xC7, 0xFF, 0xC7, 0xE7, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // ghostEyeDown.png
};

const uint8_t ghostEyeMaskBitmap[numberOfDirections][16 * 16 / 8] PROGMEM {
  {0xC7, 0x83, 0x83, 0x83, 0xC7, 0x83, 0x83, 0x83, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostEyeLeftMask.png
  {0xFF, 0xFF, 0xC7, 0x83, 0x83, 0x83, 0xC7, 0x83, 0x83, 0x83, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostEyeRightMask.png
  {0xFF, 0xC7, 0x83, 0x83, 0x83, 0xC7, 0x83, 0x83, 0x83, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostEyeUpMask.png
  {0xFF, 0xC7, 0x83, 0x83, 0x83, 0xC7, 0x83, 0x83, 0x83, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // ghostEyeDownMask.png
};

const uint8_t ghostEdible[numberOfGhostFrames][16 * 16 / 8] PROGMEM {
  {0x07, 0xFB, 0xFD, 0x65, 0xA6, 0x7E, 0xA6, 0x65, 0xFD, 0xFB, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFB, 0xFD, 0xFB, 0xFB, 0xFD, 0xFB, 0xFB, 0xFD, 0xFB, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // ghostEdible0.png
  {0x07, 0xFB, 0xFD, 0x65, 0xA6, 0x7E, 0xA6, 0x65, 0xFD, 0xFB, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFB, 0xFB, 0xFD, 0xFB, 0xFB, 0xFB, 0xFD, 0xFB, 0xFB, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // ghostEdible1.png
};

static const byte numberOfTiles = 24;
const uint8_t tileBitmap[numberOfTiles][8] PROGMEM {
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // tile00.png
  {0xFF, 0xFF, 0xF3, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF}, // tile01.png
  {0xFF, 0xF3, 0xE1, 0xE1, 0xF3, 0xFF, 0xFF, 0xFF}, // tile02.png
  {0xFF, 0xE7, 0xFB, 0xED, 0xF5, 0xF5, 0xFF, 0xFF}, // tile03.png
  {0xF5, 0xF5, 0xED, 0xFB, 0xE7, 0xFF, 0xFF, 0xFF}, // tile04.png
  {0xF5, 0xF5, 0xF6, 0xFB, 0xFC, 0xFF, 0xFF, 0xFF}, // tile05.png
  {0xFF, 0xFC, 0xFB, 0xF6, 0xF5, 0xF5, 0xFF, 0xFF}, // tile06.png
  {0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xFF, 0xFF}, // tile07.png
  {0xFF, 0xE0, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF}, // tile08.png
  {0xFF, 0xFF, 0xE0, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF}, // tile09.png
  {0xF5, 0xED, 0xFD, 0xED, 0xF5, 0xF5, 0xFF, 0xFF}, // tile10.png
  {0xF5, 0xF5, 0xEE, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF}, // tile11.png
  {0xF5, 0xF6, 0xF7, 0xF6, 0xF5, 0xF5, 0xFF, 0xFF}, // tile12.png
  {0xF5, 0xF5, 0xF6, 0xF7, 0xF6, 0xF5, 0xFF, 0xFF}, // tile13.png
  {0xFF, 0xE0, 0xFF, 0xEE, 0xF5, 0xF5, 0xFF, 0xFF}, // tile14.png
  {0xFF, 0xF8, 0xF7, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF}, // tile15.png
  {0xFF, 0xFF, 0xF8, 0xF7, 0xF8, 0xFF, 0xFF, 0xFF}, // tile16.png
  {0xFF, 0xFB, 0xF5, 0xF5, 0xF5, 0xF5, 0xFF, 0xFF}, // tile17.png
  {0xFF, 0xE3, 0xFD, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF}, // tile18.png
  {0xFF, 0xFF, 0xE3, 0xFD, 0xE3, 0xFF, 0xFF, 0xFF}, // tile19.png
  {0xF5, 0xF5, 0xF5, 0xF5, 0xFB, 0xFF, 0xFF, 0xFF}, // tile20.png
  {0xF5, 0xF5, 0xF5, 0xFB, 0xFB, 0xFB, 0xFF, 0xFF}, // tile21.png
  {0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFF, 0xFF}, // tile22.png
  {0xFB, 0xFB, 0xFB, 0xF5, 0xF5, 0xF5, 0xFF, 0xFF}  // tile23.png
};

const uint8_t fpsBitmap[] PROGMEM { // fps.png
  0xFF, 0xC1, 0xF5, 0xFD, 0xFF, 0xC1, 0xF5, 0xF1, 0xFF, 0xD1, 0xD5, 0xC5, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t fpsMaskBitmap[] PROGMEM { // fpsMask.png
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF
};

const uint8_t lifeBitmap[] PROGMEM { // life.png
  0xFF, 0xEB, 0xC9, 0xC1, 0xC1, 0xE3, 0xFF, 0xFF
};

const uint8_t lifeMaskBitmap[] PROGMEM { // lifeMask.png
  0xC1, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC1, 0xFF
};

const uint8_t readyBitmap[] PROGMEM { // ready.png
  0xFF, 0x81, 0x81, 0xED, 0x8D, 0x91, 0xB3, 0xFF, 0x81, 0x81, 0xB5, 0xB5, 0xB5, 0xFF, 0x87, 0x83, 0xE9, 0xED, 0x81, 0x83, 0xFF, 0x81, 0x81, 0xBD, 0x99, 0x81, 0xE3, 0xFF, 0xFF, 0xF1, 0x87, 0x87, 0xF1, 0xF1, 0xFF, 0xFF, 0xA1, 0xA1, 0xF1, 0xFF
};

const uint8_t readyMaskBitmap[] PROGMEM { // readyMask.png
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x70, 0x00, 0x00, 0x00, 0x00, 0x60, 0x70, 0x20, 0x00, 0x00, 0x20, 0x00
};

const uint8_t gameOverBitmap[] PROGMEM { // gameOver.png
  0xFF, 0xE7, 0xC3, 0x99, 0xBD, 0x8D, 0x8D, 0xFF, 0x87, 0x83, 0xE9, 0xED, 0x81, 0x83, 0xFF, 0x81, 0x81, 0xF3, 0xE7, 0xF3, 0x81, 0x81, 0xFF, 0x81, 0x81, 0xB5, 0xB5, 0xB5, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x81, 0xBD, 0xBD, 0x81, 0xC3, 0xFF, 0xC1, 0x81, 0xBF, 0x9F, 0xC1, 0xE1, 0xFF, 0x81, 0x81, 0xB5, 0xB5, 0xB5, 0xFF, 0x81, 0x81, 0xED, 0x8D, 0x91, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t gameOverMaskBitmap[] PROGMEM { // gameOverMask.png
  0xC3, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t highScoreBitmap[] PROGMEM { // highScore.png
  0xFF, 0xC1, 0xF7, 0xC1, 0xFF, 0xC1, 0xFF, 0xF7, 0xFF, 0xD1, 0xD5, 0xC5, 0xFF, 0xC1, 0xDD, 0xDD, 0xFF, 0xC1, 0xDD, 0xC1, 0xFF, 0xC1, 0xF5, 0xC9, 0xFF, 0xC1, 0xD5, 0xD5, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t highScoreMaskBitmap[] PROGMEM { // highScoreMask.png
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF
};

static const int numberOfItems = 8;
const uint8_t itemBitmap[numberOfItems][16 * 16 / 8] PROGMEM {
  {0xFF, 0x9F, 0x4F, 0x0F, 0x97, 0x7B, 0x3B, 0x03, 0x79, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFD, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemCherry.png
  {0xFF, 0xCF, 0x07, 0xAB, 0x0B, 0x11, 0x4B, 0x0B, 0x27, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFD, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemStrawberry.png
  {0xFF, 0x1F, 0x0F, 0x07, 0x07, 0x0B, 0x01, 0x09, 0x0D, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemOrange.png
  {0xFF, 0x0F, 0x65, 0x15, 0x09, 0x13, 0x09, 0x07, 0x07, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFE, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemApple.png
  {0xFF, 0xFF, 0x0F, 0x95, 0xD5, 0x05, 0x81, 0xB5, 0x37, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFD, 0xFD, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemMelon.png
  {0xFF, 0xE1, 0xCF, 0x17, 0xA3, 0x71, 0xA3, 0x17, 0xCF, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemGalaxian.png
  {0xFF, 0x3F, 0x0F, 0x47, 0x33, 0x09, 0x03, 0x07, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemBell.png
  {0xFF, 0xFF, 0xFF, 0xF3, 0xE1, 0x05, 0x01, 0xA1, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // itemKey.png
};

const uint8_t itemMaskBitmap[numberOfItems][16 * 16 / 8] PROGMEM {
  {0x9F, 0x0F, 0x07, 0x07, 0x03, 0x01, 0x01, 0x01, 0x00, 0x70, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFC, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemCherryMask.png
  {0xCF, 0x07, 0x03, 0x01, 0x01, 0x00, 0x01, 0x01, 0x03, 0x07, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemStrawberryMask.png
  {0x1F, 0x0F, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x05, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemOrangeMask.png
  {0x0F, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x03, 0x07, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xFC, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemAppleMask.png
  {0xFF, 0x0F, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemMelonMask.png
  {0xE1, 0xC0, 0x01, 0x03, 0x01, 0x00, 0x01, 0x03, 0x01, 0xC0, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemGalaxianMask.png
  {0x3F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // itemBellMask.png
  {0xFF, 0xFF, 0xF3, 0xE1, 0x00, 0x00, 0x00, 0x00, 0xA1, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // itemKeyMask.png
};

static const byte worldHeight = 27;
static const byte worldWidth = 27;
static const byte numberOfRows = 27;
static const byte numberOfColumns = 21;
static const byte initialWorld[worldHeight][worldWidth] {
  //0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
  { 3,  7,  7,  7,  7,  7,  7,  7,  7,  7, 10,  7,  7,  7,  7,  7,  7,  7,  7,  7,  4,  0,  0,  0,  0,  0,  0}, //  0
  { 8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  9,  0,  0,  0,  0,  0,  0}, //  1
  { 8,  1,  3,  7,  4,  1,  3,  7,  4,  1,  8,  1,  3,  7,  4,  1,  3,  7,  4,  1,  9,  0,  0,  0,  0,  0,  0}, //  2
  { 8,  2,  8,  0,  9,  1,  8,  0,  9,  1,  8,  1,  8,  0,  9,  1,  8,  0,  9,  2,  9,  0,  0,  0,  0,  0,  0}, //  3
  { 8,  1,  6,  7,  5,  1,  6,  7,  5,  1, 15,  1,  6,  7,  5,  1,  6,  7,  5,  1,  9,  0,  0,  0,  0,  0,  0}, //  4
  { 8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  9,  0,  0,  0,  0,  0,  0}, //  5
  { 8,  1,  3,  7,  4,  1, 18,  1,  3,  7,  7,  7,  4,  1, 19,  1,  3,  7,  4,  1,  9,  0,  0,  0,  0,  0,  0}, //  6
  { 8,  1,  6,  7,  5,  1,  8,  1,  6,  7, 10,  7,  5,  1,  9,  1,  6,  7,  5,  1,  9,  0,  0,  0,  0,  0,  0}, //  7
  { 8,  1,  1,  1,  1,  1,  8,  1,  1,  1,  8,  1,  1,  1,  9,  1,  1,  1,  1,  1,  9,  0,  0,  0,  0,  0,  0}, //  8
  { 6,  7,  7,  7,  4,  1, 14,  7, 20,  0, 15,  0, 17,  7, 11,  1,  3,  7,  7,  7,  5,  0,  0,  0,  0,  0,  0}, //  9
  { 0,  0,  0,  0,  9,  1,  8,  0,  0,  0,  0,  0,  0,  0,  9,  1,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 10
  { 0,  0,  0,  0,  9,  1,  8,  0,  3, 21, 22, 23,  4,  0,  9,  1,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 11
  {17,  7,  7,  7,  5,  1, 15,  0,  8,  0,  0,  0,  9,  0, 16,  1,  6,  7,  7,  7, 20,  0,  0,  0,  0,  0,  0}, // 12
  { 0,  0,  0,  0,  0,  1,  0,  0,  8,  0,  0,  0,  9,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 13
  {17,  7,  7,  7,  4,  1, 18,  0,  6,  7,  7,  7,  5,  0, 19,  1,  3,  7,  7,  7, 20,  0,  0,  0,  0,  0,  0}, // 14
  { 0,  0,  0,  0,  9,  1,  8,  0,  0,  0,  0,  0,  0,  0,  9,  1,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 15
  { 0,  0,  0,  0,  9,  1,  8,  0,  3,  7,  7,  7,  4,  0,  9,  1,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 16
  { 3,  7,  7,  7,  5,  1, 15,  0,  6,  7, 10,  7,  5,  0, 16,  1,  6,  7,  7,  7,  4,  0,  0,  0,  0,  0,  0}, // 17
  { 8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  9,  0,  0,  0,  0,  0,  0}, // 18
  { 8,  1, 17,  7,  4,  1, 17,  7, 20,  1, 15,  1, 17,  7, 20,  1,  3,  7, 20,  1,  9,  0,  0,  0,  0,  0,  0}, // 19
  { 8,  2,  1,  1,  9,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  8,  1,  1,  2,  9,  0,  0,  0,  0,  0,  0}, // 20
  {14,  7,  4,  1,  9,  1, 18,  1,  3,  7,  7,  7,  4,  1, 19,  1,  8,  1,  3,  7, 11,  0,  0,  0,  0,  0,  0}, // 21
  {14,  7,  5,  1, 16,  1,  8,  1,  6,  7, 10,  7,  5,  1,  9,  1, 15,  1,  6,  7, 11,  0,  0,  0,  0,  0,  0}, // 22
  { 8,  1,  1,  1,  1,  1,  8,  1,  1,  1,  8,  1,  1,  1,  9,  1,  1,  1,  1,  1,  9,  0,  0,  0,  0,  0,  0}, // 23
  { 8,  1, 17,  7,  7,  7, 12,  7, 20,  1, 15,  1, 17,  7, 13,  7,  7,  7, 20,  1,  9,  0,  0,  0,  0,  0,  0}, // 24
  { 8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  9,  0,  0,  0,  0,  0,  0}, // 25
  { 6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  5,  0,  0,  0,  0,  0,  0}  // 26
};

typedef struct {
  int width;
  int height;
} Size;

typedef struct {
  float x;
  float y;
} Point;

typedef struct {
  int row;
  int column;
} Tile;

enum {
  SOUND_NONE,
  SOUND_EAT_DOT,
  SOUND_EAT_ITEM,
  SOUND_EAT_GHOST,
  SOUND_EAT_POWER_PELLET,
  SOUND_DIE,
  SOUND_DISAPPEAR
};

enum {
  GAME_MODE_TITLE,
  GAME_MODE_READY,
  GAME_MODE_PLAY,
  GAME_MODE_DIE,
  GAME_MODE_SUCCESS,
  GAME_MODE_GAME_OVER
};

enum {
  BUTTON_NONE = -1,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_A,
  BUTTON_B
};

enum {
  TILE_TYPE_SPACE,
  TILE_TYPE_DOT,
  TILE_TYPE_POWER_PELLET,
  TILE_TYPE_DOOR = 22
};

enum {
  DIRECTION_NONE = -1,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
  DIRECTION_UP,
  DIRECTION_DOWN
};

enum {
  GHOST_STATE_WAIT,
  GHOST_STATE_GO,
  GHOST_STATE_RUN,
  GHOST_STATE_DIE
};

enum {
  ITEM_CHERRY,
  ITEM_STRAWBERRY,
  ITEM_ORANGE,
  ITEM_APPLE,
  ITEM_MELON,
  ITEM_GALAXIAN,
  ITEM_BELL,
  ITEM_KEY
};

// button
static const byte numberOfButtons = 6;
int buttonPin[numberOfButtons] = {12, 13, 14, 15, 0, A0}; // (left, right, up, down, a, b)
ESPert_Button button[numberOfButtons];
static const int maxButtonDelay = 10; // milliseconds
float buttonDelay = 0.0f;
bool isButtonPressed[numberOfButtons] = {false};
int pressedButton = BUTTON_NONE;

// sound (buzzer)
static const int buzzerPin = 2;
float buzzerDuration = 0.0f;
bool isSoundEnabled = true;
bool isSoundInterruptEnabled = true;

// game time
unsigned long lastFrameTime = 0l;
float elapsedTime = 0.0f;
unsigned long frameCount = 0l;
unsigned long frameRate = 0l;

// fps
bool isFPSVisibled = false;
unsigned long fpsLastFrameTime = 0l;
const uint8_t* fpsDigitImage[2] = {NULL};
Point fpsPosition = {0.0f, 0.0f};

// game
Size screenSize = {128, 64};
Point screenCenter = {screenSize.width * 0.5f, screenSize.height * 0.5f};
int gameMode = GAME_MODE_TITLE;
int nextSound = SOUND_NONE;
float nextSoundDelay = 0.0f;
static const float maxReadyTime = 5000.0f;
float readyTime = 0.0f;
Point readyPosition = {0.0f, 0.0f};
static const float gameNormalSpeed = 25.0f / 1000.0f;
float gameSpeed = 0.0f;
static const int numberOfLevels = 256;
int level = 0;
float successTime = 0.0f;
float gameOverTime = 0.0f;
Point gameOverPosition = {0.0f, 0.0f};
static const int maxBlinkTime = 500;
float blinkTime = 0.0f;
float titleTime = 0.0f;

// score
Size numberSize = {5, 7};
String eepromKey = "Pac-Man";
static const unsigned long maxScore = 999999l;
int eepromAddress = 512 - eepromKey.length() - String(maxScore).length();
unsigned long highScore = 0l;
unsigned long score = 0l;
bool isAwardExtraLife = false;

// item
Size itemSize = {11, 11};
Tile itemTile = {0, 0};
Point itemPosition = {0.0f, 0.0f};
int itemIndex = 0;
String itemPointString = "";
Size itemPointSize = {0, 0};
Point itemPointPosition = {0.0f, 0.0f};
float itemVisibledTime = 0.0f;
int itemPoint[numberOfItems] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};
static const int maxItems = 13;
int item[maxItems] = {
  ITEM_CHERRY,
  ITEM_STRAWBERRY,
  ITEM_ORANGE,
  ITEM_ORANGE,
  ITEM_APPLE,
  ITEM_APPLE,
  ITEM_MELON,
  ITEM_MELON,
  ITEM_GALAXIAN,
  ITEM_GALAXIAN,
  ITEM_BELL,
  ITEM_BELL,
  ITEM_KEY
};

// world
Size statusBarSize = {128, 0};
Size tileSize = {6, 5};
Size worldPixelSize = {tileSize.width * numberOfColumns, (tileSize.height * numberOfRows) + 1.0f};
Point worldPosition = {0.0f, 0.0f};
byte world[worldHeight][worldWidth] = {0};
float worldStartPosition = 0.0f;
Tile availabledTile = {0, 0};
static const int directionOrderUpperLeft[numberOfDirections] = {DIRECTION_DOWN, DIRECTION_RIGHT, DIRECTION_UP, DIRECTION_LEFT};
static const int directionOrderUpperRight[numberOfDirections] = {DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_UP, DIRECTION_RIGHT};
static const int directionOrderLowerLeft[numberOfDirections] = {DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT};
static const int directionOrderLowerRight[numberOfDirections] = {DIRECTION_UP, DIRECTION_LEFT, DIRECTION_DOWN, DIRECTION_RIGHT};

// pac-man
Size pacManSize = {11, 11};
Point pacManPosition = {0};
int pacManDirection = DIRECTION_LEFT;
int pacManNextDirection = pacManDirection;
static const float pacManAnimationSpeed = 15.0f / 1000.0f;
float pacManFrame = 0.0f;
bool isPacManStoped = false;
Tile pacManTile = {0, 0};
Tile pacManLastTile = {0, 0};
static const byte startPacManLives = 3;
byte numberOfPacManLives = startPacManLives;
bool isPacManVisibled = false;
static const float minPowerPelletTime = 3000.0f;
float maxPowerPelletTime = 10000.0f;
float powerPelletTime = 0.0f;
int dotCounter = 0;
int numberOfDots = 0;
float pacManDieTime = 0.0f;
float pacManDieFrame = 0.0f;
bool isPacManDisappeared = false;
static const float pacManMaxIncreseSpeed = 3.0f;
float pacManIncreseSpeed = 0.0f;
static const float pacManMaxIncreseSpeedTime = 1000.0f;
float pacManIncreseSpeedTime = 0.0f;

// ghost
Size ghostSize = {11, 11};
Point ghostPosition[numberOfGhosts] = {0};
int ghostDirection[numberOfGhosts] = {DIRECTION_LEFT, DIRECTION_UP, DIRECTION_UP, DIRECTION_UP};
int ghostNextDirection[numberOfGhosts] = {DIRECTION_LEFT, DIRECTION_UP, DIRECTION_UP, DIRECTION_UP};
static const float ghostAnimationSpeed = 9.0f / 1000.0f;
float ghostFrame[numberOfGhosts] = {0.0f};
bool isGhostStoped[numberOfGhosts] = {false};
Tile ghostTile[numberOfGhosts] = {{10, 10}, {13, 9}, {13, 11}, {13, 10}};
Tile ghostLastTile[numberOfGhosts] = {{10, 10}, {13, 9}, {13, 11}, {13, 10}};
bool isGhostVisibled[numberOfGhosts] = {false};
float ghostWaitTime[numberOfGhosts] = {0.0f};
static const float maxGhostRandomTime = 4000.0f;
float ghostRandomTime[numberOfGhosts] = {0.0f};
int ghostState[numberOfGhosts] = {GHOST_STATE_RUN, GHOST_STATE_WAIT, GHOST_STATE_WAIT, GHOST_STATE_GO};
static const int ghostMaxPaths = worldHeight * 2;
Tile ghostPath[numberOfGhosts][ghostMaxPaths] = {{0, 0}};
int ghostNumberOfPaths[numberOfGhosts] = {0};
int ghostCurrentPath[numberOfGhosts] = { -1};
bool isGhostEdible[numberOfGhosts] = {false};
int ghostEdiblePoint[numberOfGhosts] = {200, 400, 800, 1600};
int ghostEdiblePointIndex = 0;
float ghostEdiblePointVisibledTime = 0.0f;
String ghostEdiblePointString = "";
Size ghostEdiblePointSize = {0, 0};
Point ghostEdiblePointPosition = {0.0f, 0.0f};
int ghostEdiblePointGhost = -1;
static const Tile runAwayCornerTile[4] = {{1, 1}, {1, 19}, {25, 1}, {25, 19}}; // upper left, upper right, lower left, lower right

// function prototypes
void addDotCounter(int value);
void addGhostPath(int i, Tile tile);
void addScore(unsigned long value);
void changeGameMode(int mode);
void chasePacMan(int i);
void drawBitmap(int x, int y, int width, int height, const uint8_t* bitmap, const uint8_t* mask = NULL, int color = ESPERT_WHITE);
void eat(Tile tile);
void eatGhost(int i);
void eatPowerPellet();
int getDirection(Tile fromTile, Tile toTile);
Point getPositionFromTile(Tile tilePos, Size size);
Tile getTileFromPosition(Point pos, Size size);
void ghostDie(int i);
void initGame();
bool isBlink(float factor = 1.0f);
bool isCollideGhost(int i);
bool isGhostPath(int i);
bool isJustMoveToNewTile(Tile currentTile, Tile lastTile);
bool isTileAvailabled(int direction, Tile tile, bool isDoorTileEnabled = true);
bool isWallTile(Tile tile, bool isDoorTileEnabled = true);
void newLevel();
bool pathFinding(int i, Tile targetTile, bool isDoorTileEnabled = true);
void playSound(int index);
void pressButtons();
bool readGhostPath(int i);
void readHighScores();
void render();
void renderEdibleGhost();
void renderFPS();
void renderGhost();
void renderHUD();
void renderItem();
void renderPacMan();
void renderTitle();
void renderWorld();
void resetGame();
void resetGhostPath(int i);
void resetPacManIncreseSpeed();
void restartLevel();
void runAwayGhost(int i);
void scrollWorldToPacManPosition();
void scrollWorldToPosition(float y);
void setFPSImages(int value);
void spawnGhost(int i, Tile tile, int state, int direction, int waitTime = 0.0f);
String toString(unsigned long value, int length, String prefixChar = "0");
void update();
void updateGhost();
void updatePacMan();
void updateTitle();
void writeHighScore();

void setup() {
  espert.init();
  espert.oled.init();
  espert.buzzer.init(buzzerPin);
  randomSeed(analogRead(0));

  for (int i = 0; i < numberOfButtons; i++) {
    button[i].init(buttonPin[i], INPUT_PULLUP);
  }

  initGame();
  espert.buzzer.on(1);
  espert.buzzer.on(0);
}

void loop() {
  update();
  render();
}

void addDotCounter(int value) {
  dotCounter += value;

  if (itemVisibledTime == 0.0f && (dotCounter == 70 || dotCounter == 170)) { // spawn item
    itemVisibledTime = 10000.0f;
  }

  if (dotCounter >= numberOfDots) {
    changeGameMode(GAME_MODE_SUCCESS);
  }
}

void addGhostPath(int i, Tile tile) {
  if (ghostNumberOfPaths[i] < ghostMaxPaths - 1) {
    ghostPath[i][ghostNumberOfPaths[i]++] = tile;
  }
}

void addScore(unsigned long value) {
  score = constrain(score + value, 0l, maxScore);

  if (!isAwardExtraLife && score >= 10000l) {
    isAwardExtraLife = true;
    numberOfPacManLives++;
  }

  if (score > highScore) {
    highScore = score;
  }
}

void changeGameMode(int mode) {
  fpsPosition = {52, 58};

  switch (mode) {
    case GAME_MODE_TITLE:
      writeHighScore();
      resetGame();
      titleTime = 0.0f;
      pacManDirection = DIRECTION_LEFT;
      pacManPosition = {screenSize.width, 53.0f};
      fpsPosition = {106, 57};
      break;

    case GAME_MODE_READY:
      writeHighScore();
      break;

    case GAME_MODE_PLAY:
      readyTime = maxReadyTime;
      break;

    case GAME_MODE_DIE:
      playSound(SOUND_DIE);
      pacManDieTime = 0.0f;
      pacManDieFrame = 0.0f;
      isPacManDisappeared = false;
      break;

    case GAME_MODE_SUCCESS:
      successTime = 0.0f;
      break;

    case GAME_MODE_GAME_OVER:
      gameOverTime = 0.0f;
      break;
  }

  gameMode = mode;
}

void chasePacMan(int i) {
  if (powerPelletTime == 0.0f && pathFinding(i, pacManTile)) {
    readGhostPath(i);
  }
}

void drawBitmap(int x, int y, int width, int height, const uint8_t* bitmap, const uint8_t* mask, int color) {
  if (mask) {
    espert.oled.setTextColor(1 - color);
    espert.oled.drawBitmap(x, y, mask, width, height, 1 - color, false);
  }

  espert.oled.setTextColor(color);
  espert.oled.drawBitmap(x, y, bitmap, width, height, color, false);
}

void eat(Tile tile) {
  if (gameMode != GAME_MODE_READY && gameMode != GAME_MODE_GAME_OVER && gameMode != GAME_MODE_SUCCESS && gameMode != GAME_MODE_DIE) {
    if (tile.row >= 0 && tile.row < numberOfRows && tile.column >= 0 && tile.column < numberOfColumns) {
      if (itemVisibledTime > 0.0f && tile.row == itemTile.row && tile.column == itemTile.column) { // eat item
        itemVisibledTime = -2000.0f;
        addScore(itemPoint[item[itemIndex]]);
        playSound(SOUND_EAT_ITEM);
        resetPacManIncreseSpeed();
      }

      for (int i = 0; i < numberOfGhosts; i++) {
        if (isCollideGhost(i)) {
          if (isGhostEdible[i]) { // eat ghost
            if (ghostEdiblePointVisibledTime == 0.0f) {
              eatGhost(i);
              break;
            }
          } else if (ghostState[i] != GHOST_STATE_DIE) {
            changeGameMode(GAME_MODE_DIE);
            break;
          }
        }
      }

      if (gameMode != GAME_MODE_READY && gameMode != GAME_MODE_GAME_OVER && gameMode != GAME_MODE_SUCCESS && gameMode != GAME_MODE_DIE) {
        if (world[tile.row][tile.column] == TILE_TYPE_DOT || world[tile.row][tile.column] == TILE_TYPE_POWER_PELLET) {
          if (world[tile.row][tile.column] == TILE_TYPE_DOT) {
            addScore(10);
            playSound(SOUND_EAT_DOT);
            addDotCounter(1);
          } else if (world[tile.row][tile.column] == TILE_TYPE_POWER_PELLET) {
            addScore(50);
            playSound(SOUND_EAT_POWER_PELLET);
            eatPowerPellet();
          }

          world[tile.row][tile.column] = TILE_TYPE_SPACE;
          resetPacManIncreseSpeed();
        }
      }
    }
  }
}

void eatGhost(int i) {
  ghostEdiblePointVisibledTime = 1000.0f;
  ghostEdiblePointString = String(ghostEdiblePoint[ghostEdiblePointIndex]);
  ghostEdiblePointSize = {(ghostEdiblePointString.length() * (numberSize.width - 1)) - 1, numberSize.height};
  ghostEdiblePointPosition = getPositionFromTile(pacManTile, ghostEdiblePointSize);
  addScore(ghostEdiblePoint[ghostEdiblePointIndex]);
  playSound(SOUND_EAT_GHOST);
  ghostEdiblePointIndex++;
  ghostEdiblePointGhost = i;
  ghostDie(i);
}

void eatPowerPellet() {
  addDotCounter(1);
  powerPelletTime = maxPowerPelletTime;
  ghostEdiblePointIndex = 0;

  for (int i = 0; i < numberOfGhosts; i++) {
    if (ghostState[i] != GHOST_STATE_DIE) {
      isGhostEdible[i] = true;
      runAwayGhost(i);
    }
  }
}

int getDirection(Tile fromTile, Tile toTile) {
  int direction = DIRECTION_NONE;

  if (toTile.row == fromTile.row) {
    if (toTile.column == fromTile.column - 1) {
      direction = DIRECTION_LEFT;
    } else if (toTile.column == fromTile.column + 1) {
      direction = DIRECTION_RIGHT;
    }
  } else if (toTile.column == fromTile.column) {
    if (toTile.row == fromTile.row - 1) {
      direction = DIRECTION_UP;
    } else if (toTile.row == fromTile.row + 1) {
      direction = DIRECTION_DOWN;
    }
  }

  return direction;
}

Point getPositionFromTile(Tile tile, Size size) {
  Point pos = {0, 0};
  pos.y = (tile.row * tileSize.height) + ((tileSize.height - size.height) * 0.5f);
  pos.x = (tile.column * tileSize.width) + ((tileSize.width - size.width) * 0.5f);
  return pos;
}

Tile getTileFromPosition(Point pos, Size size) {
  Tile tile = {0, 0};
  tile.row = (pos.y - ((tileSize.height - size.height) * 0.5f)) / tileSize.height;
  tile.column = (pos.x - ((tileSize.width - size.width) * 0.5f)) / tileSize.width;
  return tile;
}

void ghostDie(int i) {
  ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
  ghostState[i] = GHOST_STATE_DIE;
  isGhostEdible[i] = false;

  if (pathFinding(i, {13, 10}, false)) {
    readGhostPath(i);
  }
}

void initGame() {
  readHighScores();
  resetGame();
  changeGameMode(GAME_MODE_TITLE);
  lastFrameTime = millis();
  fpsLastFrameTime = lastFrameTime;
  setFPSImages(frameRate);
}

bool isBlink(float factor) {
  int speed = maxBlinkTime * factor;
  return ((int)blinkTime % speed < speed * 0.5f) ? true : false;
}

bool isCollideGhost(int i) {
  float offset = 2.0f;
  return (pacManPosition.x + offset < ghostPosition[i].x + ghostSize.width - 1 - offset &&
          pacManPosition.x + pacManSize.width - 1 - offset > ghostPosition[i].x + offset &&
          pacManPosition.y + offset < ghostPosition[i].y + ghostSize.height - 1 - offset &&
          pacManPosition.y + pacManSize.height - 1 - offset > ghostPosition[i].y + offset);
}

bool isGhostPath(int i) {
  bool found = (ghostNumberOfPaths[i] > 0) ? true : false;

  if (found) {
    int direction = ghostDirection[i];

    if ((direction == DIRECTION_LEFT && ghostTile[i].column < ghostPath[i][ghostCurrentPath[i]].column) ||
        (direction == DIRECTION_RIGHT && ghostTile[i].column > ghostPath[i][ghostCurrentPath[i]].column) ||
        (direction == DIRECTION_UP && ghostTile[i].row < ghostPath[i][ghostCurrentPath[i]].row) ||
        (direction == DIRECTION_DOWN && ghostTile[i].row > ghostPath[i][ghostCurrentPath[i]].row)) {
      ghostTile[i] = ghostPath[i][ghostCurrentPath[i]];
      bool found = readGhostPath(i);

      if (!found || ghostDirection[i] != direction) {
        ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
      }
    }
  }

  return found;
}

bool isJustMoveToNewTile(Tile currentTile, Tile lastTile) {
  return (currentTile.row != lastTile.row || currentTile.column != lastTile.column) ? true : false;
}

bool isTileAvailabled(int direction, Tile tile, bool isDoorTileEnabled) {
  bool availabled = false;
  int row = tile.row;
  int column = tile.column;
  availabledTile = tile;

  if (direction == DIRECTION_LEFT) {
    column--;
    availabled = (tile.column > 0 && (world[tile.row][column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[tile.row][column] == TILE_TYPE_DOOR)));
  } else if (direction == DIRECTION_RIGHT) {
    column++;
    availabled = (tile.column < numberOfColumns - 1 && (world[tile.row][column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[tile.row][column] == TILE_TYPE_DOOR)));
  } else if (direction == DIRECTION_UP) {
    row--;
    availabled = (tile.row > 0 && tile.column >= 0 && tile.column < numberOfColumns && (world[row][tile.column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[row][tile.column] == TILE_TYPE_DOOR)));
  } else if (direction == DIRECTION_DOWN) {
    row++;
    availabled = (tile.row < numberOfRows - 1 && tile.column >= 0 && tile.column < numberOfColumns && (world[row][tile.column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[row][tile.column] == TILE_TYPE_DOOR)));
  }

  if (availabled) {
    availabledTile = {row, column};
  }

  return availabled;
}

bool isWallTile(Tile tile, bool isDoorTileEnabled) {
  bool isWall = false;

  if (tile.row >= 0 && tile.row < numberOfRows && tile.column >= 0 && tile.column < numberOfColumns) {
    if (world[tile.row][tile.column] > TILE_TYPE_POWER_PELLET) {
      isWall = (world[tile.row][tile.column] == TILE_TYPE_DOOR) ? isDoorTileEnabled : true;
    }
  }

  return isWall;
}

void newLevel() {
  memcpy(&world, &initialWorld, sizeof(world));
  numberOfDots = 0;
  for (byte row = 0; row < numberOfRows; row++) {
    for (byte column = 0; column < numberOfColumns; column++) {
      if (world[row][column] == TILE_TYPE_DOT || world[row][column] == TILE_TYPE_POWER_PELLET) {
        numberOfDots++;
      }
    }
  }

  gameSpeed = constrain(25.0f + (level * 2.0f), 25.0f, 37.0f) / 1000.0f;
  maxPowerPelletTime = constrain(10000.0f - (level * 2000.0f), 3000.0f, 10000.0f);
  dotCounter = 0;

  // item
  itemVisibledTime = 0.0f;
  itemIndex = item[(level >= maxItems - 1) ? maxItems - 1 : level];
  itemPointString = String(itemPoint[itemIndex]);
  itemPointSize = {(itemPointString.length() * (numberSize.width - 1)) - 1, numberSize.height};
  itemPointPosition = getPositionFromTile(itemTile, itemPointSize);

  restartLevel();
}

bool pathFinding(int i, Tile targetTile, bool isDoorTileEnabled) {
  bool finished = false;
  int tryCounter = ghostMaxPaths * 3;
  Tile currentTile = ghostTile[i];
  Tile lastTile = currentTile;
  availabledTile = currentTile;
  resetGhostPath(i);

  while (!finished) {
    int direction[numberOfDirections] = {DIRECTION_NONE};

    if (currentTile.row <= targetTile.row) {
      if (currentTile.column <= targetTile.column) {
        memcpy(&direction, &directionOrderUpperLeft, sizeof(direction));
      } else {
        memcpy(&direction, &directionOrderUpperRight, sizeof(direction));
      }
    } else {
      if (currentTile.column <= targetTile.column) {
        memcpy(&direction, &directionOrderLowerLeft, sizeof(direction));
      } else {
        memcpy(&direction, &directionOrderLowerRight, sizeof(direction));
      }
    }

    for (int n = 0; n < numberOfDirections; n++) {
      if (isTileAvailabled(direction[n], currentTile, isDoorTileEnabled) && !(availabledTile.row == lastTile.row && availabledTile.column == lastTile.column)) {
        lastTile = currentTile;
        currentTile = availabledTile;
        addGhostPath(i, currentTile);
        break;
      }
    }

    if ((currentTile.row == targetTile.row && currentTile.column == targetTile.column) || ghostNumberOfPaths[i] >= ghostMaxPaths - 1 || --tryCounter <= 0) {
      finished = true;
    }

    ESP.wdtFeed();
  }

  return (ghostNumberOfPaths[i] > 0) ? true : false;
}

void playSound(int index) {
  if (isSoundEnabled && isSoundInterruptEnabled) {
    nextSound = SOUND_NONE;
    nextSoundDelay = 0.0f;
    int frequency = 0;

    switch (index) {
      case SOUND_EAT_DOT:
        frequency = 20;
        buzzerDuration = 20.0f;
        break;

      case SOUND_EAT_ITEM:
      case SOUND_EAT_GHOST:
      case SOUND_EAT_POWER_PELLET:
      case SOUND_DIE:
      case SOUND_DISAPPEAR:
        frequency = 30;
        buzzerDuration = 100.0f;
        isSoundInterruptEnabled = false;
        break;

      default:
        frequency = 0;
        buzzerDuration = 0.0f;
        break;
    }

    espert.buzzer.on(frequency);
  }
}

void pressButtons() {
  for (int i = 0; i < numberOfButtons; i++) {
    bool isPressed = (digitalRead(buttonPin[i]) == LOW) ? true : false;

    if (isPressed != isButtonPressed[i]) {
      isButtonPressed[i] = isPressed;

      if (isPressed) {
        pressedButton = i;

        switch (gameMode) {
          case GAME_MODE_READY:
          case GAME_MODE_PLAY:
            if (pressedButton == BUTTON_LEFT || pressedButton == BUTTON_RIGHT || pressedButton == BUTTON_UP || pressedButton == BUTTON_DOWN) {
              pacManNextDirection = pressedButton;

              if (pacManDirection != pacManNextDirection) {
                if (pacManNextDirection == DIRECTION_RIGHT) {
                  if (isPacManStoped) {
                    Tile tile = pacManTile;

                    if (tile.column < numberOfColumns - 1) {
                      tile.column++;
                    }

                    if (!isWallTile(tile)) {
                      pacManDirection = pacManNextDirection;
                      isPacManStoped = false;
                    } else {
                      pacManNextDirection = pacManDirection;
                    }
                  }
                }
                else if (pacManNextDirection == DIRECTION_LEFT) {
                  if (isPacManStoped) {
                    Tile tile = pacManTile;

                    if (tile.column > 0) {
                      tile.column--;
                    }

                    if (!isWallTile(tile)) {
                      pacManDirection = pacManNextDirection;
                      isPacManStoped = false;
                    } else {
                      pacManNextDirection = pacManDirection;
                    }
                  }
                }
                else if (pacManNextDirection == DIRECTION_DOWN) {
                  if (isPacManStoped) {
                    Tile tile = pacManTile;

                    if (tile.row < numberOfRows - 1) {
                      tile.row++;
                    }

                    if (!isWallTile(tile)) {
                      pacManDirection = pacManNextDirection;
                      isPacManStoped = false;
                    } else {
                      pacManNextDirection = pacManDirection;
                    }
                  }
                }
                else if (pacManNextDirection == DIRECTION_UP) {
                  if (isPacManStoped) {
                    Tile tile = pacManTile;

                    if (tile.row > 0) {
                      tile.row--;
                    }

                    if (!isWallTile(tile)) {
                      pacManDirection = pacManNextDirection;
                      isPacManStoped = false;
                    } else {
                      pacManNextDirection = pacManDirection;
                    }
                  }
                }
              }
            }
            break;
        }
      } else if (pressedButton == i) {
        if (gameMode == GAME_MODE_TITLE) {
          newLevel();
          changeGameMode(GAME_MODE_READY);
        } else if (gameMode == GAME_MODE_GAME_OVER) {
          changeGameMode(GAME_MODE_TITLE);
        }

        pressedButton = BUTTON_NONE;
      }
      break;
    }
  }
}

bool readGhostPath(int i) {
  bool found = false;

  if (ghostNumberOfPaths[i] > 0 && ++ghostCurrentPath[i] < ghostNumberOfPaths[i]) {
    int direction = getDirection(ghostTile[i], ghostPath[i][ghostCurrentPath[i]]);

    if (direction != DIRECTION_NONE) {
      ghostDirection[i] = direction;
      ghostNextDirection[i] = ghostDirection[i];
      found = true;
    }
  }

  if (!found) {
    resetGhostPath(i);

    if (ghostState[i] == GHOST_STATE_DIE) {
      if (ghostTile[i].row == 13 && ghostTile[i].column == 10) {
        spawnGhost(i, {13, 10}, GHOST_STATE_GO, DIRECTION_DOWN);
      } else {
        ghostDie(i);
      }
    }
  }

  ghostRandomTime[i] = maxGhostRandomTime;

  return found;
}

void readHighScores() {
  int i = eepromAddress;

  if (espert.eeprom.read(i, eepromKey.length()) == eepromKey) {
    i += eepromKey.length();
    String data = espert.eeprom.read(i, String(maxScore).length());
    highScore = data.toInt();

    if (highScore < 0l || highScore > maxScore) {
      highScore = constrain(highScore, 0l, maxScore);
      writeHighScore();
    }
  }
}

void render() {
  espert.oled.clear(false);

  switch (gameMode) {
    case GAME_MODE_TITLE:
      renderTitle();
      break;

    case GAME_MODE_READY:
    case GAME_MODE_PLAY:
    case GAME_MODE_DIE:
    case GAME_MODE_SUCCESS:
    case GAME_MODE_GAME_OVER:
      renderWorld();

      if (gameMode == GAME_MODE_READY) {
        drawBitmap(worldPosition.x + readyPosition.x, worldPosition.y + readyPosition.y, 40, 8, readyBitmap, readyMaskBitmap);
      } else if (gameMode == GAME_MODE_GAME_OVER) {
        drawBitmap(worldPosition.x + gameOverPosition.x, worldPosition.y + gameOverPosition.y, 64, 8, gameOverBitmap, gameOverMaskBitmap);
      }

      renderItem();
      renderEdibleGhost();
      renderPacMan();
      renderGhost();
      renderHUD();
      break;
  }

  renderFPS();
  espert.oled.update();
  ESP.wdtFeed();
}

void renderEdibleGhost() {
  if (gameMode == GAME_MODE_GAME_OVER) {
  } else if (gameMode == GAME_MODE_DIE && pacManDieTime > 2000.0f) {
  } else if (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f) {
  } else {
    for (int i = 0; i < numberOfGhosts; i++) {
      if (isGhostVisibled[i]) {
        if (ghostState[i] == GHOST_STATE_DIE) {
          if (i != ghostEdiblePointGhost) {
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], ghostEyeMaskBitmap[ghostDirection[i]], ESPERT_BLACK);
          }
        } else if (isGhostEdible[i]) {
          if (powerPelletTime > minPowerPelletTime || (powerPelletTime <= minPowerPelletTime && isBlink(0.5f))) {
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEdible[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
          } else if (powerPelletTime <= minPowerPelletTime) {
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], NULL, ESPERT_BLACK);
          }
        }
      }
    }
  }
}

void renderFPS() {
  if (isFPSVisibled) {
    drawBitmap(fpsPosition.x, fpsPosition.y, 16, 8, fpsBitmap, fpsMaskBitmap);

    for (int i = 0; i < 2; i++) {
      drawBitmap(fpsPosition.x + 14 + (i * (numberSize.width - 1)), fpsPosition.y, 8, 8, fpsDigitImage[i], numberMaskBitmap);
    }
  }
}

void renderGhost() {
  if (gameMode == GAME_MODE_GAME_OVER) {
  } else if (gameMode == GAME_MODE_DIE && pacManDieTime > 2000.0f) {
  } else if (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f) {
  } else {
    for (int i = 0; i < numberOfGhosts; i++) {
      if (isGhostVisibled[i] && !isGhostEdible[i] && ghostState[i] != GHOST_STATE_DIE) {
        drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
        drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], NULL, ESPERT_BLACK);
      }
    }
  }
}

void renderHUD() {
  drawBitmap(7, -1, 32, 8, highScoreBitmap, highScoreMaskBitmap);
  String highScoreString = String(highScore);
  for (int i = 0; i < highScoreString.length(); i++) {
    drawBitmap(37 + (i * (numberSize.width - 1)), -1, 8, 8, numberBitmap[highScoreString.charAt(i) - '0'], numberMaskBitmap);
  }

  String scoreString = String(score);
  for (int i = 0; i < scoreString.length(); i++) {
    drawBitmap(screenSize.width - 8 - ((i + 1) * (numberSize.width - 1)), -1, 8, 8, numberBitmap[scoreString.charAt(scoreString.length() - i - 1) - '0'], numberMaskBitmap);
  }

  for (int i = 0; i < numberOfPacManLives; i++) {
    drawBitmap(7 + (i * 6), 58, 8, 8, lifeBitmap, lifeMaskBitmap);
  }

  for (int i = 0; i < 4 && level - i >= 0; i++) {
    int n = level - i;

    if (n > maxItems - 1) {
      n = maxItems - 1;
    }

    drawBitmap(screenSize.width - 8 - ((i + 1) * itemSize.width), screenSize.height - itemSize.height + 1, 16, 16, itemBitmap[item[n]], itemMaskBitmap[item[n]]);
  }
}

void renderItem() {
  if (gameMode != GAME_MODE_READY && gameMode != GAME_MODE_GAME_OVER) {
    if (itemVisibledTime > 0.0f) {
      drawBitmap(worldPosition.x + itemPosition.x, worldPosition.y + itemPosition.y, 16, 16, itemBitmap[itemIndex], itemMaskBitmap[itemIndex]);
    } else if (itemVisibledTime < 0.0f) {
      for (int i = 0; i < itemPointString.length(); i++) {
        drawBitmap(worldPosition.x + itemPointPosition.x + (i * (numberSize.width - 1)), worldPosition.y + itemPointPosition.y, 8, 8, numberBitmap[itemPointString.charAt(i) - '0'], numberMaskBitmap);
      }
    }
  }
}

void renderPacMan() {
  if (gameMode == GAME_MODE_GAME_OVER) {
  } else if (gameMode == GAME_MODE_DIE && pacManDieTime > 2000.0f) {
    if (pacManDieFrame != -1.0f) {
      drawBitmap(worldPosition.x + pacManPosition.x + 1, worldPosition.y + pacManPosition.y + 1, 16, 16, dieBitmap[(int)pacManDieFrame]);
    }
  } else if (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f) {
    drawBitmap(worldPosition.x + pacManPosition.x, worldPosition.y + pacManPosition.y, 16, 16, pacManBitmap[pacManDirection][(int)pacManFrame], pacManMaskBitmap[pacManDirection][(int)pacManFrame]);
  } else {
    if (isPacManVisibled) {
      if (ghostEdiblePointVisibledTime > 0.0f) {
        for (int i = 0; i < ghostEdiblePointString.length(); i++) {
          drawBitmap(worldPosition.x + ghostEdiblePointPosition.x + (i * (numberSize.width - 1)), worldPosition.y + ghostEdiblePointPosition.y, 8, 8, numberBitmap[ghostEdiblePointString.charAt(i) - '0'], numberMaskBitmap);
        }
      } else {
        drawBitmap(worldPosition.x + pacManPosition.x, worldPosition.y + pacManPosition.y, 16, 16, pacManBitmap[pacManDirection][(int)pacManFrame], pacManMaskBitmap[pacManDirection][(int)pacManFrame]);
      }
    }
  }
}

void renderTitle() {
  if (titleTime > 250.0f && titleTime < 3250.0f) {
    drawBitmap(0, 0, 128, 64, titleBitmap);
    drawBitmap(49, 35, 32, 8, highScoreBitmap);
    String highScoreString = String(highScore);
    int x = (screenSize.width - (highScoreString.length() * (numberSize.width - 1))) * 0.5f;
    for (int i = 0; i < highScoreString.length(); i++) {
      drawBitmap(x + (i * (numberSize.width - 1)), 42, 8, 8, numberBitmap[highScoreString.charAt(i) - '0'], numberMaskBitmap);
    }
  }

  if (titleTime >= 3250.0f) {
    float t = 3500.0f;

    if (titleTime > t) {
      drawBitmap(17, 0, 104, 8, characterNicknameBitmap);
    }

    for (int i = 0; i < numberOfGhosts; i++) {
      t += 880.0f;
      if (titleTime > t) {
        drawBitmap(26, 11 + (i * 10), 40, 8, characterBitmap[i]);
      }

      t += 50.0f;
      if (titleTime > t) {
        int y = 6 + (i * (ghostSize.height - 1));
        drawBitmap(7, y, 16, 16, ghostBitmap[0]);
        drawBitmap(7, y, 16, 16, ghostEyeBitmap[DIRECTION_RIGHT], NULL, ESPERT_BLACK);
      }

      t += 830.0f;
      if (titleTime > t) {
        drawBitmap(81, 11 + (i * 10), 40, 8, nicknameBitmap[i]);
      }
    }

    t += 880.0f;
    if (titleTime > t && titleTime <= t + 2000.0f) {
      drawBitmap(45, 51, 40, 16, ptsBitmap);
    }

    t += 2000.0f; // 13420
    if (titleTime > t) {
      if (pacManDirection == DIRECTION_LEFT && isBlink(0.5f)) {
        drawBitmap(17, 56, 8, 8, tileBitmap[TILE_TYPE_POWER_PELLET]);
      }

      for (int i = ghostEdiblePointIndex; i < numberOfGhosts; i++) {
        if (pacManDirection == DIRECTION_LEFT) {
          drawBitmap(ghostPosition[i].x, ghostPosition[i].y, 16, 16, ghostBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
          drawBitmap(ghostPosition[i].x, ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], NULL, ESPERT_BLACK);
        } else {
          drawBitmap(ghostPosition[i].x, ghostPosition[i].y, 16, 16, ghostEdible[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
        }
      }

      if (ghostEdiblePointVisibledTime > 0.0f) {
        for (int i = 0; i < ghostEdiblePointString.length(); i++) {
          drawBitmap(ghostEdiblePointPosition.x + (i * (numberSize.width - 1)), ghostEdiblePointPosition.y, 8, 8, numberBitmap[ghostEdiblePointString.charAt(i) - '0'], numberMaskBitmap);
        }
      } else {
        drawBitmap(pacManPosition.x, pacManPosition.y, 16, 16, pacManBitmap[pacManDirection][(int)pacManFrame], pacManMaskBitmap[pacManDirection][(int)pacManFrame]);
      }
    }
  }
}

void renderWorld() {
  int y = worldPosition.y;

  for (byte row = 0; row < numberOfRows; row++) {
    if (y > -tileSize.height && y < screenSize.height + tileSize.height) {
      int x = worldPosition.x;

      for (byte column = 0; column < numberOfColumns; column++) {
        int tileType = world[row][column];

        if (tileType != TILE_TYPE_SPACE && (tileType != TILE_TYPE_POWER_PELLET || (tileType == TILE_TYPE_POWER_PELLET && isBlink()))) {
          if (gameMode != GAME_MODE_SUCCESS || (gameMode == GAME_MODE_SUCCESS && successTime <= 2000.0f) || (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f && isBlink(0.5f))) {
            drawBitmap(x, y, 8, 8, tileBitmap[tileType]);
            ESP.wdtFeed();
          }
        }

        x += tileSize.width;
      }
    }

    y += tileSize.height;
  }
}

void resetGame() {
  level = 0;
  score = 0l;
  isAwardExtraLife = false;
  numberOfPacManLives = startPacManLives;
  itemTile = {15, 10};
  itemPosition = getPositionFromTile(itemTile, itemSize);
  restartLevel();
}

void resetGhostPath(int i) {
  memset(&ghostPath[i], 0, sizeof(ghostPath[i]));
  ghostNumberOfPaths[i] = 0;
  ghostCurrentPath[i] = -1;
}

void resetPacManIncreseSpeed() {
  pacManIncreseSpeed = 0.0f;
  pacManIncreseSpeedTime = pacManMaxIncreseSpeedTime;
}

void restartLevel() {
  successTime = 0.0f;

  readyTime = 0.0f;
  Tile tile = {15, 10};
  Size readySize = {40, 8};
  readyPosition = getPositionFromTile(tile, readySize);

  gameOverTime = 0.0f;
  Size gameOverSize = {59, 8};
  gameOverPosition = getPositionFromTile(tile, gameOverSize);

  // pac-man
  pacManTile = {20, 10};
  pacManLastTile = pacManTile;
  pacManPosition = getPositionFromTile(pacManTile, pacManSize);
  worldPosition = {(screenSize.width - worldPixelSize.width) * 0.5f, screenSize.height - worldPixelSize.height};
  scrollWorldToPacManPosition();
  worldStartPosition = worldPosition.y;
  worldPosition.y = 0.0f;
  pacManDirection = DIRECTION_LEFT;
  pacManNextDirection = pacManDirection;
  pacManFrame = 0.0f;
  isPacManStoped = false;
  isPacManVisibled = false;
  powerPelletTime = 0.0f;
  blinkTime = 0.0f;
  pacManDieTime = 0.0f;
  pacManDieFrame = 0.0f;
  isPacManDisappeared = false;
  resetPacManIncreseSpeed();

  // ghost
  ghostEdiblePointIndex = 0;
  ghostEdiblePointVisibledTime = 0.0f;
  ghostEdiblePointGhost = -1;
  for (int i = 0; i < numberOfGhosts; i++) {
    if (i == 0) {
      spawnGhost(i, {13, 11}, GHOST_STATE_WAIT, DIRECTION_UP, (level == 0) ? 36000.0f : (level == 0) ? 16000.0f : 0.0f);
    } else if (i == 1) {
      spawnGhost(i, {13, 9}, GHOST_STATE_WAIT, DIRECTION_UP, (level == 0) ? 6000.0f : 0.0f);
    } else if (i == 2) {
      spawnGhost(i, {13, 10}, GHOST_STATE_GO, DIRECTION_UP);
    } else if (i == 3) {
      spawnGhost(i, {10, 10}, GHOST_STATE_RUN, DIRECTION_LEFT + random(0, 2));
    }

    ghostLastTile[i] = ghostTile[i];
    ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
    ghostFrame[i] = 0.0f;
    isGhostStoped[i] = false;
    isGhostVisibled[i] = false;
    isGhostEdible[i] = false;
    ghostRandomTime[i] = maxGhostRandomTime;
    resetGhostPath(i);
  }
}

void runAwayGhost(int i) {
  if (ghostState[i] == GHOST_STATE_RUN) {
    if (isJustMoveToNewTile(ghostTile[i], ghostLastTile[i])) {
      ghostPosition[i] = getPositionFromTile(ghostLastTile[i], ghostSize);
    } else {
      ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
    }

    int pacManCorner = 0;

    // find pac-man corner position
    if (pacManTile.row <= numberOfRows * 0.5f) { // upper
      if (pacManTile.column <= numberOfColumns * 0.5f) { // left
        pacManCorner = 0;
      } else { // right
        pacManCorner = 1;
      }
    } else { // lower
      if (pacManTile.column <= numberOfColumns * 0.5f) { // left
        pacManCorner = 2;
      } else { // right
        pacManCorner = 3;
      }
    }

    int availabledCorners[3] = {0};
    int n = 0;
    for (int corner = 0; corner < 4; corner++) {
      if (corner != pacManCorner) {
        availabledCorners[n++] = corner;
      }
    }

    int ghostCorner = random(0, 3); // random 0 to 2
    if (pathFinding(i, runAwayCornerTile[availabledCorners[ghostCorner]])) {
      readGhostPath(i);
    }
  }
}

void scrollWorldToPacManPosition() {
  if (gameMode != GAME_MODE_GAME_OVER) {
    float y = round(pacManPosition.y + (pacManSize.height * 0.5f));
    if (y > screenCenter.y - statusBarSize.height - 1.0f && y < worldPixelSize.height - screenCenter.y) {
      worldPosition.y = screenCenter.y - y;
    }
  }
}

void scrollWorldToPosition(float y) {
  if (worldPosition.y > y) {
    worldPosition.y -= elapsedTime * 22.0f / 1000.0f;

    if (worldPosition.y <= y) {
      worldPosition.y = y;
    }
  } else if (worldPosition.y < y) {
    worldPosition.y += elapsedTime * 22.0f / 1000.0f;

    if (worldPosition.y >= y) {
      worldPosition.y = y;
    }
  }
}

void setFPSImages(int value) {
  if (isFPSVisibled) {
    value = constrain(value, 0, 99);
    String fpsString = toString(value, 2, "0");

    for (int i = 0; i < 2; i++) {
      fpsDigitImage[i] = numberBitmap[fpsString.charAt(i) - '0'];
    }
  }
}

void spawnGhost(int i, Tile tile, int state, int direction, int waitTime) {
  ghostTile[i] = tile;
  ghostState[i] = state;
  ghostDirection[i] = direction;
  ghostNextDirection[i] = direction;
  ghostWaitTime[i] = waitTime;
}

String toString(unsigned long value, int length, String prefixChar) {
  String stringValue = String(value);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += prefixChar;
  }

  return prefix + stringValue;
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

  // sound
  if (buzzerDuration > 0.0f) {
    buzzerDuration -= elapsedTime;

    if (buzzerDuration <= 0.0f) {
      buzzerDuration = 0.0f;
      espert.buzzer.off();
      isSoundInterruptEnabled = true;
    }
  } else if (nextSoundDelay > 0.0f) {
    nextSoundDelay -= elapsedTime;

    if (nextSoundDelay <= 0.0f) {
      nextSoundDelay = 0.0f;
      playSound(nextSound);
    }
  }

  blinkTime += elapsedTime;
  if (blinkTime >= maxBlinkTime) {
    blinkTime = 0.0f;
  }

  switch (gameMode) {
    case GAME_MODE_TITLE:
      updateTitle();
      break;

    case GAME_MODE_READY:
      if (readyTime >= 1000.0f) {
        scrollWorldToPosition(worldStartPosition);
      }

      if (readyTime < maxReadyTime) {
        readyTime += elapsedTime;

        if (readyTime >= 3000.0f && !isPacManVisibled) {
          numberOfPacManLives--;
          isPacManVisibled = true;

          for (int i = 0; i < numberOfGhosts; i++) {
            isGhostVisibled[i] = true;
          }
        }
      } else {
        changeGameMode(GAME_MODE_PLAY);
      }
      break;

    case GAME_MODE_DIE:
    case GAME_MODE_PLAY:
    case GAME_MODE_SUCCESS:
    case GAME_MODE_GAME_OVER:
      if (gameMode == GAME_MODE_DIE) {
        pacManDieTime += elapsedTime;

        if (pacManDieTime > 2000.0f) {
          if (pacManDieFrame != -1.0f) {
            pacManDieFrame += elapsedTime * 10.0f / 1000.0f;

            if (!isPacManDisappeared && pacManDieFrame >= numberOfDieFrames - 2) {
              playSound(SOUND_DISAPPEAR);
              isPacManDisappeared = true;
            } else if (pacManDieFrame >= numberOfDieFrames) {
              pacManDieFrame = -1.0f;
            }
          }

          if (pacManDieFrame == -1.0f && pacManDieTime > 5000.0f) {
            if (numberOfPacManLives == 0) {
              changeGameMode(GAME_MODE_GAME_OVER);
            } else {
              restartLevel();
              changeGameMode(GAME_MODE_READY);
            }
            break;
          }
        }
      } else if (gameMode == GAME_MODE_SUCCESS) {
        successTime += elapsedTime;

        if (successTime > 2000.0f) {
          pacManFrame = numberOfPacManFrames - 1;

          if (successTime > 5000.0f) {
            if (++level >= numberOfLevels) {
              level = 0;
            }

            numberOfPacManLives++;
            newLevel();
            changeGameMode(GAME_MODE_READY);
            break;
          }
        }
      } else if (gameMode == GAME_MODE_GAME_OVER) {
        scrollWorldToPosition(worldStartPosition + (tileSize.height * 5.0f));
        gameOverTime += elapsedTime;

        if (gameOverTime > 5000.0f) {
          changeGameMode(GAME_MODE_TITLE);
          break;
        }
      }

      if (gameMode == GAME_MODE_PLAY) {
        if (itemVisibledTime > 0.0f) {
          itemVisibledTime -= elapsedTime;
          if (itemVisibledTime <= 0.0f) {
            itemVisibledTime = 0.0f;
          }
        } else if (itemVisibledTime < 0.0f) {
          itemVisibledTime += elapsedTime;
          if (itemVisibledTime >= 0.0f) {
            itemVisibledTime = 0.0f;
          }
        }
      }

      updateGhost();
      updatePacMan();
      break;
  }

  ESP.wdtFeed();
}

void updateGhost() {
  if (ghostEdiblePointVisibledTime > 0.0f) {
    ghostEdiblePointVisibledTime -= elapsedTime;

    if (ghostEdiblePointVisibledTime <= 0.0f) {
      ghostEdiblePointVisibledTime = 0.0f;
      ghostEdiblePointGhost = -1;
    }
  }

  for (int i = 0; i < numberOfGhosts; i++) {
    if (!isGhostStoped[i]) {
      ghostFrame[i] += elapsedTime * ghostAnimationSpeed;

      if (ghostFrame[i] >= numberOfGhostFrames) {
        ghostFrame[i] = 0.0f;
      }

      if (ghostWaitTime[i] > 0.0f) {
        ghostWaitTime[i] -= elapsedTime;

        if (ghostWaitTime[i] <= 0.0f) {
          ghostWaitTime[i] = 0.0f;
        }
      }

      bool isHitWall = false;
      bool isDoorTileEnabled = (ghostState[i] == GHOST_STATE_GO) ? false : true;
      float ghostSpeed = gameSpeed;

      if (ghostState[i] == GHOST_STATE_DIE) {
        ghostSpeed = gameNormalSpeed * 3.0f;
      } else if (ghostState[i] == GHOST_STATE_WAIT || ghostState[i] == GHOST_STATE_GO) {
        ghostSpeed = gameNormalSpeed * 0.6f;
      } else if (isGhostEdible[i] || (ghostTile[i].row == 13 && (ghostTile[i].column <= 3 || ghostTile[i].column >= numberOfColumns - 4))) { // tunnel
        ghostSpeed *= 0.5f;
      }

      if (gameMode == GAME_MODE_GAME_OVER || gameMode == GAME_MODE_SUCCESS || gameMode == GAME_MODE_DIE || (ghostEdiblePointVisibledTime > 0.0f && (ghostState[i] != GHOST_STATE_DIE || (ghostState[i] == GHOST_STATE_DIE && i == ghostEdiblePointGhost)))) {
        ghostSpeed = 0.0f;
      }

      if (ghostDirection[i] == DIRECTION_LEFT) {
        ghostPosition[i].x -= elapsedTime * ghostSpeed;
        ghostTile[i] = getTileFromPosition(ghostPosition[i], ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].column < numberOfColumns - 1) {
            ghostTile[i].column++;
          } else if (ghostTile[i].column == -2) {
            ghostTile[i].column = numberOfColumns;
            ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
          }
        }
      } else if (ghostDirection[i] == DIRECTION_RIGHT) {
        ghostPosition[i].x += elapsedTime * ghostSpeed;
        Point pos = ghostPosition[i];
        pos.x += tileSize.width;
        ghostTile[i] = getTileFromPosition(pos, ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].column > 0) {
            ghostTile[i].column--;
          } else if (ghostTile[i].column == numberOfColumns + 1) {
            ghostTile[i].column = -1;
            ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
          }
        }
      } else if (ghostDirection[i] == DIRECTION_UP) {
        ghostPosition[i].y -= elapsedTime * ghostSpeed;
        ghostTile[i] = getTileFromPosition(ghostPosition[i], ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].row < numberOfRows - 1) {
            ghostTile[i].row++;
          }
        }
      } else if (ghostDirection[i] == DIRECTION_DOWN) {
        ghostPosition[i].y += elapsedTime * ghostSpeed;
        Point pos = ghostPosition[i];
        pos.y += tileSize.height;
        ghostTile[i] = getTileFromPosition(pos, ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].row > 0) {
            ghostTile[i].row--;
          }
        }
      }

      if (isHitWall) {
        ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);

        if (ghostState[i] == GHOST_STATE_WAIT) {
          if (ghostDirection[i] == DIRECTION_DOWN) {
            if (ghostWaitTime[i] == 0.0f) {
              ghostState[i] = GHOST_STATE_GO;
              resetGhostPath(i);
              addGhostPath(i, {13, 10});
              addGhostPath(i, {12, 10});
              addGhostPath(i, {11, 10});
              addGhostPath(i, {10, 10});
              readGhostPath(i);
            } else {
              ghostDirection[i] = DIRECTION_UP;
              ghostNextDirection[i] = ghostDirection[i];
            }
          } else {
            ghostDirection[i] = DIRECTION_DOWN;
            ghostNextDirection[i] = ghostDirection[i];
          }
        } else if (ghostState[i] == GHOST_STATE_GO) {
          if (ghostDirection[i] == DIRECTION_UP) {
            ghostDirection[i] = DIRECTION_LEFT + random(0, 2); // random 0 to 1
            ghostNextDirection[i] = ghostDirection[i];
            ghostState[i] = GHOST_STATE_RUN;
            ghostRandomTime[i] = maxGhostRandomTime;
          } else {
            ghostDirection[i] = DIRECTION_UP;
            ghostNextDirection[i] = ghostDirection[i];
          }
        } else if (ghostState[i] == GHOST_STATE_RUN) {
          if (ghostNextDirection[i] == ghostDirection[i]) {
            ghostRandomTime[i] = maxGhostRandomTime;
            int n = random(0, 3); // random 0 to 2

            if (n < 2) { // force cross direction
              if (ghostDirection[i] == DIRECTION_LEFT || ghostDirection[i] == DIRECTION_RIGHT) {
                ghostNextDirection[i] = DIRECTION_UP + n;

                if (!isTileAvailabled(ghostNextDirection[i], ghostTile[i])) {
                  ghostNextDirection[i] = DIRECTION_UP + (1 - n);
                }
              } else {
                ghostNextDirection[i] = DIRECTION_LEFT + n;

                if (!isTileAvailabled(ghostNextDirection[i], ghostTile[i])) {
                  ghostNextDirection[i] = DIRECTION_LEFT + (1 - n);
                }
              }
            } else {
              chasePacMan(i);
            }
          }

          ghostDirection[i] = ghostNextDirection[i];
        }
      } else if (ghostNumberOfPaths[i] == 0) {
        if (ghostState[i] == GHOST_STATE_RUN) {
          ghostRandomTime[i] -= elapsedTime;

          if (ghostRandomTime[i] <= 0.0f) {
            ghostRandomTime[i] = 0.0f;

            // change direction while running
            if (ghostLastTile[i].row >= 0 && ghostLastTile[i].row < numberOfRows && ghostLastTile[i].column >= 0 && ghostLastTile[i].column < numberOfColumns) {
              if (isJustMoveToNewTile(ghostTile[i], ghostLastTile[i])) {
                if (random(0, 2) == 0) { // random 0 to 1
                  ghostRandomTime[i] = maxGhostRandomTime;
                  int n = random(0, 3); // random 0 to 2

                  if (n < 2) { // cross direction, if tile availabled
                    if (ghostDirection[i] == DIRECTION_LEFT || ghostDirection[i] == DIRECTION_RIGHT) {
                      ghostNextDirection[i] = DIRECTION_UP + n;
                    } else {
                      ghostNextDirection[i] = DIRECTION_LEFT + n;
                    }

                    if (isTileAvailabled(ghostNextDirection[i], ghostLastTile[i]), isDoorTileEnabled) {
                      ghostTile[i] = ghostLastTile[i];
                      ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
                      ghostDirection[i] = ghostNextDirection[i];
                    } else {
                      ghostNextDirection[i] = ghostDirection[i];
                    }
                  } else {
                    chasePacMan(i);
                  }
                }
              }
            }
          }
        }
      }

      ghostLastTile[i] = ghostTile[i];
    }
  }
}

void updatePacMan() {
  if (powerPelletTime > 0.0f && ghostEdiblePointVisibledTime == 0.0f) {
    powerPelletTime -= elapsedTime;

    if (powerPelletTime <= 0.0f) {
      powerPelletTime = 0.0f;
      memset(&isGhostEdible, false, sizeof(isGhostEdible));
    }
  }

  if (!isPacManStoped) {
    if (gameMode == GAME_MODE_PLAY) {
      if (pacManIncreseSpeedTime > 0.0f) {
        pacManIncreseSpeedTime -= elapsedTime;

        if (pacManIncreseSpeedTime <= 0.0f) {
          pacManIncreseSpeedTime = 0.0f;
        }
      } else if (pacManIncreseSpeed < pacManMaxIncreseSpeed) {
        pacManIncreseSpeed += elapsedTime * 0.002f;

        if (pacManIncreseSpeed >= pacManMaxIncreseSpeed) {
          pacManIncreseSpeed = pacManMaxIncreseSpeed;
        }
      }
    }

    float pacManSpeed = gameSpeed + (pacManIncreseSpeed / 1000.0f);
    if (gameMode == GAME_MODE_GAME_OVER || gameMode == GAME_MODE_SUCCESS || gameMode == GAME_MODE_DIE || ghostEdiblePointVisibledTime > 0.0f) {
      pacManSpeed = 0.0f;
    }

    if (pacManSpeed > 0.0f) {
      pacManFrame += elapsedTime * pacManAnimationSpeed;

      if (pacManFrame >= numberOfPacManFrames) {
        pacManFrame = 0.0f;
      }
    }

    bool isHitWall = false;

    if (pacManDirection == DIRECTION_LEFT) {
      pacManPosition.x -= elapsedTime * pacManSpeed;
      pacManTile = getTileFromPosition(pacManPosition, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.column < numberOfColumns - 1) {
        pacManTile.column++;
      } else if (pacManTile.column == -2) {
        pacManTile.column = numberOfColumns;
        pacManPosition = getPositionFromTile(pacManTile, pacManSize);
      }
    } else if (pacManDirection == DIRECTION_RIGHT) {
      pacManPosition.x += elapsedTime * pacManSpeed;
      Point pos = pacManPosition;
      pos.x += tileSize.width;
      pacManTile = getTileFromPosition(pos, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.column > 0) {
        pacManTile.column--;
      } else if (pacManTile.column == numberOfColumns + 1) {
        pacManTile.column = -1;
        pacManPosition = getPositionFromTile(pacManTile, pacManSize);
      }
    } else if (pacManDirection == DIRECTION_UP) {
      pacManPosition.y -= elapsedTime * pacManSpeed;
      pacManTile = getTileFromPosition(pacManPosition, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.row < numberOfRows - 1) {
        pacManTile.row++;
      }
    } else if (pacManDirection == DIRECTION_DOWN) {
      pacManPosition.y += elapsedTime * pacManSpeed;
      Point pos = pacManPosition;
      pos.y += tileSize.height;
      pacManTile = getTileFromPosition(pos, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.row > 0) {
        pacManTile.row--;
      }
    }

    if (isHitWall) {
      pacManPosition = getPositionFromTile(pacManTile, pacManSize);

      if (pacManDirection != pacManNextDirection) {
        if (isTileAvailabled(pacManNextDirection, pacManTile)) {
          pacManDirection = pacManNextDirection;
        } else {
          pacManNextDirection = pacManDirection;
        }
      } else {
        isPacManStoped = true;
        resetPacManIncreseSpeed();

        if ((int)pacManFrame == numberOfPacManFrames - 1) {
          pacManFrame = 0.0f;
        }
      }
    } else if (pacManDirection != pacManNextDirection) {
      if (isJustMoveToNewTile(pacManTile, pacManLastTile)) {
        if (isTileAvailabled(pacManNextDirection, pacManLastTile)) {
          if (((pacManDirection == DIRECTION_LEFT || pacManDirection == DIRECTION_RIGHT) && (pacManNextDirection == DIRECTION_UP || pacManNextDirection == DIRECTION_DOWN)) ||
              ((pacManDirection == DIRECTION_UP || pacManDirection == DIRECTION_DOWN) && (pacManNextDirection == DIRECTION_LEFT || pacManNextDirection == DIRECTION_RIGHT))) {
            pacManTile = pacManLastTile;
            pacManPosition = getPositionFromTile(pacManTile, pacManSize);
          }

          pacManDirection = pacManNextDirection;
        }
      }
    }

    pacManLastTile = pacManTile;
    scrollWorldToPacManPosition();
  }

  eat(pacManTile);
}

void updateTitle() {
  bool finished = false;
  float speed = gameNormalSpeed * 1.5f;
  titleTime += elapsedTime;

  if (titleTime > 13420.0f) {
    if (ghostEdiblePointVisibledTime == 0.0f) {
      pacManFrame += elapsedTime * pacManAnimationSpeed;

      if (pacManFrame >= numberOfPacManFrames) {
        pacManFrame = 0.0f;
      }

      pacManPosition.x += elapsedTime * speed * (pacManDirection == DIRECTION_LEFT ? -1.0f : 1.0f);

      if (powerPelletTime == 0.0f && pacManPosition.x < 17.0f) { // eat power pellet
        powerPelletTime = maxPowerPelletTime;
        pacManDirection = DIRECTION_RIGHT;
      }
    } else {
      ghostEdiblePointVisibledTime -= elapsedTime;

      if (ghostEdiblePointVisibledTime <= 0.0f) {
        ghostEdiblePointVisibledTime = 0.0f;
        ghostEdiblePointGhost = -1;

        if (ghostEdiblePointIndex == numberOfGhosts) {
          changeGameMode(GAME_MODE_TITLE);
          finished = true;
        }
      }
    }

    for (int i = ghostEdiblePointIndex; i < numberOfGhosts && !finished; i++) {
      if (ghostEdiblePointVisibledTime == 0.0f) {
        ghostFrame[i] += elapsedTime * ghostAnimationSpeed;

        if (ghostFrame[i] >= numberOfGhostFrames) {
          ghostFrame[i] = 0.0f;
        }
      }

      ghostDirection[i] = pacManDirection;

      if (pacManDirection == DIRECTION_LEFT) {
        ghostPosition[i] = {pacManPosition.x + 13 + (i * (ghostSize.width + 1)), pacManPosition.y};
      } else {
        if (ghostEdiblePointVisibledTime == 0.0f) {
          ghostPosition[i].x += elapsedTime * (speed * 0.35f);

          if (isCollideGhost(i)) {
            ghostEdiblePointVisibledTime = 1000.0f;
            ghostEdiblePointString = String(ghostEdiblePoint[ghostEdiblePointIndex]);
            ghostEdiblePointSize = {(ghostEdiblePointString.length() * (numberSize.width - 1)) - 1, numberSize.height};
            ghostEdiblePointPosition = {ghostPosition[i].x + ((ghostSize.width - ghostEdiblePointSize.width) * 0.5f) - 3, ghostPosition[i].y + ((ghostSize.height - ghostEdiblePointSize.height) * 0.5f)};
            ghostEdiblePointIndex++;
            ghostEdiblePointGhost = i;
          }
        }
      }
    }
  }
}

void writeHighScore() {
  int i = eepromAddress;
  espert.eeprom.write(i, eepromKey);

  i += eepromKey.length();
  espert.eeprom.write(i, toString(highScore, String(maxScore).length(), "0"));
}
