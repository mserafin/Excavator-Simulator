#include "Axis.h"

Axis::Axis() {}

Axis::Axis(int rangeAxis, unsigned long responseDelay) {
  this->rangeAxis = rangeAxis;
  this->responseDelay = responseDelay;
}

void Axis::begin(int pin, void (*callback)(int value)) {
  this->pin = pin;
  this->onChange = callback;
  pinMode(this->pin, INPUT);
}

void Axis::refresh() {
  int reading = this->readAxis(this->pin);
  if (reading != 0) {
    this->checkDelayAxis(reading, story);
    if (!story.state) {
      this->onChange(reading);
    }
  }
}

int Axis::readAxis(int pin) {
  int reading = analogRead(pin);
  reading = map(reading, 0, 1023, 0, (rangeAxis * 2));

  int distance = reading - rangeAxis;

  if (abs(distance) < 2) {
    distance = 0;
  }

  return distance;
}

void Axis::checkDelayAxis(int reading, Store &store) {
  float diff = rangeAxis - abs(reading);
  if (diff == 0) {
    diff = 0.1;
  }

  unsigned long currentMillis = millis();
  if ((currentMillis - store.lastReadMillis) > (diff * responseDelay)) {
    store.lastReadMillis = currentMillis;
    store.state = false;
    return;
  }
  store.state = true;
}

