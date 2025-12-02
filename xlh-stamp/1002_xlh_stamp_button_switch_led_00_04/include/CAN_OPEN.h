#ifndef _CAN_OPEN_H_
#define _CAN_OPEN_H_

#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN_BASE.h"

typedef struct
{
  uint8_t byLedsButton;
  uint8_t byLedsSwitch;
} s_in;

typedef struct
{
  uint8_t byInputsButton;
  uint8_t byInputsSwitch;
} s_out;

class CAN_OPEN: public CAN_OPEN_BASE
{
public:
  CAN_OPEN(void);
  void setup(uint8_t addr_co_eeprom);
  void rx_pdo_1(twai_message_t *msg_rx) override;
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
