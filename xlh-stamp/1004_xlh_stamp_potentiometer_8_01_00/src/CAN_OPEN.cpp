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
}

void CAN_OPEN::tx_pdo_1(void)
{
  CAN_OPEN_BASE::tx_pdo_1();

  uint8_t i;
  for(i = 0; i < 8; i++)
  {
    if (this->out.abyPoti[i] != this->out_old.abyPoti[i])
      this->pdo_tx_1_send_msg = 1;
  }

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_1_send_msg == 1))
  {
    twai_message_t tx_frame;
    can_msg_data_int_to_byte canMsgDataIntToByte;
    
    this->pdo_tx_1_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_1_id;
    for(i = 0; i < 8; i++)
    {
      tx_frame.data[i] = this->out.abyPoti[i];
    }
    tx_frame.data[0] = this->out.abyPoti[0];
    tx_frame.data[1] = this->out.abyPoti[1];
    tx_frame.data[2] = this->out.abyPoti[2];
    tx_frame.data[3] = this->out.abyPoti[3];
    tx_frame.data[4] = this->out.abyPoti[4];
    tx_frame.data[5] = this->out.abyPoti[5];
    tx_frame.data[6] = this->out.abyPoti[6];
    tx_frame.data[7] = this->out.abyPoti[7];
    ESP32Can.writeFrame(&tx_frame);
    for(i = 0; i < 8; i++)
    {
      this->out_old.abyPoti[i] = this->out.abyPoti[i];
    }
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();
  this->in.byNone = 0;
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