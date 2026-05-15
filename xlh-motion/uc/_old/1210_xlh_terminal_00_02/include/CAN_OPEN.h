#ifndef _CAN_OPEN_H_
#define _CAN_OPEN_H_

#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN_BASE.h"

typedef struct
{
  uint8_t abyRxData1[8];
  uint8_t abyRxData2[8];
  uint8_t abyRxData3[8];
  uint8_t abyRxData4[8];  
} s_in;

typedef struct
{
  uint8_t abyTxData1[8];
  uint8_t abyTxData2[8];
  uint8_t abyTxData3[8];
  uint8_t abyTxData4[8];
} s_out;

class CAN_OPEN : public CAN_OPEN_BASE
{
public:
  u_int16_t dummy;
  CAN_OPEN(void);
  void setup(uint8_t addr_co_eeprom);
  // These shadow the same-named CAN_OPEN_BASE methods. The CAN ISR calls
  // them on a CAN_OPEN instance directly, so dispatch is static (no vtable).
  void rx_pdo_1(twai_message_t *msg_rx);
  void rx_pdo_2(twai_message_t *msg_rx);
  void rx_pdo_3(twai_message_t *msg_rx);
  void rx_pdo_4(twai_message_t *msg_rx);
  void tx_pdo_1(void);
  void tx_pdo_2(void);
  void tx_pdo_3(void);
  void tx_pdo_4(void);
  void reset_output(void);
  void cyclic_isr_rx(void);
  void cyclic_isr_tx(void);
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