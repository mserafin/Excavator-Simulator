#include <Keyboard.h>
#include "Expander.h"
#include "Ignition.h"
#include "Display.h"
#include "Axis.h"
#include "Button.h"
#include "ButtonArray.h"
#include "AxisArray.h"
//#include "Mouse.h"

const bool DEBUG = 0;

const int IGNITION_PIN = 10;
const int RANGE_AXIS = 10;
const int COUNT_AXLES = 8;
const int COUNT_SWITCHES = 10;
const int COUNT_LEDS = 6;
const unsigned long RESPONSE_DELAY = 500UL;
const unsigned long DISPLAY_DELAY = 1500UL;

bool buttonAxisA1IsActive = false;
bool buttonAxisA2IsActive = false;

unsigned long lastDisplayMillis;

Display lcd;
Expander expander1;
Expander expander2;
Ignition ignition;
ButtonArray buttonArray = ButtonArray(COUNT_SWITCHES);
AxisArray axisArray = AxisArray(COUNT_AXLES);

enum Simulator {
  EXCAVATORS,
  FARMING
};

struct ShortcutsAxis {
  char KEY_JOYSTICK_UP;
  char KEY_JOYSTICK_DOWN;
  char KEY_JOYSTICK_LEFT;
  char KEY_JOYSTICK_RIGHT;
  char KEY_JOYSTICK_SWITCH;
};

struct Joysticks {
  ShortcutsAxis Joysticks1;
  ShortcutsAxis Joysticks2;
  ShortcutsAxis Joysticks3;
  ShortcutsAxis Joysticks4;
} Mapping[2] = {
  {
    {KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW},
    {'a', 'q', 'k', 'i'},
    {'w', 's'},
    {'e', 'd', 'r', 'f'}
  },
  {
    {'w', 's', 'a', 'd'}
  }
};

struct DebugAxis {
  String axisName;
  int axisValue;
};

Simulator simulator = EXCAVATORS;

void setup() {
  setupIgnition();
  setupExpander();
  setupArcadeLed();
  setupOutput();
  setupDisplay();
  setupSwitch();
  setupAxis();
}

void loop() {
  inspectIgnition();
  inspectExpander();
  inspectSwitch();
  inspectAxis();

  if (checkDisplayUpdate()) {
    lcd.display("Symulator!", true, 0, 0);
    lcd.display("Milosz", false, 0, 1);
  }
}

void setupIgnition() {
  ignition.begin(IGNITION_PIN, ignitionOnChangeStatus);
}

void setupExpander() {
  expander1.begin(0x20);
  expander2.begin(0x21);
}

void setupArcadeLed() {
  struct Config {
    byte pin;
    Expander *expander;
  } leds[COUNT_LEDS] = {
    {1, &expander1},
    {3, &expander1},
    {5, &expander1},
    {7, &expander1},
    {1, &expander2},
    {3, &expander2}
  };

  for (byte i = 0; i < COUNT_LEDS; i++) {
    Config &config = leds[i];
    config.expander->pinMode(config.pin, OUTPUT);
    config.expander->digitalWrite(config.pin, HIGH);
  }
}

void setupOutput() {
  if (!DEBUG) {
    Keyboard.begin();
    //Mouse.begin();
    return;
  }

  while (!Serial);
  Serial.begin(115200);
}

void setupDisplay() {
  lcd.begin(16, 2);
}

void setupSwitch() {
  struct Config {
    byte pin;
    Expander *expander;
    void (*action)(bool state);
  } buttons[COUNT_SWITCHES] = {
    {5, NULL, &SwitchJoystickD5},
    {7, NULL, &SwitchJoystickD7},
    {8, NULL, &SwitchJoystickD8},
    {9, NULL, &SwitchJoystickD9},
    {0, &expander1, &SwitchArcadeE1D0},
    {2, &expander1, &SwitchArcadeE1D2},
    {4, &expander1, &SwitchArcadeE1D4},
    {6, &expander1, &SwitchArcadeE1D6},
    {0, &expander2, &SwitchArcadeE2D0},
    {2, &expander2, &SwitchArcadeE2D2}
  };

  for (byte i = 0; i < COUNT_SWITCHES; i++) {
    Config &config = buttons[i];
    Button button;

    if (config.expander != NULL) {
      button.begin(config.pin, config.expander);
    } else {
      button.begin(config.pin);
    }
    button.onClick = *config.action;
    buttonArray.append(button);
  }
}

void setupAxis() {
  struct Config {
    byte pin;
    void (*action)(int key);
  } Axises[COUNT_AXLES] = {
    {A0, &AxisA0},
    {A1, &AxisA1},
    {A2, &AxisA3},
    {A3, &AxisA4},
    {A4, &AxisA7},
    {A5, &AxisA6},
    {A6, &AxisA2},
    {A7, &AxisA5}
  };

  for (byte i = 0; i < COUNT_AXLES; i++) {
    Config &config = Axises[i];
    Axis axis = Axis(RANGE_AXIS, RESPONSE_DELAY);
    axis.begin(config.pin, *config.action);
    axisArray.append(axis);
  }
}

void inspectIgnition() {
  ignition.refresh();
}

void inspectExpander() {
  expander1.refresh();
  expander2.refresh();
}

void inspectSwitch() {
  buttonArray.refresh();
}

void inspectAxis() {
  axisArray.refresh();
}

void ignitionOnChangeStatus() {
  lcd.display("Wlacz zaplon!");
}

bool checkDisplayUpdate() {
  unsigned long currentMillis = millis();
  if ((currentMillis - lastDisplayMillis) > DISPLAY_DELAY) {
    lastDisplayMillis = currentMillis;
    return true;
  }
  return false;
}

void AxisA0(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks1;
  char key;

  switch (simulator) {
    case EXCAVATORS:
      key = reading > 0 ? shortcut.KEY_JOYSTICK_RIGHT : shortcut.KEY_JOYSTICK_LEFT;

      if (buttonAxisA1IsActive) {
        append(KEY_LEFT_CTRL, false, debug);
      }
      append(key, true, debug);
      break;
    case FARMING:
      key = reading > 0 ? shortcut.KEY_JOYSTICK_LEFT : shortcut.KEY_JOYSTICK_RIGHT;
      append(key, true, debug);
      break;
  }
}

void AxisA1(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks1;

  switch (simulator) {
    case EXCAVATORS:
    case FARMING:
      char key = reading > 0 ? shortcut.KEY_JOYSTICK_UP : shortcut.KEY_JOYSTICK_DOWN;
      append(key, true, debug);
      break;
  }
}

void AxisA2(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks2;

  if (simulator == EXCAVATORS) {
    char key = reading > 0 ? shortcut.KEY_JOYSTICK_UP : shortcut.KEY_JOYSTICK_DOWN;

    if (buttonAxisA2IsActive) {
      key = reading > 0 ? shortcut.KEY_JOYSTICK_LEFT : shortcut.KEY_JOYSTICK_RIGHT;
    }

    append(key, true, debug);
    return;
  }
}

void AxisA3(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks2;
  //append('x', debug);
}

void AxisA4(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks3;

  if (simulator == EXCAVATORS) {
    char key = reading > 0 ? shortcut.KEY_JOYSTICK_UP : shortcut.KEY_JOYSTICK_DOWN;
    append(key, true, debug);
    return;
  }
}

void AxisA5(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks3;
  //append('x', debug);
}

void AxisA6(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks4;

  if (simulator == EXCAVATORS) {
    char key = reading > 0 ? shortcut.KEY_JOYSTICK_UP : shortcut.KEY_JOYSTICK_DOWN;
    append(key, true, {__PRETTY_FUNCTION__, reading});
    return;
  }
}

void AxisA7(int reading) {
  DebugAxis debug = {__PRETTY_FUNCTION__, reading};
  ShortcutsAxis shortcut = Mapping[simulator].Joysticks4;

  if (simulator == EXCAVATORS) {
    char key = reading > 0 ? shortcut.KEY_JOYSTICK_LEFT : shortcut.KEY_JOYSTICK_RIGHT;
    append(key, true, debug);
    return;
  }
}

void SwitchJoystickD5(bool state) {
  buttonAxisA2IsActive = state;
}

void SwitchJoystickD7(bool state) {
  buttonAxisA1IsActive = state;
}

void SwitchJoystickD8(bool state) {
  //Serial.print("SwitchD8: ");
  //Serial.println(state);
}

void SwitchJoystickD9(bool state) {
  //Serial.print("SwitchD9: ");
  //Serial.println(state);
}

void SwitchArcadeE1D0(bool state) {
  //button.expander->blinkAsync(button.pin + 1, 2, 1000UL);
  //button.expander->toggleAsync(button.pin + 1);

  expander1.blinkAsync(1, 2, 1000UL);

  //Serial.print("SwitchArcadeE1D0: ");
  //Serial.println(state);
}

void SwitchArcadeE1D2(bool state) {
  expander1.toggleAsync(3);

  //Serial.print("SwitchArcadeE1D2: ");
  //Serial.println(state);
}

void SwitchArcadeE1D4(bool state) {
  expander1.blinkAsync(5, 2, 1000UL);

  //Serial.print("SwitchArcadeE1D4: ");
  //Serial.println(state);
}

void SwitchArcadeE1D6(bool state) {
  expander1.toggleAsync(7);

  //Serial.print("SwitchArcadeE1D6: ");
  //Serial.println(state);
}

void SwitchArcadeE2D0(bool state) {
  expander2.toggleAsync(1);

  //Serial.print("SwitchArcadeE2D0: ");
  //Serial.println(state);
}

void SwitchArcadeE2D2(bool state) {
  expander2.toggleAsync(3);

  //Serial.print("SwitchArcadeE2D2: ");
  //Serial.println(state);
}

void append(char key, bool push, DebugAxis debug) {
  if (!DEBUG) {
    Keyboard.press(key);

    if (push) {
      //delay(100);
      Keyboard.releaseAll();
    }
    return;
  }

  Serial.print(debug.axisName);
  Serial.print(", value: ");
  Serial.print(debug.axisValue);
  Serial.print(", key: ");
  Serial.println(key);
}

