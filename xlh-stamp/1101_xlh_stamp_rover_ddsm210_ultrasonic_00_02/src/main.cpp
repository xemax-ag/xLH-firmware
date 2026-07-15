#include <Arduino.h>
#include <M5AtomS3.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "DDSM210.h"
#include "ULTRASONIC.h"
#include "TOOLBOX.h"
#include <EEPROM.h>
#include "MUX.h"
#include "main.h"

portMUX_TYPE ddsm210Mux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE ultrasonicMux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t *timer0 = NULL;

TaskHandle_t xHandleCan = NULL;
#define STACK_SIZE_CAN 5000
StackType_t xStackCan[STACK_SIZE_CAN];
StaticTask_t xTaskBufferCan;

#define STACK_SIZE_DDSM210 25000
StackType_t xStackDSDM210[STACK_SIZE_DDSM210];
StaticTask_t xTaskBufferDDSM210;

#define STACK_SIZE_ULTRASONIC 25000
StackType_t xStackUltrasonic[STACK_SIZE_ULTRASONIC];
StaticTask_t xTaskBufferUltrasonic;

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
	ultrasonic.setup();
	ddsm210.setup();
	can_open.setup(0);

	delay(25);
	can_open.bootup();

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
		loop_ddsm210, "loop_ddsm210", STACK_SIZE_DDSM210, NULL, 18,
		xStackDSDM210, &xTaskBufferDDSM210, 0);

	xTaskCreateStaticPinnedToCore(
		loop_ultrasonic, "loop_ultrasonic", STACK_SIZE_ULTRASONIC, NULL, 19,
		xStackUltrasonic, &xTaskBufferUltrasonic, 1);

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

void loop_ddsm210(void *pvParameters)
{
	while (1)
	{
		ddsm210.loop();
		delay(1);
	}
}

void loop_ultrasonic(void *pvParameters)
{
	while (1)
	{
		ultrasonic.loop();
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
