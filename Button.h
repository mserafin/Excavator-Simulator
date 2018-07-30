#pragma once
#include "Arduino.h"
#include "Expander.h"

class Button {
  public:
    Button();

    void begin(uint8_t pin);

    void begin(uint8_t pin, Expander *expander);

    void (*onClick)(bool state);

    void (*onDoubleClick)();

    void (*onHold)();

    void refresh();

    uint8_t getPin() const;

  protected:
    Expander *expander;

    uint8_t pin;

    struct Store {
      bool state;
      bool lastState;
      uint32_t lastMillis;
    } story;

  private:
};
