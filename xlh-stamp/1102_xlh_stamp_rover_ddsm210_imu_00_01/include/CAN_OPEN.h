#ifndef _CAN_OPEN_H_
#define _CAN_OPEN_H_

#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN_BASE.h"

typedef struct
{
  int16_t iSetSpeed;
    int16_t iSetCurrentLimit;
  uint8_t bySetAccTime;
  uint8_t bySetCmd;

} s_in;

typedef struct
{
  int16_t iSpeed;
  int16_t iCurrent;
  uint8_t byAccTime;
  uint8_t byTemperature;
  int32_t diPosition;
  // int32_t diMileage;
  // uint16_t uiPosition;
  uint16_t uiErrorCode;
  int16_t iYaw;
  int16_t iPitch;
  int16_t iRoll;
  float fYaw;
  float fPitch;
  float fRoll;
  int16_t debug_1;
  int16_t debug_2;

} s_out;

class CAN_OPEN : public CAN_OPEN_BASE
{
public:
  u_int16_t dummy;
  CAN_OPEN(void);
  void setup(uint8_t addr_co_eeprom);
  void rx_pdo_1(twai_message_t *msg_rx) override;
  void tx_pdo_1(void) override;
  void tx_pdo_2(void) override;
  void tx_pdo_3(void) override;
  void reset_output(void) override;
  void loop(void);
  s_in in;
  s_out out;
  s_out out_old;

private:
  uint8_t usr_btn_old;
  uint8_t addr_co_eeprom;
};

extern CAN_OPEN can_open;

#endif