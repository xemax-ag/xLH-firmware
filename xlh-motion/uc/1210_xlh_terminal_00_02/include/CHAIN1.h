#ifndef _CHAIN1_H_
#define _CHAIN1_H_
#include <Arduino.h>
#include "M5Chain.h"
#include "CHAIN.h"


class CHAIN1
{
public:
  CHAIN1(void);
  void setup(void);
  void init(void);
  void cyclic(void);
  sChainIn in;
  sChainIn inOld;
  sChainOut out;

private:
  uint8_t initDone;
  uint16_t deviceNums = 0;
  uint8_t operationStatus = 0;
  chain_status_t chainStatus = CHAIN_OK;
  device_list_t *devicesList = NULL;
  Chain M5Chain;
};

extern CHAIN1 chain1;

#endif
