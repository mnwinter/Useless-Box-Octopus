#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

// Button class handles debouncing of INPUT_PULLUP switches
class Button {
public:
  Button(uint8_t pin);    // constructor
  void begin();           // configure pin
  bool pressed();         // returns true once per press
private:
  uint8_t _pin;           // pin number
  unsigned long _lastMs;  // last press timestamp
};
#endif
