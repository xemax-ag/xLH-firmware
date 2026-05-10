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
  uint8_t i; 

  if ((msg_rx->identifier == this->pdo_rx_1_id) && (msg_rx->data_length_code == 8))
  {
    this->in.abyRxData1[0] = msg_rx->data[0];
    this->in.abyRxData1[1] = msg_rx->data[1];
    this->in.abyRxData1[2] = msg_rx->data[2];
    this->in.abyRxData1[3] = msg_rx->data[3];
    this->in.abyRxData1[4] = msg_rx->data[4];
    this->in.abyRxData1[5] = msg_rx->data[5];
    this->in.abyRxData1[6] = msg_rx->data[6];
    this->in.abyRxData1[7] = msg_rx->data[7];
  }
}

void CAN_OPEN::rx_pdo_2(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_2(msg_rx);
  uint8_t i; 

  can_msg_data_dint_to_byte canMsgDataDintToByte;
  if ((msg_rx->identifier == this->pdo_rx_2_id) && (msg_rx->data_length_code == 8))
  {
    this->in.abyRxData2[0] = msg_rx->data[0];
    this->in.abyRxData2[1] = msg_rx->data[1];
    this->in.abyRxData2[2] = msg_rx->data[2];
    this->in.abyRxData2[3] = msg_rx->data[3];
    this->in.abyRxData2[4] = msg_rx->data[4];
    this->in.abyRxData2[5] = msg_rx->data[5];
    this->in.abyRxData2[6] = msg_rx->data[6];
    this->in.abyRxData2[7] = msg_rx->data[7];
  }
}

void CAN_OPEN::rx_pdo_3(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_3(msg_rx);
  uint8_t i; 

  can_msg_data_dint_to_byte canMsgDataDintToByte;
  if ((msg_rx->identifier == this->pdo_rx_3_id) && (msg_rx->data_length_code == 8))
  {
    this->in.abyRxData3[0] = msg_rx->data[0];
    this->in.abyRxData3[1] = msg_rx->data[1];
    this->in.abyRxData3[2] = msg_rx->data[2];
    this->in.abyRxData3[3] = msg_rx->data[3];
    this->in.abyRxData3[4] = msg_rx->data[4];
    this->in.abyRxData3[5] = msg_rx->data[5];
    this->in.abyRxData3[6] = msg_rx->data[6];
    this->in.abyRxData3[7] = msg_rx->data[7];
  }
}

void CAN_OPEN::rx_pdo_4(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_4(msg_rx);
  can_msg_data_int_to_byte canMsgDataIntToByte;
  uint8_t i; 

  if ((msg_rx->identifier == this->pdo_rx_4_id) && (msg_rx->data_length_code == 8))
  {
    this->in.abyRxData4[0] = msg_rx->data[0];
    this->in.abyRxData4[1] = msg_rx->data[1];
    this->in.abyRxData4[2] = msg_rx->data[2];
    this->in.abyRxData4[3] = msg_rx->data[3];
    this->in.abyRxData4[4] = msg_rx->data[4];
    this->in.abyRxData4[5] = msg_rx->data[5];
    this->in.abyRxData4[6] = msg_rx->data[6];
    this->in.abyRxData4[7] = msg_rx->data[7];
  }
}

void CAN_OPEN::tx_pdo_1(void)
{
  CAN_OPEN_BASE::tx_pdo_1();

  if (this->out.abyTxData1[0] != this->out_old.abyTxData1[0]) this->pdo_tx_1_send_msg = 1;
  if (this->out.abyTxData1[1] != this->out_old.abyTxData1[1]) this->pdo_tx_1_send_msg = 1;
  if (this->out.abyTxData1[2] != this->out_old.abyTxData1[2]) this->pdo_tx_1_send_msg = 1;
  if (this->out.abyTxData1[3] != this->out_old.abyTxData1[3]) this->pdo_tx_1_send_msg = 1;
  if (this->out.abyTxData1[4] != this->out_old.abyTxData1[4]) this->pdo_tx_1_send_msg = 1;
  if (this->out.abyTxData1[5] != this->out_old.abyTxData1[5]) this->pdo_tx_1_send_msg = 1;
  if (this->out.abyTxData1[6] != this->out_old.abyTxData1[6]) this->pdo_tx_1_send_msg = 1;
  if (this->out.abyTxData1[7] != this->out_old.abyTxData1[7]) this->pdo_tx_1_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_1_send_msg == 1))
  {
    twai_message_t tx_frame;

    this->pdo_tx_1_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_1_id;

    tx_frame.data[0] = this->out.abyTxData1[0];
    tx_frame.data[1] = this->out.abyTxData1[1];
    tx_frame.data[2] = this->out.abyTxData1[2];
    tx_frame.data[3] = this->out.abyTxData1[3];
    tx_frame.data[4] = this->out.abyTxData1[4];
    tx_frame.data[5] = this->out.abyTxData1[5];
    tx_frame.data[6] = this->out.abyTxData1[6];
    tx_frame.data[7] = this->out.abyTxData1[7];
    ESP32Can.writeFrame(&tx_frame);

    this->out_old.abyTxData1[0] = this->out.abyTxData1[0];
    this->out_old.abyTxData1[1] = this->out.abyTxData1[1];
    this->out_old.abyTxData1[2] = this->out.abyTxData1[2];
    this->out_old.abyTxData1[3] = this->out.abyTxData1[3];
    this->out_old.abyTxData1[4] = this->out.abyTxData1[4];
    this->out_old.abyTxData1[5] = this->out.abyTxData1[5];
    this->out_old.abyTxData1[6] = this->out.abyTxData1[6];
    this->out_old.abyTxData1[7] = this->out.abyTxData1[7];
  }
}

void CAN_OPEN::tx_pdo_2(void)
{
  CAN_OPEN_BASE::tx_pdo_2();

  if (this->out.abyTxData2[0] != this->out_old.abyTxData2[0]) this->pdo_tx_2_send_msg = 1;
  if (this->out.abyTxData2[1] != this->out_old.abyTxData2[1]) this->pdo_tx_2_send_msg = 1;
  if (this->out.abyTxData2[2] != this->out_old.abyTxData2[2]) this->pdo_tx_2_send_msg = 1;
  if (this->out.abyTxData2[3] != this->out_old.abyTxData2[3]) this->pdo_tx_2_send_msg = 1;
  if (this->out.abyTxData2[4] != this->out_old.abyTxData2[4]) this->pdo_tx_2_send_msg = 1;
  if (this->out.abyTxData2[5] != this->out_old.abyTxData2[5]) this->pdo_tx_2_send_msg = 1;
  if (this->out.abyTxData2[6] != this->out_old.abyTxData2[6]) this->pdo_tx_2_send_msg = 1;
  if (this->out.abyTxData2[7] != this->out_old.abyTxData2[7]) this->pdo_tx_2_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_2_send_msg == 1))
  {
    twai_message_t tx_frame;

    this->pdo_tx_2_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_2_id;

    tx_frame.data[0] = this->out.abyTxData2[0];
    tx_frame.data[1] = this->out.abyTxData2[1];
    tx_frame.data[2] = this->out.abyTxData2[2];
    tx_frame.data[3] = this->out.abyTxData2[3];
    tx_frame.data[4] = this->out.abyTxData2[4];
    tx_frame.data[5] = this->out.abyTxData2[5];
    tx_frame.data[6] = this->out.abyTxData2[6];
    tx_frame.data[7] = this->out.abyTxData2[7];
    ESP32Can.writeFrame(&tx_frame);
    
    this->out_old.abyTxData2[0] = this->out.abyTxData2[0];
    this->out_old.abyTxData2[1] = this->out.abyTxData2[1];
    this->out_old.abyTxData2[2] = this->out.abyTxData2[2];
    this->out_old.abyTxData2[3] = this->out.abyTxData2[3];
    this->out_old.abyTxData2[4] = this->out.abyTxData2[4];
    this->out_old.abyTxData2[5] = this->out.abyTxData2[5];
    this->out_old.abyTxData2[6] = this->out.abyTxData2[6];
    this->out_old.abyTxData2[7] = this->out.abyTxData2[7];
  }
}

void CAN_OPEN::tx_pdo_3(void)
{
  CAN_OPEN_BASE::tx_pdo_3();

  if (this->out.abyTxData3[0] != this->out_old.abyTxData3[0]) this->pdo_tx_3_send_msg = 1;
  if (this->out.abyTxData3[1] != this->out_old.abyTxData3[1]) this->pdo_tx_3_send_msg = 1;
  if (this->out.abyTxData3[2] != this->out_old.abyTxData3[2]) this->pdo_tx_3_send_msg = 1;
  if (this->out.abyTxData3[3] != this->out_old.abyTxData3[3]) this->pdo_tx_3_send_msg = 1;
  if (this->out.abyTxData3[4] != this->out_old.abyTxData3[4]) this->pdo_tx_3_send_msg = 1;
  if (this->out.abyTxData3[5] != this->out_old.abyTxData3[5]) this->pdo_tx_3_send_msg = 1;
  if (this->out.abyTxData3[6] != this->out_old.abyTxData3[6]) this->pdo_tx_3_send_msg = 1;
  if (this->out.abyTxData3[7] != this->out_old.abyTxData3[7]) this->pdo_tx_3_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_3_send_msg == 1))
  {
    twai_message_t tx_frame;

    this->pdo_tx_3_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_3_id;

    tx_frame.data[0] = this->out.abyTxData3[0];
    tx_frame.data[1] = this->out.abyTxData3[1];
    tx_frame.data[2] = this->out.abyTxData3[2];
    tx_frame.data[3] = this->out.abyTxData3[3];
    tx_frame.data[4] = this->out.abyTxData3[4];
    tx_frame.data[5] = this->out.abyTxData3[5];
    tx_frame.data[6] = this->out.abyTxData3[6];
    tx_frame.data[7] = this->out.abyTxData3[7];
    ESP32Can.writeFrame(&tx_frame);

    this->out_old.abyTxData3[0] = this->out.abyTxData3[0];
    this->out_old.abyTxData3[1] = this->out.abyTxData3[1];
    this->out_old.abyTxData3[2] = this->out.abyTxData3[2];
    this->out_old.abyTxData3[3] = this->out.abyTxData3[3];
    this->out_old.abyTxData3[4] = this->out.abyTxData3[4];
    this->out_old.abyTxData3[5] = this->out.abyTxData3[5];
    this->out_old.abyTxData3[6] = this->out.abyTxData3[6];
    this->out_old.abyTxData3[7] = this->out.abyTxData3[7];
  }
}

void CAN_OPEN::tx_pdo_4(void)
{
  CAN_OPEN_BASE::tx_pdo_4();

  if (this->out.abyTxData4[0] != this->out_old.abyTxData4[0]) this->pdo_tx_4_send_msg = 1;
  if (this->out.abyTxData4[1] != this->out_old.abyTxData4[1]) this->pdo_tx_4_send_msg = 1;
  if (this->out.abyTxData4[2] != this->out_old.abyTxData4[2]) this->pdo_tx_4_send_msg = 1;
  if (this->out.abyTxData4[3] != this->out_old.abyTxData4[3]) this->pdo_tx_4_send_msg = 1;
  if (this->out.abyTxData4[4] != this->out_old.abyTxData4[4]) this->pdo_tx_4_send_msg = 1;
  if (this->out.abyTxData4[5] != this->out_old.abyTxData4[5]) this->pdo_tx_4_send_msg = 1;
  if (this->out.abyTxData4[6] != this->out_old.abyTxData4[6]) this->pdo_tx_4_send_msg = 1;
  if (this->out.abyTxData4[7] != this->out_old.abyTxData4[7]) this->pdo_tx_4_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_4_send_msg == 1))
  {
    twai_message_t tx_frame;

    this->pdo_tx_4_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_4_id;

    tx_frame.data[0] = this->out.abyTxData4[0];
    tx_frame.data[1] = this->out.abyTxData4[1];
    tx_frame.data[2] = this->out.abyTxData4[2];
    tx_frame.data[3] = this->out.abyTxData4[3];
    tx_frame.data[4] = this->out.abyTxData4[4];
    tx_frame.data[5] = this->out.abyTxData4[5];
    tx_frame.data[6] = this->out.abyTxData4[6];
    tx_frame.data[7] = this->out.abyTxData4[7];
    ESP32Can.writeFrame(&tx_frame);

    this->out_old.abyTxData4[0] = this->out.abyTxData4[0];
    this->out_old.abyTxData4[1] = this->out.abyTxData4[1];
    this->out_old.abyTxData4[2] = this->out.abyTxData4[2];
    this->out_old.abyTxData4[3] = this->out.abyTxData4[3];
    this->out_old.abyTxData4[4] = this->out.abyTxData4[4];
    this->out_old.abyTxData4[5] = this->out.abyTxData4[5];
    this->out_old.abyTxData4[6] = this->out.abyTxData4[6];
    this->out_old.abyTxData4[7] = this->out.abyTxData4[7];
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();

  this->out.abyTxData1[0] = 0;
  this->out.abyTxData1[1] = 0;
  this->out.abyTxData1[2] = 0;
  this->out.abyTxData1[3] = 0;
  this->out.abyTxData1[4] = 0;
  this->out.abyTxData1[5] = 0;
  this->out.abyTxData1[6] = 0;
  this->out.abyTxData1[7] = 0;

  this->out.abyTxData2[0] = 0;
  this->out.abyTxData2[1] = 0;
  this->out.abyTxData2[2] = 0;
  this->out.abyTxData2[3] = 0;
  this->out.abyTxData2[4] = 0;
  this->out.abyTxData2[5] = 0;
  this->out.abyTxData2[6] = 0;
  this->out.abyTxData2[7] = 0;

  this->out.abyTxData3[0] = 0;
  this->out.abyTxData3[1] = 0;
  this->out.abyTxData3[2] = 0;
  this->out.abyTxData3[3] = 0;
  this->out.abyTxData3[4] = 0;
  this->out.abyTxData3[5] = 0;
  this->out.abyTxData3[6] = 0;
  this->out.abyTxData3[7] = 0;

  this->out.abyTxData4[0] = 0;
  this->out.abyTxData4[1] = 0;
  this->out.abyTxData4[2] = 0;
  this->out.abyTxData4[3] = 0;
  this->out.abyTxData4[4] = 0;
  this->out.abyTxData4[5] = 0;
  this->out.abyTxData4[6] = 0;
  this->out.abyTxData4[7] = 0;
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