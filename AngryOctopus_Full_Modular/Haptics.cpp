#include "Haptics.h"
Haptics::Haptics(uint8_t pin):_pin(pin){}
void Haptics::begin(){pinMode(_pin,OUTPUT);off();}
void Haptics::on(){digitalWrite(_pin,HIGH);}
void Haptics::off(){digitalWrite(_pin,LOW);}
void Haptics::buzz(int ms){on();delay(ms);off();}
