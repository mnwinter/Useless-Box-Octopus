#ifndef HAPTICS_H
#define HAPTICS_H
#include <Arduino.h>

class Haptics{
public:
  Haptics(uint8_t pin);
  void begin();
  void on();
  void off();
  void buzz(int ms);
private:
  uint8_t _pin;
};
#endif
