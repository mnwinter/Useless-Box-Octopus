#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

class Buzzer{
public:
  Buzzer(uint8_t pin);
  void begin();
  void beep(int freq,int ms);
  void angry();
  void curious();
  void annoyed();
  void calm();
private:
  uint8_t _pin;
};
#endif
