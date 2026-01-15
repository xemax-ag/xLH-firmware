#include <stdint.h>
#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "CONFIG.h"
#include <EEPROM.h>

CAN_OPEN::CAN_OPEN(void) : CAN_OPEN_BASE() { ; }

void CAN_OPEN::setup(uint8_t addr_co_eeprom)
{
  this->addr_co_eeprom = addr_co_eeprom;
  this->node_id = uint16_t(EEPROM.read(this->addr_co_eeprom));
  if (node_id > 16)
    this->node_id = 16;
  CAN_OPEN_BASE::setup();
}

void CAN_OPEN::rx_pdo_1(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_1(msg_rx);
  can_msg_data_int_to_byte canMsgDataIntToByte;
  if ((msg_rx->identifier == this->pdo_rx_1_id) && (msg_rx->data_length_code == 5))
  {
    this->in.byDxl1Control = msg_rx->data[0];
    if (this->in.byDxl1Control & 0b00000001)
      this->in.byDxl1Torque = true;
    else
      this->in.byDxl1Torque = false;
    if (this->in.byDxl1Control & 0b00000010)
      this->in.byDxl1Led = true;
    else
      this->in.byDxl1Led = false;

    canMsgDataIntToByte.abyValue[0] = msg_rx->data[1];
    canMsgDataIntToByte.abyValue[1] = msg_rx->data[2];
    this->in.uiDxl1Position = canMsgDataIntToByte.uiValue;

    canMsgDataIntToByte.abyValue[0] = msg_rx->data[3];
    canMsgDataIntToByte.abyValue[1] = msg_rx->data[4];
    this->in.uiDxl1Velocity = canMsgDataIntToByte.iValue;
  }
}

void CAN_OPEN::rx_pdo_2(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_2(msg_rx);
  can_msg_data_int_to_byte canMsgDataIntToByte;
  if ((msg_rx->identifier == this->pdo_rx_2_id) && (msg_rx->data_length_code == 5))
  {
    this->in.byDxl2Control = msg_rx->data[0];
    if (this->in.byDxl2Control & 0b00000001)
      this->in.byDxl2Torque = true;
    else
      this->in.byDxl2Torque = false;
    if (this->in.byDxl2Control & 0b00000010)
      this->in.byDxl2Led = true;
    else
      this->in.byDxl2Led = false;

    canMsgDataIntToByte.abyValue[0] = msg_rx->data[1];
    canMsgDataIntToByte.abyValue[1] = msg_rx->data[2];
    this->in.uiDxl2Position = canMsgDataIntToByte.uiValue;

    canMsgDataIntToByte.abyValue[0] = msg_rx->data[3];
    canMsgDataIntToByte.abyValue[1] = msg_rx->data[4];
    this->in.uiDxl2Velocity = canMsgDataIntToByte.iValue;
  }
}

void CAN_OPEN::rx_pdo_3(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_3(msg_rx);
  can_msg_data_int_to_byte canMsgDataIntToByte;
  if ((msg_rx->identifier == this->pdo_rx_3_id) && (msg_rx->data_length_code == 3))
  {
    this->in.byRgbRed = msg_rx->data[0];
    this->in.byRgbGreen = msg_rx->data[1];
    this->in.byRgbBlue = msg_rx->data[2];
  }
}

void CAN_OPEN::tx_pdo_1(void)
{
  CAN_OPEN_BASE::tx_pdo_1();
  if (this->out.uiDxl1Position != this->out_old.uiDxl1Position)
    this->pdo_tx_1_send_msg = 1;
  if (this->out.iDxl1Current != this->out_old.iDxl1Current)
    this->pdo_tx_1_send_msg = 1;
  if (this->out.uiDxl2Position != this->out_old.uiDxl2Position)
    this->pdo_tx_1_send_msg = 1;
  if (this->out.iDxl2Current != this->out_old.iDxl2Current)
    this->pdo_tx_1_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_1_send_msg == 1))
  {
    twai_message_t tx_frame;
    can_msg_data_int_to_byte canMsgDataIntToByte;

    this->pdo_tx_1_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_1_id;

    canMsgDataIntToByte.uiValue = this->out.uiDxl1Position;
    tx_frame.data[0] = canMsgDataIntToByte.abyValue[0];
    tx_frame.data[1] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.iValue = this->out.iDxl1Current;
    tx_frame.data[2] = canMsgDataIntToByte.abyValue[0];
    tx_frame.data[3] = canMsgDataIntToByte.abyValue[1];

    canMsgDataIntToByte.uiValue = this->out.uiDxl2Position;
    tx_frame.data[4] = canMsgDataIntToByte.abyValue[0];
    tx_frame.data[5] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.iValue = this->out.iDxl2Current;
    tx_frame.data[6] = canMsgDataIntToByte.abyValue[0];
    tx_frame.data[7] = canMsgDataIntToByte.abyValue[1];

    ESP32Can.writeFrame(&tx_frame);
    this->out_old.uiDxl1Position = this->out.uiDxl1Position;
    this->out_old.iDxl1Current = this->out.iDxl1Current;
    this->out_old.uiDxl2Position = this->out.uiDxl2Position;
    this->out_old.iDxl2Current = this->out.iDxl2Current;
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();
  this->in.byDxl1Control = 0;
  this->in.uiDxl1Position = 0;
  this->in.uiDxl1Velocity = 0;
  this->in.byDxl2Control = 0;
  this->in.uiDxl2Position = 0;
  this->in.uiDxl2Velocity = 0;
  this->in.byRgbRed = 0;
  this->in.byRgbGreen = 0;
  this->in.byRgbBlue = 0;
}

void CAN_OPEN::loop(void)
{
  // USER BTN
  if ((digitalRead(GPIO_TOUCH_BTN) == 0) && (digitalRead(GPIO_TOUCH_BTN) != this->usr_btn_old))
  {
    this->node_id++;
    if (this->node_id > 16)
      this->node_id = 1;
    EEPROM.write(this->addr_co_eeprom, uint8_t(this->node_id));
    EEPROM.commit();
    CAN_OPEN_BASE::ids();
    this->node_guard_state = NODE_GUARD_STATE_STOPPED;
    CAN_OPEN_BASE::bootup();
  }
  this->usr_btn_old = digitalRead(GPIO_TOUCH_BTN);
}

CAN_OPEN can_open;