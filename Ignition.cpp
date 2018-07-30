#include "Ignition.h"

Ignition::Ignition() {}

void Ignition::begin(int pin, void (*callback)(void)) {
  this->pin = pin;
  this->onChangeStatus = callback;
  pinMode(pin, INPUT_PULLUP);
}

void Ignition::refresh() {
  while (!this->getStatus());
}

bool Ignition::getStatus() {
  int state = digitalRead(this->pin);
  if (state != this->status) {
    this->status = state;
    this->onChangeStatus();
  }
  return state;
}
