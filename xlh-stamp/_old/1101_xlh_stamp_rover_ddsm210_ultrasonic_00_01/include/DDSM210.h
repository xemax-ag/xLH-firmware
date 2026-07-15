#ifndef _DDSM210_H_
#define _DDSM210_H_

#include <Arduino.h>

class DDSM210
{
public:
  u_int16_t dummy;
  DDSM210(void);
  void setup(void);
  void loop(void);

private:
  uint8_t init_done;
  uint8_t byErrorCode1;
  uint8_t byErrorCode2;
  int32_t diDdsm210Mileage;
  int32_t diDdsm210Position;
  int32_t diDdsm210PositionOld;
  int32_t diPosition;
};

extern DDSM210 ddsm210;

#endif