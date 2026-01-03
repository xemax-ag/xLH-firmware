#include <stdint.h>
#include <Arduino.h>
#include <ddsm_ctrl.h>
#include "CAN_OPEN.h"
#include "CONFIG.h"
#include "DDSM210.h"

DDSM_CTRL dc;

DDSM210::DDSM210(void) { ; }

void DDSM210::setup(void)
{
  // ddsm init.
  Serial1.begin(DDSM_BAUDRATE, SERIAL_8N1, DDSM_RX, DDSM_TX);
  dc.pSerial = &Serial1;
  // config the type of ddsm.
  dc.set_ddsm_type(210);
  // clear ddsm serial buffer.
  dc.clear_ddsm_buffer();
  // mode
  // dc.ddsm_change_mode(1, 2);
}

void DDSM210::loop(void)
{
  if (this->init_done == 0)
  {
    delay(8);
    dc.clear_ddsm_buffer();
    // --- DDSM210 ---
    // 0 - open loop, CMD: -32767 ~ 32767
    // 2 - speed loop, CMD: -2100 ~ 2100 -> -210 ~ 210 rpm
    // 3 - position loop, CMD: 0 ~ 32767 -> 0 ~ 360°
    dc.ddsm_change_mode(1, 2);
  }

  delay(1);
  dc.clear_ddsm_buffer();
  dc.ddsm_ctrl(1, can_open.in.iSetSpeed, can_open.in.bySetAccTime);
  //if (can_open.out.iCurrent < can_open.in.iSetCurrentLimit)
  //  dc.ddsm_ctrl(1, can_open.in.iSetSpeed, can_open.in.bySetAccTime);
  //else
  //  dc.ddsm_ctrl(1, 0, 0);


  can_open.out.iSpeed = dc.speed_data;
  can_open.out.iCurrent = dc.current;
  can_open.out.byAccTime = dc.acceleration_time;
  can_open.out.byTemperature = dc.temperature;
  this->byErrorCode1 = dc.fault_code;
  // if (can_open.out.iCurrent >= can_open.in.iSetCurrentLimit)
  //	dc.ddsm_ctrl(1, 0, 0);

  delay(1);
  dc.clear_ddsm_buffer();
  dc.ddsm_get_info(1);
  // can_open.out.diMileage = dc.mileage;
  // can_open.out.uiPosition = dc.ddsm_pos;

  this->byErrorCode2 = dc.fault_code;
  can_open.out.uiErrorCode = (uint16_t(this->byErrorCode1) << 8) + uint16_t(this->byErrorCode2);

  this->diDdsm210Position = int32_t(dc.ddsm_pos);

  if (abs(this->diDdsm210Mileage - dc.mileage) > 1)
    this->diDdsm210Mileage = dc.mileage;

  if ((abs(this->diDdsm210Position - this->diDdsm210PositionOld) > 20000) || (abs(this->diDdsm210Mileage - dc.mileage) > 10000)) // 1
  {
    if (this->diDdsm210Position > this->diDdsm210PositionOld)
      this->diDdsm210Mileage--;
    else
      this->diDdsm210Mileage++;
  }
  if (this->diDdsm210Position > this->diDdsm210PositionOld)
    this->diPosition = this->diDdsm210Position + this->diDdsm210Mileage * 32768;
  else
    this->diPosition = this->diDdsm210Position + this->diDdsm210Mileage * 32768;

  this->diDdsm210PositionOld = this->diDdsm210Position;
  can_open.out.diPosition = this->diPosition;

  can_open.out.debug_1 = int16_t(dc.mileage);
  can_open.out.debug_2 = int16_t(dc.ddsm_pos);

  this->init_done = 1;
}

DDSM210 ddsm210;