#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include <Arduino.h>

class ULTRASONIC
{
public:
  u_int16_t dummy;
  ULTRASONIC(void);
  void setup(void);
  void loop(void);
  
private:
  uint8_t init_done;
};

extern ULTRASONIC ultrasonic;

#endif