#pragma once
#include "Arduino.h"

class Axis {
  public:
    Axis();
    
    Axis(int rangeAxis, unsigned long responseDelay);

    void begin(int pin, void (*callback)(int value));

    void refresh();

  protected:
    struct Store {
      bool state;
      unsigned long lastReadMillis;
    } story;

    void checkDelayAxis(int reading, Store &store);

    int readAxis(int pin);

    void (*onChange)(int value);

  private:
    int rangeAxis = 10;
    
    unsigned long responseDelay = 250;
    
    int pin;
};
