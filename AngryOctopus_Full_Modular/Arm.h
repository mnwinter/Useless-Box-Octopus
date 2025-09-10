#ifndef ARM_H
#define ARM_H
#include <Arduino.h>
#include <Servo.h>

class Arm{
public:
  Arm(uint8_t pin);
  void begin();
  void home();
  void write(int deg);
  int read()const;
  void easeTo(int target,int steps,int baseDelay);
private:
  uint8_t _pin;
  Servo _servo;
};
#endif
