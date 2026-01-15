#ifndef _CAN_OPEN_H_
#define _CAN_OPEN_H_

#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN_BASE.h"

typedef struct
{
  uint8_t byDxl1Torque;
  uint8_t byDxl2Torque;
  uint8_t byDxl1Led;
  uint8_t byDxl2Led;
  uint8_t byDxl1Control;
  uint8_t byDxl2Control;
  uint16_t uiDxl1Position;
  uint16_t uiDxl2Position;
  uint16_t uiDxl1Velocity;
  uint16_t uiDxl2Velocity;
  uint8_t byRgbRed;
  uint8_t byRgbGreen;
  uint8_t byRgbBlue;
} s_in;

typedef struct
{
  uint16_t uiDxl1Position;
  uint16_t uiDxl2Position;
  int16_t iDxl1Current;
  int16_t iDxl2Current;
} s_out;

class CAN_OPEN : public CAN_OPEN_BASE
{
public:
  CAN_OPEN(void);
  void setup(uint8_t addr_co_eeprom);
  void rx_pdo_1(twai_message_t *msg_rx) override;
  void rx_pdo_2(twai_message_t *msg_rx) override;
  void rx_pdo_3(twai_message_t *msg_rx) override;
  void tx_pdo_1(void) override;
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
