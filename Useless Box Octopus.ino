/*******************************************************************************************
 * Angry Octopus - Modular Personality Robot
 * With Servos + WS2812 LEDs + Vibration Motor + Piezo Buzzer
 *
 * Board: Arduino UNO (or compatible)
 * 
 * Features:
 *  - 5 switches → 5 arm servos
 *  - 1 door servo
 *  - Personality engine (CALM → CURIOUS → ANNOYED → ANGRY)
 *  - Weighted random behaviors
 *  - LEDs (2 eyes + 3 glow) for emotional feedback
 *  - Vibration motor for physical “shakes”
 *  - Piezo buzzer for tones and sound effects
 *
 *******************************************************************************************/

#include <Servo.h>
#include <Adafruit_NeoPixel.h>

/*******************************************************************************************
 * CONFIGURATION CONSTANTS
 * Pin mapping, thresholds, and hardware definitions
 *******************************************************************************************/
const uint8_t PIN_SW[5]  = {12, 10, 8, 6, 4}; // 5 switches
const uint8_t PIN_ARM[5] = {11, 9, 7, 5, 3};  // 5 arm servos
const uint8_t PIN_DOOR   = 13;                // door servo
const uint8_t PIN_WS281X = A0;                // WS2812 data pin
const uint8_t PIN_VIB    = 2;                 // vibration motor breakout IN
const uint8_t PIN_BUZZ   = A1;                // piezo buzzer pin

const int DOOR_OPEN_DEG  = 140;               // door angle open
const int ARM_HOME_DEG   = 80;                // resting arm position
const int ARM_MAX_DEG    = 180;               // max safe servo angle
const int ARM_MIN_DEG    = 0;                 // min servo angle

const uint8_t NUM_ARMS   = 5;
const uint8_t NUM_LEDS   = 5;                 // 2 eyes + 3 base glow

const uint8_t LED_BRIGHT = 60;                // brightness cap for LEDs

// Mood engine tuning
const unsigned long DEBOUNCE_MS = 120;
const unsigned long MOOD_TICK_MS= 500;
const int MOOD_MIN=0, MOOD_MAX=100;
const int MOOD_UP=15, MOOD_DN=3;

/*******************************************************************************************
 * SMALL HELPER FUNCTIONS
 *******************************************************************************************/
inline int clampDeg(int d) { return (d<0)?0:(d>180?180:d); } // clamp servo angle
inline int irand(int a,int b){return random(a,b+1);}         // inclusive random

/*******************************************************************************************
 * BUTTON CLASS - handles INPUT_PULLUP + debouncing
 *******************************************************************************************/
class Button {
public:
  Button(uint8_t pin):_pin(pin),_lastMs(0){}
  void begin(){ pinMode(_pin, INPUT_PULLUP);}              // configure as input
  bool pressed(){
    if(digitalRead(_pin)==LOW){                            // active low
      unsigned long now=millis();
      if(now-_lastMs>DEBOUNCE_MS){                         // debounce
        _lastMs=now;
        while(digitalRead(_pin)==LOW) delay(5);            // wait for release
        return true;
      }
    }
    return false;
  }
private:
  uint8_t _pin;
  unsigned long _lastMs;
};

/*******************************************************************************************
 * SERVO WRAPPER CLASSES (Door + Arm) - add easing and home positions
 *******************************************************************************************/
class Door {
public:
  Door(uint8_t pin):_pin(pin){}
  void begin(){_servo.attach(_pin);_servo.write(0);}       // attach + closed
  void write(int deg){_servo.write(clampDeg(deg));}        // write bounded
  int read()const{return _servo.read();}
  void easeTo(int target,int steps,int baseDelay){
    int start=read();
    for(int i=0;i<=steps;i++){
      float t=(float)i/steps;
      float e=(t<0.5f)?2*t*t:-1+(4-2*t)*t;                 // ease curve
      int pos=start+(int)((target-start)*e);
      write(pos);
      delay(baseDelay+irand(0,3));
    }
  }
private:
  uint8_t _pin;
  Servo _servo;
};

class Arm {
public:
  Arm(uint8_t pin):_pin(pin){}
  void begin(){_servo.attach(_pin);home();}
  void home(){_servo.write(ARM_HOME_DEG);}
  void write(int deg){_servo.write(clampDeg(deg));}
  int read()const{return _servo.read();}
  void easeTo(int target,int steps,int baseDelay){
    int start=read();
    for(int i=0;i<=steps;i++){
      float t=(float)i/steps;
      float e=(t<0.5f)?2*t*t:-1+(4-2*t)*t;
      int pos=start+(int)((target-start)*e);
      write(pos);
      delay(baseDelay+irand(0,3));
    }
  }
private:
  uint8_t _pin;
  Servo _servo;
};

/*******************************************************************************************
 * HAPTICS CLASS - vibration motor on breakout board
 *******************************************************************************************/
class Haptics {
public:
  Haptics(uint8_t pin):_pin(pin){}
  void begin(){ pinMode(_pin, OUTPUT); off();}
  void on(){ digitalWrite(_pin,HIGH);}
  void off(){digitalWrite(_pin,LOW);}
  void buzz(int ms){on();delay(ms);off();}
private:
  uint8_t _pin;
};

/*******************************************************************************************
 * BUZZER CLASS - piezo speaker with simple tones
 *******************************************************************************************/
class Buzzer {
public:
  Buzzer(uint8_t pin):_pin(pin){}
  void begin(){ pinMode(_pin, OUTPUT); noTone(_pin);}
  void beep(int freq,int ms){ tone(_pin,freq,ms); delay(ms);}
  void angry(){ beep(200,150); beep(150,200);}
  void curious(){ beep(800,80); delay(50); beep(900,80);}
  void annoyed(){ beep(400,100);}
  void calm(){ beep(1000,50);}
private:
  uint8_t _pin;
};

/*******************************************************************************************
 * LED CLASS - eyes (2) + glow (3) with mood-based colors
 *******************************************************************************************/
class EyesAndGlow {
public:
  EyesAndGlow(uint8_t pin,uint16_t count):_strip(count,pin,NEO_GRB+NEO_KHZ800){}
  void begin(){_strip.begin();_strip.setBrightness(LED_BRIGHT);setAll(0,0,0);}
  void setAll(uint8_t r,uint8_t g,uint8_t b){
    for(uint16_t i=0;i<_strip.numPixels();i++)
      _strip.setPixelColor(i,_strip.Color(r,g,b));
    _strip.show();
  }
  void setEyes(uint8_t r,uint8_t g,uint8_t b){
    _strip.setPixelColor(0,_strip.Color(r,g,b));
    _strip.setPixelColor(1,_strip.Color(r,g,b));
    _strip.show();
  }
  void setGlow(uint8_t r,uint8_t g,uint8_t b){
    for(int i=2;i<5;i++) _strip.setPixelColor(i,_strip.Color(r,g,b));
    _strip.show();
  }
  void blinkEyes(){setEyes(0,0,0);delay(100);setEyes(255,255,255);}
private:
  Adafruit_NeoPixel _strip;
};

/*******************************************************************************************
 * MOOD ENGINE - tracks “personality” state based on interaction frequency
 *******************************************************************************************/
enum Mood:uint8_t {CALM,CURIOUS,ANNOYED,ANGRY};
class MoodEngine {
public:
  MoodEngine():_score(0),_lastTick(0){}
  void reset(){_score=0;_lastTick=millis();}
  void poke(){_score=min(MOOD_MAX,_score+MOOD_UP);}
  void update(){
    unsigned long now=millis();
    if(now-_lastTick>=MOOD_TICK_MS){
      _lastTick=now;
      _score=max(MOOD_MIN,_score-MOOD_DN);
    }
  }
  Mood state()const{
    if(_score<20)return CALM;
    if(_score<45)return CURIOUS;
    if(_score<75)return ANNOYED;
    return ANGRY;
  }
private:
  uint8_t _score;
  unsigned long _lastTick;
};

/*******************************************************************************************
 * BEHAVIORS - different “moves” the octopus can perform
 *******************************************************************************************/
class Behavior{public:virtual~Behavior(){}virtual void run(uint8_t i)=0;};

// simple push
class SimplePush:public Behavior{
public:void run(uint8_t i)override{
  gDoor.easeTo(DOOR_OPEN_DEG,40,12);
  gArms[i].easeTo(ARM_MAX_DEG,35,8);
  delay(100);
  gArms[i].easeTo(ARM_MIN_DEG,35,10);
  gArms[i].easeTo(ARM_HOME_DEG,15,7);
  gDoor.easeTo(0,40,9);
}};

// slamdoor
class SlamDoor:public Behavior{
public:void run(uint8_t i)override{
  gDoor.easeTo(DOOR_OPEN_DEG,22,6);
  gArms[i].easeTo(ARM_MAX_DEG,18,5);
  delay(50);
  gArms[i].easeTo(ARM_HOME_DEG,16,5);
  gHaptics.buzz(400);
  gBuzzer.angry();
  gDoor.write(0);
}};

/*******************************************************************************************
 * GLOBAL OBJECTS
 *******************************************************************************************/
Door gDoor(PIN_DOOR);
Arm gArms[NUM_ARMS]={Arm(PIN_ARM[0]),Arm(PIN_ARM[1]),Arm(PIN_ARM[2]),Arm(PIN_ARM[3]),Arm(PIN_ARM[4])};
Button gBtns[NUM_ARMS]={Button(PIN_SW[0]),Button(PIN_SW[1]),Button(PIN_SW[2]),Button(PIN_SW[3]),Button(PIN_SW[4])};
Haptics gHaptics(PIN_VIB);
Buzzer gBuzzer(PIN_BUZZ);
EyesAndGlow gLEDs(PIN_WS281X,NUM_LEDS);
MoodEngine gMood;

SimplePush bSimple;
SlamDoor   bSlam;

/*******************************************************************************************
 * SETUP
 *******************************************************************************************/
void setup(){
  randomSeed(analogRead(A2));            // RNG seed
  for(int i=0;i<NUM_ARMS;i++){gBtns[i].begin();gArms[i].begin();}
  gDoor.begin();
  gHaptics.begin();
  gBuzzer.begin();
  gLEDs.begin();
  gMood.reset();
}

/*******************************************************************************************
 * LOOP
 *******************************************************************************************/
void loop(){
  gMood.update();                        // decay mood
  for(uint8_t i=0;i<NUM_ARMS;i++){
    if(gBtns[i].pressed()){
      gMood.poke();
      Mood m=gMood.state();
      if(m==CALM) { bSimple.run(i); gBuzzer.calm();}
      if(m==CURIOUS){ bSimple.run(i); gBuzzer.curious();}
      if(m==ANNOYED){ bSimple.run(i); gBuzzer.annoyed();}
      if(m==ANGRY){ bSlam.run(i);}      // slam when angry
    }
  }
}
