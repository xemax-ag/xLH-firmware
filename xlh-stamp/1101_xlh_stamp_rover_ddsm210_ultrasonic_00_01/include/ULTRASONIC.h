#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include <Arduino.h>

class ULTRASONIC
{
public:
  u_int16_t dummy;
  ULTRASONIC(void);
  void setup(void);
  void pcaselect(uint8_t i);
  void loop(void);
  
private:
  uint8_t init_done;
  uint8_t sonic_device;
};

extern ULTRASONIC ultrasonic;

#endif