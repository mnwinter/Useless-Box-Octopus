#ifndef SIMPLEPUSH_H
#define SIMPLEPUSH_H
#include "Behavior.h"
#include "Door.h"
#include "Arm.h"
extern Door gDoor;
extern Arm gArms[5];
class SimplePush:public Behavior{
public:
  void run(uint8_t i)override;
};
#endif
