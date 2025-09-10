/*******************************************************************************************
 * Angry Octopus - Modular Personality Robot
 * Full project with OOP modular design
 * Includes: Servos, Switches, Mood Engine, LEDs, Vibration Motor, Piezo Buzzer
 *******************************************************************************************/

#include "Button.h"
#include "Door.h"
#include "Arm.h"
#include "Haptics.h"
#include "Buzzer.h"
#include "EyesAndGlow.h"
#include "MoodEngine.h"
#include "Behavior.h"
#include "SimplePush.h"
#include "SlamDoor.h"

// Pin assignments
const uint8_t PIN_SW[5]  = {12, 10, 8, 6, 4};  // switches
const uint8_t PIN_ARM[5] = {11, 9, 7, 5, 3};   // arm servos
const uint8_t PIN_DOOR   = 13;                 // door servo
const uint8_t PIN_WS281X = A0;                 // LEDs
const uint8_t PIN_VIB    = 2;                  // vibration motor
const uint8_t PIN_BUZZ   = A1;                 // buzzer

const uint8_t NUM_ARMS   = 5;
const uint8_t NUM_LEDS   = 5;

// Instantiate global subsystems
Door gDoor(PIN_DOOR);
Arm gArms[NUM_ARMS] = {Arm(PIN_ARM[0]), Arm(PIN_ARM[1]), Arm(PIN_ARM[2]), Arm(PIN_ARM[3]), Arm(PIN_ARM[4])};
Button gBtns[NUM_ARMS] = {Button(PIN_SW[0]), Button(PIN_SW[1]), Button(PIN_SW[2]), Button(PIN_SW[3]), Button(PIN_SW[4])};
Haptics gHaptics(PIN_VIB);
Buzzer gBuzzer(PIN_BUZZ);
EyesAndGlow gLEDs(PIN_WS281X, NUM_LEDS);
MoodEngine gMood;

// Behaviors
SimplePush bSimple;
SlamDoor   bSlam;

void setup() {
  randomSeed(analogRead(A2)); // seed RNG
  for (int i = 0; i < NUM_ARMS; i++) { gBtns[i].begin(); gArms[i].begin(); }
  gDoor.begin();
  gHaptics.begin();
  gBuzzer.begin();
  gLEDs.begin();
  gMood.reset();
}

void loop() {
  gMood.update();
  for (uint8_t i = 0; i < NUM_ARMS; i++) {
    if (gBtns[i].pressed()) {
      gMood.poke();
      Mood m = gMood.state();
      if (m == CALM)    { bSimple.run(i); gBuzzer.calm(); }
      if (m == CURIOUS) { bSimple.run(i); gBuzzer.curious(); }
      if (m == ANNOYED) { bSimple.run(i); gBuzzer.annoyed(); }
      if (m == ANGRY)   { bSlam.run(i); }
    }
  }
}
