#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "CONFIG.h"
#include <EEPROM.h>
#include "CHAIN1.h"
#include "CHAIN2.h"

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
  uint8_t brightness;

  if ((msg_rx->identifier == this->pdo_rx_1_id) && (msg_rx->data_length_code == 8))
  {
    memcpy(this->in.abyRxData1, msg_rx->data, 8);

    brightness = this->in.abyRxData1[7];
    if (brightness > 100) brightness = 100;

    // Publish to chain1.in / chain2.in atomically per chain — the chain tasks
    // (potentially on another core) snapshot these fields under the same mux.
    portENTER_CRITICAL_ISR(&chain1Mux);
    chain1.in.rgbLed[0] = (this->in.abyRxData1[0] & 0b00000011);
    chain1.in.rgbLed[1] = (this->in.abyRxData1[0] & 0b00001100) >> 2;
    chain1.in.rgbLed[2] = (this->in.abyRxData1[0] & 0b00110000) >> 4;
    chain1.in.rgbLed[3] = (this->in.abyRxData1[0] & 0b11000000) >> 6;

    chain1.in.rgbLed[4] = (this->in.abyRxData1[1] & 0b00000011);
    chain1.in.rgbLed[5] = (this->in.abyRxData1[1] & 0b00001100) >> 2;
    chain1.in.rgbLed[6] = (this->in.abyRxData1[1] & 0b00110000) >> 4;
    chain1.in.rgbLed[7] = (this->in.abyRxData1[1] & 0b11000000) >> 6;

    chain1.in.rgbLed[8] = (this->in.abyRxData1[2] & 0b00000011);
    chain1.in.rgbLed[9] = (this->in.abyRxData1[2] & 0b00001100) >> 2;
    chain1.in.rgbLed[10] = (this->in.abyRxData1[2] & 0b00110000) >> 4;
    chain2.in.rgbLed[11] = (this->in.abyRxData1[2] & 0b11000000) >> 6;
    memset(chain1.in.rgbLedBrightness, brightness, sizeof(chain1.in.rgbLedBrightness));
    portEXIT_CRITICAL_ISR(&chain1Mux);

    portENTER_CRITICAL_ISR(&chain2Mux);
    chain2.in.rgbLed[0] = (this->in.abyRxData1[3] & 0b00000011);
    chain2.in.rgbLed[1] = (this->in.abyRxData1[3] & 0b00001100) >> 2;
    chain2.in.rgbLed[2] = (this->in.abyRxData1[3] & 0b00110000) >> 4;
    chain2.in.rgbLed[3] = (this->in.abyRxData1[3] & 0b11000000) >> 6;

    chain2.in.rgbLed[4] = (this->in.abyRxData1[4] & 0b00000011);
    chain2.in.rgbLed[5] = (this->in.abyRxData1[4] & 0b00001100) >> 2;
    chain2.in.rgbLed[6] = (this->in.abyRxData1[4] & 0b00110000) >> 4;
    chain2.in.rgbLed[7] = (this->in.abyRxData1[4] & 0b11000000) >> 6;

    chain2.in.rgbLed[8] = (this->in.abyRxData1[5] & 0b00000011);
    chain2.in.rgbLed[9] = (this->in.abyRxData1[5] & 0b00001100) >> 2;
    chain2.in.rgbLed[10] = (this->in.abyRxData1[5] & 0b00110000) >> 4;
    chain2.in.rgbLed[11] = (this->in.abyRxData1[5] & 0b11000000) >> 6;

    chain2.in.rgbLed[12] = (this->in.abyRxData1[5] & 0b00000011);
    memset(chain2.in.rgbLedBrightness, brightness, sizeof(chain2.in.rgbLedBrightness));
    portEXIT_CRITICAL_ISR(&chain2Mux);
  }
}

void CAN_OPEN::tx_pdo_1(void)
{
  can_msg_data_int_to_byte canMsgDataIntToByte;

  portENTER_CRITICAL_ISR(&chain1Mux);
  this->out.abyTxData1[0] = chain1.out.value8Bit[0];
  this->out.abyTxData1[1] = chain1.out.value8Bit[1];

  canMsgDataIntToByte.iValue = chain1.out.value16Bit[0];
  this->out.abyTxData1[2] = canMsgDataIntToByte.abyValue[0];
  this->out.abyTxData1[3] = canMsgDataIntToByte.abyValue[1];

  canMsgDataIntToByte.iValue = chain1.out.value16Bit[1];
  this->out.abyTxData1[4] = canMsgDataIntToByte.abyValue[0];
  this->out.abyTxData1[5] = canMsgDataIntToByte.abyValue[1];

  canMsgDataIntToByte.iValue = chain1.out.value16Bit[2];
  this->out.abyTxData1[6] = canMsgDataIntToByte.abyValue[0];
  this->out.abyTxData1[7] = canMsgDataIntToByte.abyValue[1];
  portEXIT_CRITICAL_ISR(&chain1Mux);

  CAN_OPEN_BASE::tx_pdo_1();

  if (memcmp(this->out.abyTxData1, this->out_old.abyTxData1, 8) != 0)
    this->pdo_tx_1_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_1_send_msg == 1))
  {
    twai_message_t tx_frame;

    this->pdo_tx_1_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_1_id;
    memcpy(tx_frame.data, this->out.abyTxData1, 8);
    ESP32Can.writeFrame(&tx_frame);

    memcpy(this->out_old.abyTxData1, this->out.abyTxData1, 8);
  }
}

void CAN_OPEN::tx_pdo_2(void)
{
  can_msg_data_int_to_byte canMsgDataIntToByte;

  // Sample chain2.out under its mux so the snapshot reflects one cyclic() pass.
  portENTER_CRITICAL_ISR(&chain2Mux);
  this->out.abyTxData2[0] = chain2.out.value8Bit[0];
  this->out.abyTxData2[1] = chain2.out.value8Bit[1];

  canMsgDataIntToByte.iValue = chain2.out.value16Bit[0];
  this->out.abyTxData2[2] = canMsgDataIntToByte.abyValue[0];
  this->out.abyTxData2[3] = canMsgDataIntToByte.abyValue[1];
  portEXIT_CRITICAL_ISR(&chain2Mux);
  CAN_OPEN_BASE::tx_pdo_2();

  if (memcmp(this->out.abyTxData2, this->out_old.abyTxData2, 8) != 0)
    this->pdo_tx_2_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_2_send_msg == 1))
  {
    twai_message_t tx_frame;

    this->pdo_tx_2_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 4;
    tx_frame.identifier = this->pdo_tx_2_id;
    memcpy(tx_frame.data, this->out.abyTxData2, 8);
    ESP32Can.writeFrame(&tx_frame);

    memcpy(this->out_old.abyTxData2, this->out.abyTxData2, 8);
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();
  memset(&this->out, 0, sizeof(this->out));
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