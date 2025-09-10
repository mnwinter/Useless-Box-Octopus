#include "Button.h"
const unsigned long DEBOUNCE_MS=120;
Button::Button(uint8_t pin):_pin(pin),_lastMs(0){}
void Button::begin(){pinMode(_pin,INPUT_PULLUP);}
bool Button::pressed(){if(digitalRead(_pin)==LOW){unsigned long now=millis();if(now-_lastMs>DEBOUNCE_MS){_lastMs=now;while(digitalRead(_pin)==LOW)delay(5);return true;}}return false;}
