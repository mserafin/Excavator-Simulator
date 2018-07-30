#pragma once
#include "Arduino.h"
#include "Expander.h"
#include "Button.h"

class ButtonArray {
  private:
    uint8_t counter = 0;
    
    uint8_t size = 0;

  public:
    ButtonArray(uint8_t size);

    void append(Button button);

    void refresh();

  protected:
    Button *buttons;

};
