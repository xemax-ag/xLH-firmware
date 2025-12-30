#include <Arduino.h>
#include <M5AtomS3.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "DDSM210.h"
#include "IMU.h"
#include "TOOLBOX.h"
#include <EEPROM.h>
#include "main.h"


hw_timer_t *timer0 = NULL;

void setup()
{
	AtomS3.begin();
	visu_setup();
	pinMode(GPIO_TOUCH_BTN, INPUT);
	EEPROM.begin(1);
	// Serial.begin(921600);
	imu.setup();
	ddsm210.setup();
	can_open.setup(0);

	timer0 = timerBegin(0, 80, true);
	timerAttachInterrupt(timer0, &TimerHandler0, true);
	timerAlarmWrite(timer0, ISR_TIMER_0_TIME_US, true);
	timerAlarmEnable(timer0);

	delay(25);
	can_open.bootup();

	// https://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/01-xTaskCreate
	xTaskCreatePinnedToCore(
		loop_display,	// Function to implement the task
		"loop_display", // Name of the task
		5000,			// Stack size in bytes
		NULL,			// Task input parameter
		0,				// Priority of the task
		NULL,			// Task handle.
		0				// Core where the task should run
	);
}

void loop()
{
	static uint8_t init_done;
	static uint32_t cycle_time_old;
	uint32_t loop_time_start;

	loop_time_start = micros();

	if (init_done == 0)
	{
		init_done = 1;
	}

	imu.loop();
	ddsm210.loop();	
	can_open.loop();

	can_open.obj_dict_base.refresh_time = micros() - cycle_time_old; // us
	cycle_time_old = micros();
	can_open.obj_dict_base.loop_time = micros() - loop_time_start; // us
}

void loop_display(void *pvParameters)
{
	static uint8_t sonic_device;
	static uint8_t init_done;

	if (init_done == 0)
	{
		init_done = 1;
	}

	while (1)
	{
		visu_loop();
		delay(50);
	}
}

void IRAM_ATTR TimerHandler0()
{
	can_open.cyclic_isr_rx();
	can_open.cyclic_isr_tx();
	imu.read_sensor = 1;
}