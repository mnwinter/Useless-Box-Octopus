#include "EyesAndGlow.h"
EyesAndGlow::EyesAndGlow(uint8_t pin,uint16_t count):_strip(count,pin,NEO_GRB+NEO_KHZ800){}
void EyesAndGlow::begin(){_strip.begin();_strip.setBrightness(60);setAll(0,0,0);}
void EyesAndGlow::setAll(uint8_t r,uint8_t g,uint8_t b){for(uint16_t i=0;i<_strip.numPixels();i++)_strip.setPixelColor(i,_strip.Color(r,g,b));_strip.show();}
void EyesAndGlow::setEyes(uint8_t r,uint8_t g,uint8_t b){_strip.setPixelColor(0,_strip.Color(r,g,b));_strip.setPixelColor(1,_strip.Color(r,g,b));_strip.show();}
void EyesAndGlow::setGlow(uint8_t r,uint8_t g,uint8_t b){for(int i=2;i<5;i++)_strip.setPixelColor(i,_strip.Color(r,g,b));_strip.show();}
void EyesAndGlow::blinkEyes(){setEyes(0,0,0);delay(100);setEyes(255,255,255);}
