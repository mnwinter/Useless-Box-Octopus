#ifndef BEHAVIOR_H
#define BEHAVIOR_H
class Behavior{
public:
  virtual ~Behavior(){}
  virtual void run(uint8_t i)=0;
};
#endif
