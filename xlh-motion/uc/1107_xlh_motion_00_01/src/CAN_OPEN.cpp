#include <stdint.h>
#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "SPI_IO.h"
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
  if ((msg_rx->identifier == this->pdo_rx_1_id) && (msg_rx->data_length_code == 8))
  {
    spi_io.ucCan.d.byControl1 = msg_rx->data[0];
    spi_io.ucCan.d.byControl2 = msg_rx->data[1];
    spi_io.ucCan.d.byControl3 = msg_rx->data[2];
    spi_io.ucCan.d.byControl4 = msg_rx->data[3];

    spi_io.ucCan.d.byDigOut_0_7 = msg_rx->data[4];
    spi_io.ucCan.d.byDigOut_8_15 = msg_rx->data[5];

    canMsgDataIntToByte.abyValue[0] = msg_rx->data[6];
    canMsgDataIntToByte.abyValue[1] = msg_rx->data[7];
    spi_io.ucCan.d.uiMagnetDutyCycle = canMsgDataIntToByte.uiValue;
  }
}

void CAN_OPEN::rx_pdo_2(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_2(msg_rx);
  can_msg_data_dint_to_byte canMsgDataDintToByte;
  if ((msg_rx->identifier == this->pdo_rx_2_id) && (msg_rx->data_length_code == 8))
  {
    canMsgDataDintToByte.abyValue[0] = msg_rx->data[0];
    canMsgDataDintToByte.abyValue[1] = msg_rx->data[1];
    canMsgDataDintToByte.abyValue[2] = msg_rx->data[2];
    canMsgDataDintToByte.abyValue[3] = msg_rx->data[3];
    spi_io.ucCan.d.diFrequency1 = canMsgDataDintToByte.diValue;

    canMsgDataDintToByte.abyValue[0] = msg_rx->data[4];
    canMsgDataDintToByte.abyValue[1] = msg_rx->data[5];
    canMsgDataDintToByte.abyValue[2] = msg_rx->data[6];
    canMsgDataDintToByte.abyValue[3] = msg_rx->data[7];
    spi_io.ucCan.d.diFrequency2 = canMsgDataDintToByte.diValue;
  }
}

void CAN_OPEN::rx_pdo_3(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_3(msg_rx);
  can_msg_data_dint_to_byte canMsgDataDintToByte;
  if ((msg_rx->identifier == this->pdo_rx_3_id) && (msg_rx->data_length_code == 8))
  {
    canMsgDataDintToByte.abyValue[0] = msg_rx->data[0];
    canMsgDataDintToByte.abyValue[1] = msg_rx->data[1];
    canMsgDataDintToByte.abyValue[2] = msg_rx->data[2];
    canMsgDataDintToByte.abyValue[3] = msg_rx->data[3];
    spi_io.ucCan.d.diFrequency3 = canMsgDataDintToByte.diValue;

    canMsgDataDintToByte.abyValue[0] = msg_rx->data[4];
    canMsgDataDintToByte.abyValue[1] = msg_rx->data[5];
    canMsgDataDintToByte.abyValue[2] = msg_rx->data[6];
    canMsgDataDintToByte.abyValue[3] = msg_rx->data[7];
    spi_io.ucCan.d.diFrequency4 = canMsgDataDintToByte.diValue;
  }
}

void CAN_OPEN::rx_pdo_4(twai_message_t *msg_rx)
{
  CAN_OPEN_BASE::rx_pdo_4(msg_rx);
  can_msg_data_int_to_byte canMsgDataIntToByte;
  if ((msg_rx->identifier == this->pdo_rx_4_id) && (msg_rx->data_length_code == 8))
  {
    spi_io.ucCan.d.byServo1 = msg_rx->data[0];
    spi_io.ucCan.d.byServo2 = msg_rx->data[1];
    spi_io.ucCan.d.byServo3 = msg_rx->data[2];
    spi_io.ucCan.d.byServo4 = msg_rx->data[3];
    spi_io.ucCan.d.byServo5 = msg_rx->data[4];
    spi_io.ucCan.d.byServo6 = msg_rx->data[5];
    spi_io.ucCan.d.byServo7 = msg_rx->data[6];
    spi_io.ucCan.d.byServo8 = msg_rx->data[7];
  }
}

void CAN_OPEN::tx_pdo_1(void)
{
  CAN_OPEN_BASE::tx_pdo_1();

  if (spi_io.fpgaCan.d.diPosition1 != spi_io.fpgaCanOld.d.diPosition1)
    this->pdo_tx_1_send_msg = 1;
  if (spi_io.fpgaCan.d.diPosition2 != spi_io.fpgaCanOld.d.diPosition2)
    this->pdo_tx_1_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_1_send_msg == 1))
  {
    twai_message_t tx_frame;
    can_msg_data_dint_to_byte canMsgDataDintToByte;

    this->pdo_tx_1_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_1_id;

    canMsgDataDintToByte.diValue = spi_io.fpgaCan.d.diPosition1;
    tx_frame.data[0] = canMsgDataDintToByte.abyValue[0];
    tx_frame.data[1] = canMsgDataDintToByte.abyValue[1];
    tx_frame.data[2] = canMsgDataDintToByte.abyValue[2];
    tx_frame.data[3] = canMsgDataDintToByte.abyValue[3];

    canMsgDataDintToByte.diValue = spi_io.fpgaCan.d.diPosition2;
    tx_frame.data[4] = canMsgDataDintToByte.abyValue[0];
    tx_frame.data[5] = canMsgDataDintToByte.abyValue[1];
    tx_frame.data[6] = canMsgDataDintToByte.abyValue[2];
    tx_frame.data[7] = canMsgDataDintToByte.abyValue[3];

    ESP32Can.writeFrame(&tx_frame);
    spi_io.fpgaCanOld.d.diPosition1 = spi_io.fpgaCan.d.diPosition1;
    spi_io.fpgaCanOld.d.diPosition2 = spi_io.fpgaCan.d.diPosition2;
  }
}

void CAN_OPEN::tx_pdo_2(void)
{
  CAN_OPEN_BASE::tx_pdo_2();

  if (spi_io.fpgaCan.d.diPosition3 != spi_io.fpgaCanOld.d.diPosition3)
    this->pdo_tx_2_send_msg = 1;
  if (spi_io.fpgaCan.d.diPosition4 != spi_io.fpgaCanOld.d.diPosition4)
    this->pdo_tx_2_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_2_send_msg == 1))
  {
    twai_message_t tx_frame;
    can_msg_data_dint_to_byte canMsgDataDintToByte;

    this->pdo_tx_2_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 8;
    tx_frame.identifier = this->pdo_tx_2_id;

    canMsgDataDintToByte.diValue = spi_io.fpgaCan.d.diPosition3;
    tx_frame.data[0] = canMsgDataDintToByte.abyValue[0];
    tx_frame.data[1] = canMsgDataDintToByte.abyValue[1];
    tx_frame.data[2] = canMsgDataDintToByte.abyValue[2];
    tx_frame.data[3] = canMsgDataDintToByte.abyValue[3];

    canMsgDataDintToByte.diValue = spi_io.fpgaCan.d.diPosition4;
    tx_frame.data[4] = canMsgDataDintToByte.abyValue[0];
    tx_frame.data[5] = canMsgDataDintToByte.abyValue[1];
    tx_frame.data[6] = canMsgDataDintToByte.abyValue[2];
    tx_frame.data[7] = canMsgDataDintToByte.abyValue[3];

    ESP32Can.writeFrame(&tx_frame);
    spi_io.fpgaCanOld.d.diPosition3 = spi_io.fpgaCan.d.diPosition3;
    spi_io.fpgaCanOld.d.diPosition4 = spi_io.fpgaCan.d.diPosition4;
  }
}

void CAN_OPEN::tx_pdo_3(void)
{
  CAN_OPEN_BASE::tx_pdo_3();

  if (spi_io.fpgaCan.d.byDigIn_0_7 != spi_io.fpgaCanOld.d.byDigIn_0_7)
    this->pdo_tx_3_send_msg = 1;
  if (spi_io.fpgaCan.d.byDigIn_8_15 != spi_io.fpgaCanOld.d.byDigIn_8_15)
    this->pdo_tx_3_send_msg = 1;
  if (spi_io.fpgaCan.d.byDigInClk != spi_io.fpgaCanOld.d.byDigInClk)
    this->pdo_tx_3_send_msg = 1;

  if ((this->node_guard_state == NODE_GUARD_STATE_OPERATIONAL) && (this->pdo_tx_3_send_msg == 1))
  {
    twai_message_t tx_frame;

    this->pdo_tx_3_send_msg = 0;
    tx_frame.extd = 0;
    tx_frame.data_length_code = 3;
    tx_frame.identifier = this->pdo_tx_3_id;

    tx_frame.data[0] = spi_io.fpgaCan.d.byDigIn_0_7;
    tx_frame.data[1] = spi_io.fpgaCan.d.byDigIn_8_15;
    tx_frame.data[2] = spi_io.fpgaCan.d.byDigInClk;

    ESP32Can.writeFrame(&tx_frame);
    spi_io.fpgaCanOld.d.byDigIn_0_7 = spi_io.fpgaCan.d.byDigIn_0_7;
    spi_io.fpgaCanOld.d.byDigIn_8_15 = spi_io.fpgaCan.d.byDigIn_8_15;
    spi_io.fpgaCanOld.d.byDigInClk = spi_io.fpgaCan.d.byDigInClk;
  }
}

void CAN_OPEN::reset_output(void)
{
  CAN_OPEN_BASE::reset_output();
  spi_io.ucCan.d.byControl1 = 0;
  spi_io.ucCan.d.byControl2 = 0;
  spi_io.ucCan.d.byControl3 = 0;
  spi_io.ucCan.d.byControl4 = 0;
  spi_io.ucCan.d.byDigOut_0_7 = 0;
  spi_io.ucCan.d.byDigOut_8_15 = 0;
  spi_io.ucCan.d.byServo1 = 0;
  spi_io.ucCan.d.byServo2 = 0;
  spi_io.ucCan.d.byServo3 = 0;
  spi_io.ucCan.d.byServo4 = 0;
  spi_io.ucCan.d.byServo5 = 0;
  spi_io.ucCan.d.byServo6 = 0;
  spi_io.ucCan.d.byServo7 = 0;
  spi_io.ucCan.d.byServo8 = 0;
  spi_io.ucCan.d.uiMagnetDutyCycle = 0;
  spi_io.ucCan.d.diFrequency1 = 0;
  spi_io.ucCan.d.diFrequency2 = 0;
  spi_io.ucCan.d.diFrequency3 = 0;
  spi_io.ucCan.d.diFrequency4 = 0;
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