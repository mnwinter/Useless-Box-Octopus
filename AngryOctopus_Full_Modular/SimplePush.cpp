#include "SimplePush.h"
void SimplePush::run(uint8_t i){gDoor.easeTo(140,40,12);gArms[i].easeTo(180,35,8);delay(100);gArms[i].easeTo(0,35,10);gArms[i].easeTo(80,15,7);gDoor.easeTo(0,40,9);}
