#include <Arduino.h>
#include <M5AtomS3.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include <EEPROM.h>
#include "SPI_IO.h"
#include "main.h"

hw_timer_t *timer0 = NULL;

#define STACK_SIZE_DISPLAY 5000
StackType_t xStackDisplay[STACK_SIZE_DISPLAY];
StaticTask_t xTaskBufferDisplay;

void setup()
{
	AtomS3.begin();

	visu_setup();
	pinMode(GPIO_TOUCH_BTN, INPUT);
	EEPROM.begin(1);
	// Serial.begin(921600);
	spi_io.setup();
	can_open.setup(0);

	timer0 = timerBegin(0, 80, true);
	timerAttachInterrupt(timer0, &TimerHandler0, true);
	timerAlarmWrite(timer0, ISR_TIMER_0_TIME_US, true);
	timerAlarmEnable(timer0);

	delay(25);
	can_open.bootup();

	// https://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/01-xTaskCreate
	TaskHandle_t xHandleDisplay = NULL;
	xHandleDisplay = xTaskCreateStatic(
		loop_display,		  /* Function that implements the task. */
		"loop_display",		  /* Text name for the task. */
		STACK_SIZE_DISPLAY,	  /* Number of indexes in the xStack array. */
		NULL,				  /* Parameter passed into the task. */
		0,					  /* Priority at which the task is created. */
		xStackDisplay,		  /* Array to use as the task's stack. */
		&xTaskBufferDisplay); /* Variable to hold the task's data structure. */
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
	
	//spi_io.cyclic_isr();
	can_open.loop();

	can_open.obj_dict_base.refresh_time = micros() - cycle_time_old; // us
	cycle_time_old = micros();
	can_open.obj_dict_base.loop_time = micros() - loop_time_start; // us

	delay(1);
}

void loop_display(void *pvParameters)
{
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
	spi_io.cyclic_isr();
}