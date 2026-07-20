// https://github.com/sdp8483/ESP32-Arduino-CAN/tree/master
/*
ToDo:
- NODE ID mit Verlinkung zum Switch IO
- Verallgemeinerung Objektverzeichnis
*/

#include <stdint.h>
#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN_BASE.h"

#define CAN_RX_PIN GPIO_NUM_8
#define CAN_TX_PIN GPIO_NUM_7

CAN_OPEN_BASE::CAN_OPEN_BASE(void)
{
}

void CAN_OPEN_BASE::setup(void)
{
  this->obj_dict_base.device_type = 0x00000301;
  this->obj_dict_base.vendor_id = 0x00000321; // https://www.can-cia.org/services/canopen-vendor-id => CODESYS

  this->ids();
  this->node_guard_state = NODE_GUARD_STATE_STOPPED;

  // Optimierung: CanId Filter
  ESP32Can.begin(ESP32Can.convertSpeed(1000), CAN_TX_PIN, CAN_RX_PIN, 10, 32);
}

void CAN_OPEN_BASE::ids(void)
{
  this->nmt_id = 0x000;
  this->sync_id = 0x080;
  this->sdo_rx_id = this->node_id + 0x600;
  this->sdo_tx_id = this->node_id + 0x580;
  this->node_guard_id = this->node_id + 0x700;
  this->pdo_rx_1_id = this->node_id + 0x200;
  this->pdo_rx_2_id = this->node_id + 0x300;
  this->pdo_rx_3_id = this->node_id + 0x400;
  this->pdo_rx_4_id = this->node_id + 0x500;
  this->pdo_tx_1_id = this->node_id + 0x180;
  this->pdo_tx_2_id = this->node_id + 0x280;
  this->pdo_tx_3_id = this->node_id + 0x380;
  this->pdo_tx_4_id = this->node_id + 0x480;
}

void CAN_OPEN_BASE::bootup(void)
{
  twai_message_t tx_frame;
  tx_frame.extd = 0;
  tx_frame.data_length_code = 1;
  tx_frame.identifier = this->node_guard_id;
  tx_frame.data[0] = 0x00;
  ESP32Can.writeFrame(&tx_frame);
  this->node_guard_ms_counter = 0;
}

void CAN_OPEN_BASE::nmt(twai_message_t *msg_rx)
{
  if (msg_rx->data_length_code == 2)
  {
    if ((msg_rx->data[1] == this->node_id) || (msg_rx->data[1] == 0x00))
    {
      switch (msg_rx->data[0])
      {
      case 0x01:
        this->node_guard_state = NODE_GUARD_STATE_OPERATIONAL;
        this->pdo_tx_1_send_msg = 1;
        this->pdo_tx_2_send_msg = 1;
        this->pdo_tx_3_send_msg = 1;
        this->pdo_tx_4_send_msg = 1;
        break;
      case 0x02:
        this->node_guard_state = NODE_GUARD_STATE_STOPPED;
        break;
      case 0x80:
        this->node_guard_state = NODE_GUARD_STATE_PRE_OPERATIONAL;
        break;
      case 0x81:
        this->node_guard_state = NODE_GUARD_STATE_STOPPED;
        // this->node_guard_toggle == 0;
        ; // reset-application
        break;
      case 0x82:
        this->node_guard_state = NODE_GUARD_STATE_STOPPED;
        // this->node_guard_toggle == 0;
        ; // reset-communication
        break;
      default:; // do nothing
      }
    }
  }
}

void CAN_OPEN_BASE::node_guard(twai_message_t *msg_rx)
{
  twai_message_t msg_tx;

  if (msg_rx->rtr == 1)
  {
    msg_tx.extd = 0;
    msg_tx.data_length_code = 1;
    msg_tx.identifier = this->node_guard_id;
    if (this->node_guard_toggle == 0)
    {
      this->node_guard_toggle = 1;
      msg_tx.data[0] = this->node_guard_state;
    }
    else
    {
      this->node_guard_toggle = 0;
      msg_tx.data[0] = this->node_guard_state + 0x80;
    }
    ESP32Can.writeFrame(&msg_tx);
  }
  else
  {
    msg_tx.extd = 0;
    msg_tx.data_length_code = 1;
    msg_tx.identifier = this->node_guard_id;
    msg_tx.data[0] = this->node_guard_state;
    ESP32Can.writeFrame(&msg_tx);
    this->node_guard_ms_counter = 0;
  }
  this->node_guard_ms_counter = 0;
}

void CAN_OPEN_BASE::sdo_rx(twai_message_t *msg_rx)
{
  twai_message_t msg_tx;
  uint8_t co_data_8bit;
  u_co_data_16bit co_data_16bit;
  u_co_data_32bit co_data_32bit;

  uint8_t sdo_rx_type;
  uint8_t sdo_tx_type;
  uint16_t index;
  uint8_t sub_index;

  sdo_rx_type = msg_rx->data[0];

  co_data_16bit.data[0] = msg_rx->data[1];
  co_data_16bit.data[1] = msg_rx->data[2];
  index = co_data_16bit.uiValue;

  sub_index = msg_rx->data[3];

  co_data_8bit = msg_rx->data[4];

  co_data_16bit.data[0] = msg_rx->data[4];
  co_data_16bit.data[1] = msg_rx->data[5];

  co_data_32bit.data[0] = msg_rx->data[4];
  co_data_32bit.data[1] = msg_rx->data[5];
  co_data_32bit.data[2] = msg_rx->data[6];
  co_data_32bit.data[3] = msg_rx->data[7];

  switch (sdo_rx_type)
  {
  case SDO_UPLOAD_REQUEST:
    switch (index)
    {
    case OBJ_DICT_DEVICE_TYPE:
      sdo_tx_type = SDO_UPLOAD_4_BYTE;
      msg_tx.extd = 0;
      msg_tx.data_length_code = 8;
      msg_tx.identifier = this->sdo_tx_id;
      msg_tx.data[0] = SDO_UPLOAD_4_BYTE;
      co_data_16bit.uiValue = index;
      msg_tx.data[1] = co_data_16bit.data[0];
      msg_tx.data[2] = co_data_16bit.data[1];
      msg_tx.data[3] = sub_index;
      co_data_32bit.udiValue = this->obj_dict_base.device_type;
      msg_tx.data[4] = co_data_32bit.data[0];
      msg_tx.data[5] = co_data_32bit.data[1];
      msg_tx.data[6] = co_data_32bit.data[2];
      msg_tx.data[7] = co_data_32bit.data[3];
      ESP32Can.writeFrame(&msg_tx);
      break;
    case OBJ_DICT_VENDOR_ID:
      sdo_tx_type = SDO_UPLOAD_4_BYTE;
      msg_tx.extd = 0;
      msg_tx.data_length_code = 8;
      msg_tx.identifier = this->sdo_tx_id;
      msg_tx.data[0] = SDO_UPLOAD_4_BYTE;
      co_data_16bit.uiValue = index;
      msg_tx.data[1] = co_data_16bit.data[0];
      msg_tx.data[2] = co_data_16bit.data[1];
      msg_tx.data[3] = sub_index;
      co_data_32bit.udiValue = this->obj_dict_base.vendor_id;
      msg_tx.data[4] = co_data_32bit.data[0];
      msg_tx.data[5] = co_data_32bit.data[1];
      msg_tx.data[6] = co_data_32bit.data[2];
      msg_tx.data[7] = co_data_32bit.data[3];
      ESP32Can.writeFrame(&msg_tx);
      break;
    case OBJ_DICT_STATUS_INFORMATION:
      switch (sub_index)
      {
      case 1:
        sdo_tx_type = SDO_UPLOAD_4_BYTE;
        msg_tx.extd = 0;
        msg_tx.data_length_code = 8;
        msg_tx.identifier = this->sdo_tx_id;
        msg_tx.data[0] = SDO_UPLOAD_4_BYTE;
        co_data_16bit.uiValue = index;
        msg_tx.data[1] = co_data_16bit.data[0];
        msg_tx.data[2] = co_data_16bit.data[1];
        msg_tx.data[3] = sub_index;
        co_data_32bit.udiValue = this->obj_dict_base.refresh_time;
        msg_tx.data[4] = co_data_32bit.data[0];
        msg_tx.data[5] = co_data_32bit.data[1];
        msg_tx.data[6] = co_data_32bit.data[2];
        msg_tx.data[7] = co_data_32bit.data[3];
        ESP32Can.writeFrame(&msg_tx);
        break;
      case 2:
        sdo_tx_type = SDO_UPLOAD_4_BYTE;
        msg_tx.extd = 0;
        msg_tx.data_length_code = 8;
        msg_tx.identifier = this->sdo_tx_id;
        msg_tx.data[0] = SDO_UPLOAD_4_BYTE;
        co_data_16bit.uiValue = index;
        msg_tx.data[1] = co_data_16bit.data[0];
        msg_tx.data[2] = co_data_16bit.data[1];
        msg_tx.data[3] = sub_index;
        co_data_32bit.udiValue = this->obj_dict_base.loop_time;
        msg_tx.data[4] = co_data_32bit.data[0];
        msg_tx.data[5] = co_data_32bit.data[1];
        msg_tx.data[6] = co_data_32bit.data[2];
        msg_tx.data[7] = co_data_32bit.data[3];
        ESP32Can.writeFrame(&msg_tx);
        break;
      }
    }
    break;
  case SDO_DOWNLOAD_1_BYTE:
    switch (index)
    {
    case 0x5000:
      if (sub_index == 0x01)
      {
        ;
      }
      break;
    }
    msg_tx.extd = 0;
    msg_tx.data_length_code = 8;
    msg_tx.identifier = this->sdo_tx_id;
    msg_tx.data[0] = SDO_DOWNLOAD_SUCCESS;
    co_data_16bit.uiValue = index;
    msg_tx.data[1] = co_data_16bit.data[0];
    msg_tx.data[2] = co_data_16bit.data[1];
    msg_tx.data[3] = sub_index;
    msg_tx.data[4] = 0x00;
    msg_tx.data[5] = 0x00;
    msg_tx.data[6] = 0x00;
    msg_tx.data[7] = 0x00;
    ESP32Can.writeFrame(&msg_tx);
    break;

  case SDO_DOWNLOAD_2_BYTE:
    switch (index)
    {
    case OBJ_DICT_PDO_TX_1_COM_PARS:
      if (sub_index == 0x05)
      {
        this->obj_dict_base.pdo_tx_1_event_time = uint32_t(co_data_16bit.uiValue);
      }
      break;
    }
    msg_tx.extd = 0;
    msg_tx.data_length_code = 8;
    msg_tx.identifier = this->sdo_tx_id;
    msg_tx.data[0] = SDO_DOWNLOAD_SUCCESS;
    co_data_16bit.uiValue = index;
    msg_tx.data[1] = co_data_16bit.data[0];
    msg_tx.data[2] = co_data_16bit.data[1];
    msg_tx.data[3] = sub_index;
    msg_tx.data[4] = 0x00;
    msg_tx.data[5] = 0x00;
    msg_tx.data[6] = 0x00;
    msg_tx.data[7] = 0x00;
    ESP32Can.writeFrame(&msg_tx);
    break;

  case SDO_DOWNLOAD_3_BYTE:
    msg_tx.extd = 0;
    msg_tx.data_length_code = 8;
    msg_tx.identifier = this->sdo_tx_id;
    msg_tx.data[0] = SDO_DOWNLOAD_SUCCESS;
    co_data_16bit.uiValue = index;
    msg_tx.data[1] = co_data_16bit.data[0];
    msg_tx.data[2] = co_data_16bit.data[1];
    msg_tx.data[3] = sub_index;
    msg_tx.data[4] = 0x00;
    msg_tx.data[5] = 0x00;
    msg_tx.data[6] = 0x00;
    msg_tx.data[7] = 0x00;
    ESP32Can.writeFrame(&msg_tx);
    break;

  case SDO_DOWNLOAD_4_BYTE:
    switch (index)
    {
    case OBJ_DICT_EMERGENCY_ID:
      if (sub_index == 0x00)
      {
        ; // ToDo
      }
      break;
    }
    msg_tx.extd = 0;
    msg_tx.data_length_code = 8;
    msg_tx.identifier = this->sdo_tx_id;
    msg_tx.data[0] = SDO_DOWNLOAD_SUCCESS;
    co_data_16bit.uiValue = index;
    msg_tx.data[1] = co_data_16bit.data[0];
    msg_tx.data[2] = co_data_16bit.data[1];
    msg_tx.data[3] = sub_index;
    msg_tx.data[4] = 0x00;
    msg_tx.data[5] = 0x00;
    msg_tx.data[6] = 0x00;
    msg_tx.data[7] = 0x00;
    ESP32Can.writeFrame(&msg_tx);
    break;
  default:; // do nothing
  }
}

void CAN_OPEN_BASE::sync(void) { ; }

void CAN_OPEN_BASE::rx_pdo_1(twai_message_t *msg_rx)
{
  // this->pdo_rx_1_ms_counter = 0;
}

void CAN_OPEN_BASE::rx_pdo_2(twai_message_t *msg_rx) { ; }
void CAN_OPEN_BASE::rx_pdo_3(twai_message_t *msg_rx) { ; }
void CAN_OPEN_BASE::rx_pdo_4(twai_message_t *msg_rx) { ; }

void CAN_OPEN_BASE::tx_pdo_1(void)
{
  // this->pdo_tx_1_send_msg = 0;
  if ((this->obj_dict_base.pdo_tx_1_event_time > 0) &&
      (this->pdo_tx_1_ms_counter >= this->obj_dict_base.pdo_tx_1_event_time))
  {
    this->pdo_tx_1_send_msg = 1;
    this->pdo_tx_1_ms_counter = 0;
  }
}

void CAN_OPEN_BASE::tx_pdo_2(void)
{
  // this->pdo_tx_2_send_msg = 0;
  if ((this->obj_dict_base.pdo_tx_2_event_time > 0) &&
      (this->pdo_tx_2_ms_counter >= this->obj_dict_base.pdo_tx_2_event_time))
  {
    this->pdo_tx_2_send_msg = 1;
    this->pdo_tx_2_ms_counter = 0;
  }
}

void CAN_OPEN_BASE::tx_pdo_3(void) { ; }
void CAN_OPEN_BASE::tx_pdo_4(void) { ; }

void CAN_OPEN_BASE::ms_counter(uint32_t value)
{
  this->pdo_tx_1_ms_counter += value;
  this->pdo_tx_2_ms_counter += value;
  this->pdo_tx_3_ms_counter += value;
  this->pdo_tx_4_ms_counter += value;
  this->node_guard_ms_counter += value;
}

void CAN_OPEN_BASE::loop(void) { ; }

void CAN_OPEN_BASE::cyclic_isr_rx(void)
{
  twai_message_t msg_rx;
  uint16_t n;

  for (int n = 0; n < 32; n++)
  {
    if (ESP32Can.readFrame(&msg_rx, 0))
    {
      if (msg_rx.identifier == this->nmt_id)
        this->nmt(&msg_rx);
      else if (msg_rx.identifier == this->node_guard_id)
        this->node_guard(&msg_rx);
      else if (msg_rx.identifier == this->sdo_rx_id)
        this->sdo_rx(&msg_rx);
      else if (msg_rx.identifier == this->pdo_rx_1_id)
        this->rx_pdo_1(&msg_rx);
      else if (msg_rx.identifier == this->pdo_rx_2_id)
        this->rx_pdo_2(&msg_rx);
      else if (msg_rx.identifier == this->pdo_rx_3_id)
        this->rx_pdo_3(&msg_rx);
      else if (msg_rx.identifier == this->pdo_rx_4_id)
        this->rx_pdo_4(&msg_rx);
    }
  }

  if (this->node_guard_state != NODE_GUARD_STATE_OPERATIONAL)
  {
    this->reset_output();
  }
}

void CAN_OPEN_BASE::cyclic_isr_tx(void)
{
  this->ms_counter(5);
  this->node_guard_timeout();
  this->tx_pdo_1();
  this->tx_pdo_2();
  this->tx_pdo_3();
  this->tx_pdo_4();
}

void CAN_OPEN_BASE::node_guard_timeout(void)
{
  if (this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL)
  {
    if ((this->node_guard_ms_counter >= NODE_GUARD_TIMEOUT) && (NODE_GUARD_TIMEOUT > 0))
    {
      this->node_guard_state = NODE_GUARD_STATE_STOPPED;
    }
  }
  else
  {
    if ((this->node_guard_ms_counter >= NODE_GUARD_TIMEOUT_BOOTUP) && (NODE_GUARD_TIMEOUT_BOOTUP > 0))
    {
      twai_message_t msg_rx;
      // this->node_guard(&msg_rx);
      this->bootup();
    }
  }
}

void CAN_OPEN_BASE::reset_output(void) { ; }

/* RTOS priorities, higher number is more important */
// #define CAN_TX_PRIORITY 3
// #define CAN_RX_PRIORITY 1
