#ifndef SLAMDOOR_H
#define SLAMDOOR_H
#include "Behavior.h"
#include "Door.h"
#include "Arm.h"
#include "Haptics.h"
#include "Buzzer.h"
extern Door gDoor;
extern Arm gArms[5];
extern Haptics gHaptics;
extern Buzzer gBuzzer;
class SlamDoor:public Behavior{
public:
  void run(uint8_t i)override;
};
#endif
