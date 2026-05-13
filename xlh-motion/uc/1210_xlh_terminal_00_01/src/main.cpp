#include <Arduino.h>
#include <M5AtomS3.h>
#include <EEPROM.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "CHAIN1.h"
#include "CHAIN2.h"
#include "main.h"

hw_timer_t *timer0 = NULL;

#define STACK_SIZE_CHAIN1 5000
StackType_t xStackChain1[STACK_SIZE_CHAIN1];
StaticTask_t xTaskBufferChain1;

#define STACK_SIZE_CHAIN2 5000
StackType_t xStackChain2[STACK_SIZE_CHAIN2];
StaticTask_t xTaskBufferChain2;

#define STACK_SIZE_DISPLAY 5000
StackType_t xStackDisplay[STACK_SIZE_DISPLAY];
StaticTask_t xTaskBufferDisplay;

void setup()
{
	AtomS3.begin();

	visu_setup();
	pinMode(GPIO_TOUCH_BTN, INPUT);
	EEPROM.begin(1);
	Serial.begin(921600);
	chain1.setup();
	chain2.setup();	
	can_open.setup(0);

	timer0 = timerBegin(0, 80, true);
	timerAttachInterrupt(timer0, &TimerHandler0, true);
	timerAlarmWrite(timer0, ISR_TIMER_0_TIME_US, true);
	timerAlarmEnable(timer0);

	delay(25);
	can_open.bootup();

	// https://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/01-xTaskCreate
	// configMAX_PRIORITIES

	TaskHandle_t xHandleChain1 = NULL;
	xHandleChain1 = xTaskCreateStatic(
		loop_chain1,		 /* Function that implements the task. */
		"loop_chain1",		 /* Text name for the task. */
		STACK_SIZE_CHAIN1,	 /* Number of indexes in the xStack array. */
		NULL,				 /* Parameter passed into the task. */
		8,					 /* Priority at which the task is created. */
		xStackChain1,		 /* Array to use as the task's stack. */
		&xTaskBufferChain1); /* Variable to hold the task's data structure. */

	TaskHandle_t xHandleChain2 = NULL;
	xHandleChain2 = xTaskCreateStatic(
		loop_chain2,		 /* Function that implements the task. */
		"loop_chain2",		 /* Text name for the task. */
		STACK_SIZE_CHAIN2,	 /* Number of indexes in the xStack array. */
		NULL,				 /* Parameter passed into the task. */
		10,					 /* Priority at which the task is created. */
		xStackChain2,		 /* Array to use as the task's stack. */
		&xTaskBufferChain2); /* Variable to hold the task's data structure. */

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

	can_open.loop();

	can_open.obj_dict_base.refresh_time = micros() - cycle_time_old; // us
	cycle_time_old = micros();
	can_open.obj_dict_base.loop_time = micros() - loop_time_start; // us

	delay(10);
}

void loop_chain1(void *pvParameters)
{
	while (1)
	{
		chain1.cyclic();
		delay(1);
	}
}

void loop_chain2(void *pvParameters)
{
	while (1)
	{
		chain2.cyclic();
		delay(1);
	}
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
}