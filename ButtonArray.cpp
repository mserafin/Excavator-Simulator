#include "ButtonArray.h"

ButtonArray::ButtonArray(uint8_t size) {
  this->size = size;
  delete[] this->buttons;
  this->counter = 0;
  this->buttons = new Button[this->size];
}

void ButtonArray::append(Button button) {
  this->buttons[this->counter++] = button;
}

void ButtonArray::refresh() {
  for (byte i = 0; i < this->size; i++)
  {
    this->buttons[i].refresh();
  }
}

