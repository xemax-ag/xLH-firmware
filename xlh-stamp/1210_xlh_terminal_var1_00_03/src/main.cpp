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
		xStackCan, &xTaskBufferCan, 0);

	timer0 = timerBegin(0, 80, true);
	timerAttachInterrupt(timer0, &TimerHandler0, true);
	timerAlarmWrite(timer0, ISR_TIMER_0_TIME_US, true);
	timerAlarmEnable(timer0);

	delay(25);
	can_open.bootup();

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
	can_open.loop();
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