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

void CAN_OPEN::rx_pdo_1(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_1(msg_rx);
  can_msg_data_int_to_byte canMsgDataIntToByte;
  if ((msg_rx->identifier == this->pdo_rx_1_id) && (msg_rx->data_length_code == 6))
  {
    memcpy(this->in.abyRxData1, msg_rx->data, 8);

    portENTER_CRITICAL_ISR(&ddsm210Mux);
      canMsgDataIntToByte.abyValue[0] = this->in.abyRxData1[0];
      canMsgDataIntToByte.abyValue[1] = this->in.abyRxData1[1];
      if (canMsgDataIntToByte.iValue > 300)
        this->inDdsm210.iSetSpeed = 300;
      else if (canMsgDataIntToByte.iValue < -300)
      {
        this->inDdsm210.iSetSpeed = -300;
      }
      else
      {
        this->inDdsm210.iSetSpeed = canMsgDataIntToByte.iValue;
      }

      canMsgDataIntToByte.abyValue[0] = this->in.abyRxData1[2];
      canMsgDataIntToByte.abyValue[1] = this->in.abyRxData1[3];
      this->inDdsm210.iSetCurrentLimit = canMsgDataIntToByte.iValue;

      this->inDdsm210.bySetAccTime = msg_rx->data[4];
      this->inDdsm210.bySetCmd = msg_rx->data[5];
    portEXIT_CRITICAL_ISR(&ddsm210Mux);
  }
}

void CAN_OPEN::tx_pdo_1(void)
{
  can_msg_data_int_to_byte canMsgDataIntToByte;

  portENTER_CRITICAL_ISR(&ddsm210Mux);
    canMsgDataIntToByte.iValue = this->outDdsm210.iSpeed;
    this->out.abyTxData1[0] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData1[1] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.iValue = this->outDdsm210.iCurrent;
    this->out.abyTxData1[2] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData1[3] = canMsgDataIntToByte.abyValue[1];
    this->out.abyTxData1[4] = this->outDdsm210.byAccTime;
    this->out.abyTxData1[5] = this->outDdsm210.byTemperature;
  portEXIT_CRITICAL_ISR(&ddsm210Mux);

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

void CAN_OPEN::tx_pdo_2(void)
{
  can_msg_data_int_to_byte canMsgDataIntToByte;
  can_msg_data_dint_to_byte canMsgDataDintToByte;

  portENTER_CRITICAL_ISR(&ddsm210Mux);
    canMsgDataDintToByte.diValue = this->outDdsm210.diPosition;
    this->out.abyTxData2[0] = canMsgDataDintToByte.abyValue[0];
    this->out.abyTxData2[1] = canMsgDataDintToByte.abyValue[1];
    this->out.abyTxData2[2] = canMsgDataDintToByte.abyValue[2];
    this->out.abyTxData2[3] = canMsgDataDintToByte.abyValue[3];

    canMsgDataIntToByte.uiValue = this->outDdsm210.uiErrorCode;
    this->out.abyTxData2[4] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData2[5] = canMsgDataIntToByte.abyValue[1];
  portEXIT_CRITICAL_ISR(&ddsm210Mux);

  CAN_OPEN_BASE::tx_pdo_2();
  if (memcmp(this->out.abyTxData2, this->out_old.abyTxData2, 8) != 0)
    this->pdo_tx_2_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_2_send_msg == 1))
  {
    twai_message_t tx_frame;


    this->pdo_tx_2_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 6;
    tx_frame.identifier = this->pdo_tx_2_id;
    memcpy(tx_frame.data, this->out.abyTxData2, 8);
    ESP32Can.writeFrame(&tx_frame);

    memcpy(this->out_old.abyTxData2, this->out.abyTxData2, 8);
  }
}

void CAN_OPEN::tx_pdo_3(void)
{
  can_msg_data_int_to_byte canMsgDataIntToByte;

  portENTER_CRITICAL_ISR(&ultrasonicMux);
    canMsgDataIntToByte.uiValue = this->outUltrasonic.uiUltrasonic[0];
    this->out.abyTxData3[0] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData3[1] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.uiValue = this->outUltrasonic.uiUltrasonic[1];
    this->out.abyTxData3[2] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData3[3] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.uiValue = this->outUltrasonic.uiUltrasonic[2];
    this->out.abyTxData3[4] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData3[5] = canMsgDataIntToByte.abyValue[1];
  portEXIT_CRITICAL_ISR(&ultrasonicMux);

  CAN_OPEN_BASE::tx_pdo_3();
  if (memcmp(this->out.abyTxData3, this->out_old.abyTxData3, 8) != 0)
    this->pdo_tx_3_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_3_send_msg == 1))
  {
    twai_message_t tx_frame;
    this->pdo_tx_3_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 6;
    tx_frame.identifier = this->pdo_tx_3_id;
    memcpy(tx_frame.data, this->out.abyTxData3, 8);
    ESP32Can.writeFrame(&tx_frame);

    memcpy(this->out_old.abyTxData3, this->out.abyTxData3, 8);
  }
}

void CAN_OPEN::tx_pdo_4(void)
{
  can_msg_data_int_to_byte canMsgDataIntToByte;

  portENTER_CRITICAL_ISR(&ultrasonicMux);
    canMsgDataIntToByte.uiValue = this->outUltrasonic.uiUltrasonic[3];
    this->out.abyTxData4[0] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData4[1] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.uiValue = this->outUltrasonic.uiUltrasonic[4];
    this->out.abyTxData4[2] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData4[3] = canMsgDataIntToByte.abyValue[1];
    canMsgDataIntToByte.uiValue = this->outUltrasonic.uiUltrasonic[5];
    this->out.abyTxData4[4] = canMsgDataIntToByte.abyValue[0];
    this->out.abyTxData4[5] = canMsgDataIntToByte.abyValue[1];
  portEXIT_CRITICAL_ISR(  &ultrasonicMux);

  CAN_OPEN_BASE::tx_pdo_4();
  if (memcmp(this->out.abyTxData4, this->out_old.abyTxData4, 8) != 0)
    this->pdo_tx_4_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_4_send_msg == 1))
  {
    twai_message_t tx_frame;
    can_msg_data_int_to_byte canMsgDataIntToByte;

    this->pdo_tx_4_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 6;
    tx_frame.identifier = this->pdo_tx_4_id;
    memcpy(tx_frame.data, this->out.abyTxData3, 8);
    ESP32Can.writeFrame(&tx_frame);

    memcpy(this->out_old.abyTxData4, this->out.abyTxData4, 8);
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();
  portENTER_CRITICAL_ISR(&ddsm210Mux);
    this->inDdsm210.iSetSpeed = 0;
    this->inDdsm210.bySetAccTime = 0;
    this->inDdsm210.iSetCurrentLimit = 150;
  portEXIT_CRITICAL(&ddsm210Mux);
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