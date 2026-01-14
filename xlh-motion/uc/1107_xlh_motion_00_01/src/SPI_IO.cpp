#include <SPI.h>
#include "SPI_IO.h"
#include "CONFIG.h"

#define HSPI_CLK_F 1000000

SPIClass *hspi = NULL;

SPI_IO::SPI_IO()
{
	;
}

void SPI_IO::setup()
{
	// SPI
	// https://docs.espressif.com/projects/esp-idf/en/release-v3.0/api-reference/peripherals/spi_master.html#:~:text=The%20ESP32%20has%20four%20SPI,%2C%20SPI1%2C%20HSPI%20and%20VSPI.
	pinMode(HSPI_SS, OUTPUT); // HSPI SS
	hspi = new SPIClass(FSPI);
	hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); // SCLK, MISO, MOSI, SS
	hspi->beginTransaction(SPISettings(HSPI_CLK_F, MSBFIRST, SPI_MODE0));
	pinMode(hspi->pinSS(), OUTPUT); // HSPI SS
	pinMode(HSPI_SS, OUTPUT);		// HSPI SS
}

void SPI_IO::copy_ucCan()
{
	// this->ucCan.d.byOut05++;

	this->uc.d.byControl1 = this->ucCan.d.byControl1;
	this->uc.d.byControl2 = this->ucCan.d.byControl2;
	this->uc.d.byControl3 = this->ucCan.d.byControl3;
	this->uc.d.byControl4 = this->ucCan.d.byControl4;
	this->uc.d.byDigOut_0_7 = this->ucCan.d.byDigOut_0_7;
	this->uc.d.byDigOut_8_15 = this->ucCan.d.byDigOut_8_15;
	this->uc.d.byOut07 = this->ucCan.d.byOut07;
	this->uc.d.byOut08 = this->ucCan.d.byOut08;
	this->uc.d.byServo1 = this->ucCan.d.byServo1;
	this->uc.d.byServo2 = this->ucCan.d.byServo2;
	this->uc.d.byServo3 = this->ucCan.d.byServo3;
	this->uc.d.byServo4 = this->ucCan.d.byServo4;
	this->uc.d.byServo5 = this->ucCan.d.byServo5;
	this->uc.d.byServo6 = this->ucCan.d.byServo6;
	this->uc.d.byServo7 = this->ucCan.d.byServo7;
	this->uc.d.byServo8 = this->ucCan.d.byServo8;
	this->uc.d.byOut17 = this->ucCan.d.byOut17;
	this->uc.d.byOut18 = this->ucCan.d.byOut18;
	this->uc.d.byOut19 = this->ucCan.d.byOut19;
	this->uc.d.byOut20 = this->ucCan.d.byOut20;
	this->uc.d.byOut21 = this->ucCan.d.byOut21;
	this->uc.d.byOut22 = this->ucCan.d.byOut22;
	this->uc.d.byOut23 = this->ucCan.d.byOut23;
	this->uc.d.byOut24 = this->ucCan.d.byOut24;

	this->uc.d.uiMagnetDutyCycle = this->ucCan.d.uiMagnetDutyCycle;
	this->uc.d.uiOut02 = this->ucCan.d.uiOut02;
	this->uc.d.uiOut03 = this->ucCan.d.uiOut03;
	this->uc.d.uiOut04 = this->ucCan.d.uiOut04;
	this->uc.d.uiOut05 = this->ucCan.d.uiOut05;
	this->uc.d.uiOut06 = this->ucCan.d.uiOut06;
	this->uc.d.uiOut07 = this->ucCan.d.uiOut07;
	this->uc.d.uiOut08 = this->ucCan.d.uiOut08;
	this->uc.d.uiOut09 = this->ucCan.d.uiOut09;
	this->uc.d.uiOut10 = this->ucCan.d.uiOut10;
	this->uc.d.uiOut11 = this->ucCan.d.uiOut11;
	this->uc.d.uiOut12 = this->ucCan.d.uiOut12;

	this->uc.d.diFrequency1 = this->ucCan.d.diFrequency1;
	this->uc.d.diFrequency2 = this->ucCan.d.diFrequency2;
	this->uc.d.diFrequency3 = this->ucCan.d.diFrequency3;
	this->uc.d.diFrequency4 = this->ucCan.d.diFrequency4;
}

void SPI_IO::copy_fpgaCan()
{
	this->fpgaCan.d.byDigInClk = this->fpga.d.byDigInClk;
	this->fpgaCan.d.byDigIn_0_7 = this->fpga.d.byDigIn_0_7;
	this->fpgaCan.d.byDigIn_8_15 = this->fpga.d.byDigIn_8_15;
	this->fpgaCan.d.byIn04 = this->fpga.d.byIn04;
	this->fpgaCan.d.byIn05 = this->fpga.d.byIn05;
	this->fpgaCan.d.byIn06 = this->fpga.d.byIn06;
	this->fpgaCan.d.byIn07 = this->fpga.d.byIn07;
	this->fpgaCan.d.byIn08 = this->fpga.d.byIn08;

	this->fpgaCan.d.iIn01 = this->fpga.d.iIn01;
	this->fpgaCan.d.iIn02 = this->fpga.d.iIn02;
	this->fpgaCan.d.iIn03 = this->fpga.d.iIn03;
	this->fpgaCan.d.iIn04 = this->fpga.d.iIn04;
	this->fpgaCan.d.iIn05 = this->fpga.d.iIn05;
	this->fpgaCan.d.iIn06 = this->fpga.d.iIn06;
	this->fpgaCan.d.iIn07 = this->fpga.d.iIn07;
	this->fpgaCan.d.iIn08 = this->fpga.d.iIn08;
	this->fpgaCan.d.iIn09 = this->fpga.d.iIn09;
	this->fpgaCan.d.iIn10 = this->fpga.d.iIn10;
	this->fpgaCan.d.iIn11 = this->fpga.d.iIn11;
	this->fpgaCan.d.iIn12 = this->fpga.d.iIn12;
	this->fpgaCan.d.iIn13 = this->fpga.d.iIn13;
	this->fpgaCan.d.iIn14 = this->fpga.d.iIn14;
	this->fpgaCan.d.iIn15 = this->fpga.d.iIn15;
	this->fpgaCan.d.iIn16 = this->fpga.d.iIn16;
	this->fpgaCan.d.iIn17 = this->fpga.d.iIn17;
	this->fpgaCan.d.iIn18 = this->fpga.d.iIn18;
	this->fpgaCan.d.iIn19 = this->fpga.d.iIn19;
	this->fpgaCan.d.iIn20 = this->fpga.d.iIn20;

	this->fpgaCan.d.diPosition1 = this->fpga.d.diPosition1;
	this->fpgaCan.d.diPosition2 = this->fpga.d.diPosition2;
	this->fpgaCan.d.diPosition3 = this->fpga.d.diPosition3;
	this->fpgaCan.d.diPosition4 = this->fpga.d.diPosition4;
}

void SPI_IO::input()
{
	;
}

void SPI_IO::output()
{
	;
}

void SPI_IO::cyclic()
{
	;
}

void SPI_IO::cyclic_isr(void)
{
	byte n;
	this->copy_ucCan();
	digitalWrite(HSPI_SS, LOW);
	for (n = 0; n < 64; n++)
	{
		this->fpga.stream.arrByte[n] = hspi->transfer(this->uc.stream.arrByte[n]);
	}
	digitalWrite(HSPI_SS, HIGH);
	this->copy_fpgaCan();
}

SPI_IO spi_io;