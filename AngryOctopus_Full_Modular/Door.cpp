#include "Door.h"
static inline int clampDeg(int d){return (d<0)?0:(d>180?180:d);}
Door::Door(uint8_t pin):_pin(pin){}
void Door::begin(){_servo.attach(_pin);_servo.write(0);}
void Door::write(int deg){_servo.write(clampDeg(deg));}
int Door::read()const{return _servo.read();}
void Door::easeTo(int target,int steps,int baseDelay){int start=read();for(int i=0;i<=steps;++i){float t=(float)i/steps;float e=(t<0.5f)?2*t*t:-1+(4-2*t)*t;int pos=start+(int)((target-start)*e);write(pos);delay(baseDelay+random(0,3));}}
