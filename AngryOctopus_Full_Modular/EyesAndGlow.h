#ifndef EYESANDGLOW_H
#define EYESANDGLOW_H
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class EyesAndGlow{
public:
  EyesAndGlow(uint8_t pin,uint16_t count);
  void begin();
  void setAll(uint8_t r,uint8_t g,uint8_t b);
  void setEyes(uint8_t r,uint8_t g,uint8_t b);
  void setGlow(uint8_t r,uint8_t g,uint8_t b);
  void blinkEyes();
private:
  Adafruit_NeoPixel _strip;
};
#endif
