#include <stdint.h>
#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "CONFIG.h"
#include <EEPROM.h>
#include "MUX.h"

CAN_OPEN::CAN_OPEN(void) : CAN_OPEN_BASE() { ; }

void CAN_OPEN::setup(uint8_t addr_co_eeprom)
{
  this->addr_co_eeprom = addr_co_eeprom;
  this->node_id = uint16_t(EEPROM.read(this->addr_co_eeprom));
  if (node_id > 16)
    this->node_id = 16;

  CAN_OPEN_BASE::setup();
}

void CAN_OPEN::tx_pdo_1(void)
{
  can_msg_data_int_to_byte canMsgDataIntToByte;

  portENTER_CRITICAL_ISR(&imuMux);
    canMsgDataIntToByte.uiValue = this->outImu.iYaw;
    this->out.abyTxData1[0] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData1[1] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.uiValue = this->outImu.iPitch;
    this->out.abyTxData1[2] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData1[3] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.uiValue = this->outImu.iRoll;
    this->out.abyTxData1[4] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData1[5] = canMsgDataIntToByte.abyValue[1];
  portEXIT_CRITICAL_ISR(&imuMux);

  CAN_OPEN_BASE::tx_pdo_1();
  if (memcmp(this->out.abyTxData1, this->out_old.abyTxData1, 8) != 0)
    this->pdo_tx_1_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_1_send_msg == 1))
  {
    twai_message_t tx_frame;
    this->pdo_tx_1_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 6;
    tx_frame.identifier = this->pdo_tx_1_id;
    memcpy(tx_frame.data, this->out.abyTxData1, 8);
    ESP32Can.writeFrame(&tx_frame);

    memcpy(this->out_old.abyTxData1, this->out.abyTxData1, 8);
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();
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