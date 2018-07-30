#pragma once
#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

class Display : public LiquidCrystal_I2C {
  public:
    Display();

    void display(String text, bool clear = true, uint8_t x = 0, uint8_t y = 0);

  protected:

  private:
};
