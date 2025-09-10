#ifndef MOODENGINE_H
#define MOODENGINE_H
#include <Arduino.h>
enum Mood:uint8_t{CALM,CURIOUS,ANNOYED,ANGRY};
class MoodEngine{
public:
  MoodEngine();
  void reset();
  void poke();
  void update();
  Mood state()const;
private:
  uint8_t _score;
  unsigned long _lastTick;
};
#endif
