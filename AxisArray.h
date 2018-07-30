#pragma once
#include "Arduino.h"
#include "Axis.h"

class AxisArray {
  private:
    uint8_t counter = 0;
    uint8_t size = 0;

  public:
    AxisArray(uint8_t size);

    void append(Axis axis);

    void refresh();

  protected:
    Axis* axises;

};
