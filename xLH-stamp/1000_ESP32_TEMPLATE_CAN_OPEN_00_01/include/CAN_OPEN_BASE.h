#ifndef _CAN_OPEN_BASE_H_
#define _CAN_OPEN_BASE_H_

#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>

typedef union
{
  byte data[4];
  float_t rValue;
  int32_t diValue;
  uint32_t udiValue;
} u_co_data_32bit;

typedef union
{
  byte data[2];
  int16_t iValue;
  uint16_t uiValue;
} u_co_data_16bit;

typedef union
{
  byte data[1];
  int8_t siValue;
  uint8_t usiValue;
  byte byValue;
} u_co_data_8bit;

typedef union
{
  int16_t iValue;
  uint16_t uiValue;
  uint8_t abyValue[2];
} can_msg_data_int_to_byte;

typedef union
{
  int32_t diValue;
  uint32_t udiValue;
  uint8_t abyValue[4];
} can_msg_data_dint_to_byte;

typedef struct
{
  uint32_t device_type;
  uint32_t vendor_id;
  uint32_t pdo_tx_1_event_time;
  uint32_t pdo_tx_2_event_time;
  uint32_t pdo_tx_3_event_time;
  uint32_t pdo_tx_4_event_time;
  uint32_t refresh_time;
  uint32_t loop_time;
} s_obj_dict_base;

#define NODE_GUARD_TIMEOUT 0
#define NODE_GUARD_TIMEOUT_BOOTUP 0

#define NODE_GUARD_STATE_STOPPED 0x04
#define NODE_GUARD_STATE_OPERATIONAL 0x05
#define NODE_GUARD_STATE_PRE_OPERATIONAL 0x7F

#define SDO_DOWNLOAD_1_BYTE 0x2F
#define SDO_DOWNLOAD_2_BYTE 0x2B
#define SDO_DOWNLOAD_3_BYTE 0x27
#define SDO_DOWNLOAD_4_BYTE 0x23
#define SDO_DOWNLOAD_SUCCESS 0x60

#define SDO_UPLOAD_REQUEST 0x40
#define SDO_UPLOAD_1_BYTE 0x4F
#define SDO_UPLOAD_2_BYTE 0x4B
#define SDO_UPLOAD_3_BYTE 0x47
#define SDO_UPLOAD_4_BYTE 0x43

#define SDO_FAILURE 0x80

#define OBJ_DICT_DEVICE_TYPE 0x1000
#define OBJ_DICT_EMERGENCY_ID 0x1014
#define OBJ_DICT_VENDOR_ID 0x1018
#define OBJ_DICT_PDO_RX_1_COM_PARS 0x1400
#define OBJ_DICT_PDO_TX_1_COM_PARS 0x1800
#define OBJ_DICT_STATUS_INFORMATION 0x5800

class CAN_OPEN_BASE
{
public:
  u_int16_t node_id;
  u_int16_t nmt_id;
  u_int16_t sync_id;
  u_int16_t sdo_rx_id;
  u_int16_t sdo_tx_id;
  u_int16_t node_guard_id;
  u_int16_t pdo_rx_1_id;
  u_int16_t pdo_rx_2_id;
  u_int16_t pdo_rx_3_id;
  u_int16_t pdo_rx_4_id;
  u_int16_t pdo_tx_1_id;
  u_int16_t pdo_tx_2_id;
  u_int16_t pdo_tx_3_id;
  u_int16_t pdo_tx_4_id;

  CAN_OPEN_BASE(void);
  virtual void setup(void);
  void ids(void);
  virtual void bootup(void);
  virtual void nmt(twai_message_t *msg_rx);
  virtual void node_guard(twai_message_t *msg_rx);
  virtual void sdo_rx(twai_message_t *msg_rx);
  virtual void sync(void);
  virtual void rx_pdo_1(twai_message_t *msg_rx);
  virtual void rx_pdo_2(twai_message_t *msg_rx);
  virtual void rx_pdo_3(twai_message_t *msg_rx);
  virtual void rx_pdo_4(twai_message_t *msg_rx);
  virtual void tx_pdo_1(void);
  virtual void tx_pdo_2(void);
  virtual void tx_pdo_3(void);
  virtual void tx_pdo_4(void);
  void ms_counter(uint32_t value);
  virtual void loop(void);
  virtual void cyclic_isr_tx(void);
  virtual void cyclic_isr_rx(void);  
  virtual void reset_output(void);  
  s_obj_dict_base obj_dict_base;
  uint8_t node_guard_state;
  uint8_t pdo_tx_1_send_msg;
  uint8_t pdo_tx_2_send_msg;
  uint8_t pdo_tx_3_send_msg;
  uint8_t pdo_tx_4_send_msg;
  
private:
  void node_guard_timeout(void);
  uint8_t node_guard_toggle; 
  uint32_t node_guard_ms_counter;
  uint32_t pdo_tx_1_ms_counter;
  uint32_t pdo_tx_2_ms_counter;
  uint32_t pdo_tx_3_ms_counter;
  uint32_t pdo_tx_4_ms_counter;
};

#endif

