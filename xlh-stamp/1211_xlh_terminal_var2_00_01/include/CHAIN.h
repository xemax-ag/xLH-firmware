#ifndef _CHAIN_H_
#define _CHAIN_H_

#include <Arduino.h>
#include "M5Chain.h"

typedef struct
{
  uint8_t rgbLed[16];
  uint8_t rgbLedBrightness[16];
  
} sChainIn;

typedef struct
{
  uint8_t keyButtonStatus[16];
  int16_t encoderValue[16];
  int16_t angleValue[16];
  //
  uint8_t value8Bit[8];
  int16_t value16Bit[8];
  
  
} sChainOut;

#define CHAIN_RGB_OFF 0
#define CHAIN_RGB_RED 1
#define CHAIN_RGB_GREEN 2
#define CHAIN_RGB_BLUE 3

extern uint8_t chainColors[4][3];

// Spinlocks guarding the cross-core shared fields between the CAN ISR
// (CAN_OPEN::rx_pdo_1 writes chainN.in.*; CAN_OPEN::tx_pdo_N reads chainN.out.value*Bit[])
// and the loop_chainN FreeRTOS tasks. Use portENTER_CRITICAL in task context and
// portENTER_CRITICAL_ISR in ISR context.
extern portMUX_TYPE chain1Mux;
extern portMUX_TYPE chain2Mux;

void printDeviceList(device_list_t *devices);

#endif
