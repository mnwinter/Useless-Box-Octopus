#include "SlamDoor.h"
void SlamDoor::run(uint8_t i){gDoor.easeTo(140,22,6);gArms[i].easeTo(180,18,5);delay(50);gArms[i].easeTo(80,16,5);gHaptics.buzz(400);gBuzzer.angry();gDoor.write(0);}
