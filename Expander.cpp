#include "Expander.h"

Expander::Expander() {}

void Expander::pinMode(uint8_t pin, uint8_t mode) {
  if (mode == OUTPUT) {
    store[pin] = {};
  }
  PCF8574::pinMode(pin, mode);
}

void Expander::blinkAsync(uint8_t pin, uint16_t count, uint32_t duration) {
  duration /= (count *= 2);

  State &state = store[pin];
  state.duration = duration;
  state.count += count;
}

void Expander::toggleAsync(uint8_t pin) {
  State &state = store[pin];
  state.duration = 0;
  state.count += 1;
}

void Expander::refresh() {
  uint32_t currentMillis = millis();

  for (uint8_t pin = 0; pin < COUNT_PINS_EXPANDER; pin++) {
    State &state = store[pin];

    if (state.count > 0 && currentMillis - state.lastMillis >= state.duration) {
      state.count--;
      state.lastMillis = currentMillis;
      PCF8574::toggle(pin);
    }
  }
}
