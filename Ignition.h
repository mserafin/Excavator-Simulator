#pragma once
#include "Arduino.h"

class Ignition {
  public:
    Ignition();

    void begin(int pin, void (*callback)(void));

    void refresh();

    bool getStatus();

  protected:

  private:
    void (*onChangeStatus)(void);

    int pin;

    bool status = true;
};
