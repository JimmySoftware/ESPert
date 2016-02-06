#include "ESPert.h"

ESPert espert;

void setup() {
  espert.init();
  espert.oled.init();
  espert.neopixel.init();
}

void loop() {
  /*
    -- Predefine Color --
    ColorRed     0xFF0000
    ColorGreen   0x008000
    ColorBlue    0x0000FF
    ColorCyan    0x00FFFF
    ColorPurple  0x800080
    ColorGray    0x808080
    ColorBrown   0xA52A2A
    ColorSilver  0xC0C0C0
    ColorViolet  0xEE82EE
    ColorPink    0xFFC0CB
    ColorGold    0xFFD700
    ColorYellow  0xFFFF00
    ColorWhite   0xFFFFFF
  */

  espert.neopixel.rainbow();
  delay(1000);

  espert.neopixel.setColor(ColorRed);
  delay(1000);

  espert.neopixel.setColor(ColorGreen);
  delay(1000);

  espert.neopixel.setColor(ColorBlue);
  delay(1000);

  espert.neopixel.setPixelColor(0, ColorRed);
  espert.neopixel.setPixelColor(1, ColorGreen);
  espert.neopixel.setPixelColor(2, ColorBlue);
  espert.neopixel.setPixelColor(3, ColorCyan);
  espert.neopixel.setPixelColor(4, ColorPurple);
  espert.neopixel.setPixelColor(5, ColorGray);
  espert.neopixel.setPixelColor(6, ColorBrown);
  espert.neopixel.setPixelColor(7, ColorGold);

  espert.neopixel.show();
  delay(1000);

  espert.neopixel.off();
  delay(1000);
}
