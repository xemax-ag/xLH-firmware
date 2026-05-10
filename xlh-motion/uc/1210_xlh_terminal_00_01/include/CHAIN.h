#ifndef _CHAIN_H_
#define _CHAIN_H_
#include <Arduino.h>

typedef struct
{
  uint8_t byDummy;
} sChainIn;

typedef struct
{
  uint8_t byDummy;
} sChainOut;


class CHAIN
{
public:
  CHAIN(void);
  void setup(void);
  void init(void);
  void cyclic(void);
  sChainIn in;
  sChainOut out;
  //sUC ucCanOld;

private:
  uint8_t init_done;
};

extern CHAIN chain;

#endif
