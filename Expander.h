#pragma once
#include "Arduino.h"
#include <Wire.h>
#include "PCF8574.h"

class Expander : public PCF8574 {
  public:
    Expander();

    void pinMode(uint8_t pin, uint8_t mode);

    void blinkAsync(uint8_t pin, uint16_t count, uint32_t duration);

    void toggleAsync(uint8_t pin);

    void refresh();

  protected:

  private:
    const static int COUNT_PINS_EXPANDER = 8;

    struct State {
      uint8_t count;
      uint16_t duration;
      uint32_t lastMillis;
    } store[COUNT_PINS_EXPANDER];
};
