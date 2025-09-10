#include "Buzzer.h"
Buzzer::Buzzer(uint8_t pin):_pin(pin){}
void Buzzer::begin(){pinMode(_pin,OUTPUT);noTone(_pin);}
void Buzzer::beep(int freq,int ms){tone(_pin,freq,ms);delay(ms);}
void Buzzer::angry(){beep(200,150);beep(150,200);}
void Buzzer::curious(){beep(800,80);delay(50);beep(900,80);}
void Buzzer::annoyed(){beep(400,100);}
void Buzzer::calm(){beep(1000,50);}
