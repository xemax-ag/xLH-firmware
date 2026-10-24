#ifndef _IMU_H_
#define _IMU_H_

#include <Arduino.h>
#include "CAN_OPEN.h"

class IMU
{
public:
  u_int16_t dummy;
  IMU(void);
  void setup(void);
  void read_bn085(void);
  void loop(void);
  uint8_t read_sensor;
  
private:
  uint8_t init_done;
  s_out_imu outImu;
};

extern IMU imu;

#endif