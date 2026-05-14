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

// Task notified from TimerHandler0 — does all CAN work outside ISR context so
// the vendored ESP32-TWAI-CAN driver (not IRAM-safe) and any future flash-cache
// disable (EEPROM commit etc.) can't fault the ISR path.
TaskHandle_t xHandleCan = NULL;

#define STACK_SIZE_CAN 5000
StackType_t xStackCan[STACK_SIZE_CAN];
StaticTask_t xTaskBufferCan;

#define STACK_SIZE_CHAIN1 25000
StackType_t xStackChain1[STACK_SIZE_CHAIN1];
StaticTask_t xTaskBufferChain1;

#define STACK_SIZE_CHAIN2 25000
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

	// Create the CAN worker task BEFORE the timer is enabled so the first ISR
	// has a valid notification target. Priority 20 sits above both chain tasks
	// (19/18), so a notify preempts them promptly. Pinned to core 1 to match
	// the timer ISR's core (Arduino setup() runs on the loopTask = core 1),
	// keeping the notify path on-core.
	xHandleCan = xTaskCreateStaticPinnedToCore(
		loop_can, "loop_can", STACK_SIZE_CAN, NULL, 20,
		xStackCan, &xTaskBufferCan, 1);

	timer0 = timerBegin(0, 80, true);
	timerAttachInterrupt(timer0, &TimerHandler0, true);
	timerAlarmWrite(timer0, ISR_TIMER_0_TIME_US, true);
	timerAlarmEnable(timer0);

	delay(25);
	can_open.bootup();

	// https://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/01-xTaskCreate
	xTaskCreateStaticPinnedToCore(
		loop_chain1, "loop_chain1", STACK_SIZE_CHAIN1, NULL, 19,
		xStackChain1, &xTaskBufferChain1, 0);

	xTaskCreateStaticPinnedToCore(
		loop_chain2, "loop_chain2", STACK_SIZE_CHAIN2, NULL, 18,
		xStackChain2, &xTaskBufferChain2, 1);

	xTaskCreateStaticPinnedToCore(
		loop_display, "loop_display", STACK_SIZE_DISPLAY, NULL, 1,
		xStackDisplay, &xTaskBufferDisplay, 0);
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

void loop_can(void *pvParameters)
{
	while (1)
	{
		// Block until TimerHandler0 fires (ulTaskNotifyTake with pdTRUE = clear-
		// on-exit, so multiple coalesced notifications collapse into one pass).
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		can_open.cyclic_isr_rx();
		can_open.cyclic_isr_tx();
	}
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
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(xHandleCan, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}