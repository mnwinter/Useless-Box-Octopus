#ifndef DOOR_H
#define DOOR_H
#include <Arduino.h>
#include <Servo.h>

class Door{
public:
  Door(uint8_t pin);
  void begin();
  void write(int deg);
  int read()const;
  void easeTo(int target,int steps,int baseDelay);
private:
  uint8_t _pin;
  Servo _servo;
};
#endif
