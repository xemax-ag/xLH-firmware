#include <stdint.h>
#include <Arduino.h>
#include <M5AtomS3.h>
#include <Unit_Sonic.h>
#include "CAN_OPEN.h"
#include "CONFIG.h"
#include "ULTRASONIC.h"

SONIC_I2C sensor_sonic;

ULTRASONIC::ULTRASONIC(void) { ; }

void ULTRASONIC::setup(void)
{
	//Wire1.begin(G38, G39, 200000L);
	sensor_sonic.begin(&Wire1, SENS_SONIC_ADDR, G38, G39, 200000L);
}


void ULTRASONIC::loop(void)
{
	if (this->init_done == 0)
	{
		this->init_done = 1;
		delay(20);
	}

	delay(75);
	can_open.out.uiUltrasonic = sensor_sonic.readDistance();

}

ULTRASONIC ultrasonic;