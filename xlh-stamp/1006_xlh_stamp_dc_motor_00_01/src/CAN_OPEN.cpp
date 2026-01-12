#include <stdint.h>
#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "CONFIG.h"
#include <EEPROM.h>


CAN_OPEN::CAN_OPEN(void): CAN_OPEN_BASE() { ; }

void CAN_OPEN::setup(uint8_t addr_co_eeprom)
{
  this->addr_co_eeprom = addr_co_eeprom;
  this->node_id = uint16_t(EEPROM.read(this->addr_co_eeprom));
    if (node_id > 16) this->node_id = 16;


  CAN_OPEN_BASE::setup();
}

void CAN_OPEN::rx_pdo_1(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_1(msg_rx);
  can_msg_data_int_to_byte canMsgDataIntToByte;
  if ((msg_rx->identifier == this->pdo_rx_1_id) && (msg_rx->data_length_code == 3))
  {
    this->in.byControl = msg_rx->data[0];

    canMsgDataIntToByte.abyValue[0] = msg_rx->data[1];
    canMsgDataIntToByte.abyValue[1] = msg_rx->data[2];
    this->in.uiDutyCycle = canMsgDataIntToByte.uiValue;
  }
}

void CAN_OPEN::tx_pdo_1(void)
{
  CAN_OPEN_BASE::tx_pdo_1();
  if (this->out.diPosition != this->out_old.diPosition)
    this->pdo_tx_1_send_msg = 1;
  if (this->out.iSpeed != this->out_old.iSpeed)
    this->pdo_tx_1_send_msg = 1;
  if (this->out.uiCurrent != this->out_old.uiCurrent)
    this->pdo_tx_1_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_1_send_msg == 1))
  {
    twai_message_t tx_frame;
    can_msg_data_int_to_byte canMsgDataIntToByte;
    can_msg_data_dint_to_byte canMsgDataDintToByte;
    
    this->pdo_tx_1_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_1_id;

    canMsgDataDintToByte.diValue = this->out.diPosition;
    tx_frame.data[0] = canMsgDataDintToByte.abyValue[0];
    tx_frame.data[1] = canMsgDataDintToByte.abyValue[1];
    tx_frame.data[2] = canMsgDataDintToByte.abyValue[2];
    tx_frame.data[3] = canMsgDataDintToByte.abyValue[3];

    canMsgDataIntToByte.iValue = this->out.iSpeed;
    tx_frame.data[4] = canMsgDataIntToByte.abyValue[0];
    tx_frame.data[5] = canMsgDataIntToByte.abyValue[1];

    canMsgDataIntToByte.uiValue = this->out.uiCurrent;
    tx_frame.data[6] = canMsgDataIntToByte.abyValue[0];
    tx_frame.data[7] = canMsgDataIntToByte.abyValue[1];

    ESP32Can.writeFrame(&tx_frame);
    this->out_old.diPosition = this->out.diPosition;
    this->out_old.iSpeed = this->out.iSpeed;
    this->out_old.uiCurrent = this->out.uiCurrent;
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();
  this->in.byControl = 0;
  this->in.uiDutyCycle = 0;
}

void CAN_OPEN::loop(void){
  // USER BTN
  if ((digitalRead(GPIO_TOUCH_BTN) == 0) && (digitalRead(GPIO_TOUCH_BTN) != this->usr_btn_old))
  {
      this->node_id++;
      if(this->node_id > 16) this->node_id = 1;
      EEPROM.write(this->addr_co_eeprom, uint8_t(this->node_id));
      EEPROM.commit();
      CAN_OPEN_BASE::ids();
      this->node_guard_state = NODE_GUARD_STATE_STOPPED;
      CAN_OPEN_BASE::bootup();
  }
  this->usr_btn_old = digitalRead(GPIO_TOUCH_BTN);
}

CAN_OPEN can_open;