#include <stdint.h>
#include <Arduino.h>
#include <M5AtomS3.h>
#include <ddsm_ctrl.h>
#include <Unit_Sonic.h>
#include "CAN_OPEN.h"
#include "CONFIG.h"
#include "ULTRASONIC.h"

SONIC_I2C sensor_sonic;

ULTRASONIC::ULTRASONIC(void) { ; }

void ULTRASONIC::setup(void)
{
	Wire1.begin(G38, G39, 100000L);
	sensor_sonic.begin(&Wire1, SENS_SONIC_ADDR, G38, G39, 200000L);
}

void ULTRASONIC::pcaselect(uint8_t i)
{
	if (i > 7)
		return;

	// Wire1.clearWriteError();
	Wire1.beginTransmission(PCAADDR);
	Wire1.write(1 << i);
	Wire1.endTransmission();
	// Wire1.clearWriteError();
	// Wire1.end();
}

void ULTRASONIC::loop(void)
{
	if (this->init_done == 0)
	{
		this->init_done = 1;
		this->sonic_device = 0;
		this->pcaselect(this->sonic_device);
		delay(20);
	}

	delay(20);
	can_open.out.uiUltrasonic[this->sonic_device] = sensor_sonic.readDistance();
	this->sonic_device++;
	if (this->sonic_device > 5)
		this->sonic_device = 0;
	//delay(5);
	this->pcaselect(this->sonic_device);
}

ULTRASONIC ultrasonic;