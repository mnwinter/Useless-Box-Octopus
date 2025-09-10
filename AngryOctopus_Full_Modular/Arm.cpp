#include "Arm.h"
static inline int clampDeg(int d){return (d<0)?0:(d>180?180:d);}
const int ARM_HOME_DEG=80;
Arm::Arm(uint8_t pin):_pin(pin){}
void Arm::begin(){_servo.attach(_pin);home();}
void Arm::home(){_servo.write(ARM_HOME_DEG);}
void Arm::write(int deg){_servo.write(clampDeg(deg));}
int Arm::read()const{return _servo.read();}
void Arm::easeTo(int target,int steps,int baseDelay){int start=read();for(int i=0;i<=steps;++i){float t=(float)i/steps;float e=(t<0.5f)?2*t*t:-1+(4-2*t)*t;int pos=start+(int)((target-start)*e);write(pos);delay(baseDelay+random(0,3));}}
