#ifndef _SPI_IO_H_
#define _SPI_IO_H_
#include <Arduino.h>

typedef union
{
  struct
  {
    uint8_t byControl1;
    uint8_t byControl2;
    uint8_t byControl3;
    uint8_t byControl4;
    uint8_t byDigOut_0_7;
    uint8_t byDigOut_8_15;
    uint8_t byOut07;
    uint8_t byOut08;
    uint8_t byServo1;
    uint8_t byServo2;
    uint8_t byServo3;
    uint8_t byServo4;
    uint8_t byServo5;
    uint8_t byServo6;
    uint8_t byServo7;
    uint8_t byServo8;
    uint8_t byOut17;
    uint8_t byOut18;
    uint8_t byOut19;
    uint8_t byOut20;
    uint8_t byOut21;
    uint8_t byOut22;
    uint8_t byOut23;
    uint8_t byOut24;
    uint16_t uiMagnetDutyCycle;
    uint16_t uiOut02;
    uint16_t uiOut03;
    uint16_t uiOut04;
    uint16_t uiOut05;
    uint16_t uiOut06;
    uint16_t uiOut07;
    uint16_t uiOut08;
    uint16_t uiOut09;
    int16_t uiOut10;
    int16_t uiOut11;
    int16_t uiOut12;
    uint32_t diFrequency1;
    uint32_t diFrequency2;
    uint32_t diFrequency3;
    uint32_t diFrequency4;
  } d;
  struct
  {
    uint8_t arrByte[64];
  } stream;
} sUC;

typedef union
{
  struct
  {
    uint8_t byDigInClk;
    uint8_t byDigIn_0_7;
    uint8_t byDigIn_8_15;
    uint8_t byIn04;
    uint8_t byIn05;
    uint8_t byIn06;
    uint8_t byIn07;
    uint8_t byIn08;
    int16_t iIn01;
    int16_t iIn02;
    int16_t iIn03;
    int16_t iIn04;
    int16_t iIn05;
    int16_t iIn06;
    int16_t iIn07;
    int16_t iIn08;
    int16_t iIn09;
    int16_t iIn10;
    int16_t iIn11;
    int16_t iIn12;
    int16_t iIn13;
    int16_t iIn14;
    int16_t iIn15;
    int16_t iIn16;
    int16_t iIn17;
    int16_t iIn18;
    int16_t iIn19;
    int16_t iIn20;
    int32_t diPosition1;
    int32_t diPosition2;
    int32_t diPosition3;
    int32_t diPosition4;
  } d;
  struct
  {
    uint8_t arrByte[64];
  } stream;
} sFpga;

class SPI_IO
{
public:
  SPI_IO(void);
  void setup(void);
  void copy_ucCan(void);
  void copy_fpgaCan(void);
  void input(void);
  void output(void);
  void cyclic(void);
  void cyclic_isr(void);
  sUC uc;
  sUC ucCan;
  //sUC ucCanOld;
  sFpga fpga;
  sFpga fpgaCan;
  sFpga fpgaCanOld;

  // private:
};

extern SPI_IO spi_io;

#endif
