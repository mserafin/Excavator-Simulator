#include "Button.h"

Button::Button() {}

void Button::begin(uint8_t pin) {
  this->pin = pin;
  pinMode(pin, INPUT_PULLUP);
}

void Button::begin(uint8_t pin, Expander *expander) {
  this->pin = pin;
  this->expander = expander;
  this->expander->pinMode(pin, INPUT_PULLUP);
}

uint8_t Button::getPin() const {
  return pin;
}

void Button::refresh() {
  uint8_t state = expander != NULL ?
                  expander->digitalRead(pin) : digitalRead(pin);

  if (state != story.lastState && state == LOW) {
    story.state = !story.state;

    if (onClick != NULL) {
      onClick(story.state);
    }
  }
  story.lastState = state;
}

