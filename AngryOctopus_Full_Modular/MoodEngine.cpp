#include "MoodEngine.h"
const int MOOD_MIN=0,MOOD_MAX=100,MOOD_UP=15,MOOD_DN=3;const unsigned long MOOD_TICK_MS=500;
MoodEngine::MoodEngine():_score(0),_lastTick(0){}
void MoodEngine::reset(){_score=0;_lastTick=millis();}
void MoodEngine::poke(){_score=min(MOOD_MAX,_score+MOOD_UP);}
void MoodEngine::update(){unsigned long now=millis();if(now-_lastTick>=MOOD_TICK_MS){_lastTick=now;_score=max(MOOD_MIN,_score-MOOD_DN);}}
Mood MoodEngine::state()const{if(_score<20)return CALM;if(_score<45)return CURIOUS;if(_score<75)return ANNOYED;return ANGRY;}
