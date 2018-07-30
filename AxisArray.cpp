#include "AxisArray.h"

AxisArray::AxisArray(uint8_t size) {
  this->size = size;
  delete[] this->axises;
  this->counter = 0;
  this->axises = new Axis[this->size];
}

void AxisArray::append(Axis axis) {
  this->axises[this->counter++] = axis;
}

void AxisArray::refresh() {
  for (byte i = 0; i < this->size; i++)
  {
    this->axises[i].refresh();
  }
}

