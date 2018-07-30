#include "Display.h"

Display::Display() : LiquidCrystal_I2C (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE) {

}

void Display::display(String text, bool clear = true, uint8_t x = 0, uint8_t y = 0) {
  if (clear) {
    LiquidCrystal_I2C::clear();
  }
  LiquidCrystal_I2C::setCursor(x, y);
  LiquidCrystal_I2C::print(text);
}
