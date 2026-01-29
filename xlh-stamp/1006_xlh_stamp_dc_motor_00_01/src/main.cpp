// size 8MB 0x800000
// https://github.com/m5stack/M5Stack/tree/master/examples/Advanced
// https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Storage/EEPROM/EEPROM.ino
#include <Arduino.h>
#include <M5AtomS3.h>
#include <EEPROM.h>
#include "M5UnitHbridge.h"
#include "UNIT_EXT_ENCODER.h"
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "main.h"

hw_timer_t *timer0 = NULL;

#define STACK_SIZE_DISPLAY 5000
StackType_t xStackDisplay[STACK_SIZE_DISPLAY];
StaticTask_t xTaskBufferDisplay;

#define STACK_SIZE_ENCODER 5000
StackType_t xStackEncoder[STACK_SIZE_ENCODER];
StaticTask_t xTaskBufferEncoder;

M5UnitHbridge hbridge;
UNIT_EXT_ENCODER encoder;

void setup()
{
    AtomS3.begin();
    visu_setup();
    pinMode(GPIO_TOUCH_BTN, INPUT);
    EEPROM.begin(1);

    Serial.begin(921600);
    // Wire1.begin(G38, G39, 400000L);

    while (!hbridge.begin(&Wire1, HBRIDGE_I2C_ADDR, G38, G39, 400000UL))
    {
        // Serial.println("Hbridge not found");
        delay(100);
    }
    // Serial.println("Hbridge found");
    hbridge.setDriverPWMFreq(1500);

    while (!encoder.begin(&Wire, UNIT_EXT_ENCODER_ADDR, G2, G1, 400000UL))
    {
        // Serial.println("Encoder not found");
        delay(100);
    }
    // Serial.println("Encoder found");

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
        loop_display,         /* Function that implements the task. */
        "loop_display",       /* Text name for the task. */
        STACK_SIZE_DISPLAY,   /* Number of indexes in the xStack array. */
        NULL,                 /* Parameter passed into the task. */
        0,                    /* Priority at which the task is created. */
        xStackDisplay,        /* Array to use as the task's stack. */
        &xTaskBufferDisplay); /* Variable to hold the task's data structure. */

    TaskHandle_t xHandleEncoder = NULL;
    xHandleEncoder = xTaskCreateStatic(
        loop_encoder,         /* Function that implements the task. */
        "loop_encoder",       /* Text name for the task. */
        STACK_SIZE_ENCODER,   /* Number of indexes in the xStack array. */
        NULL,                 /* Parameter passed into the task. */
        10,                   /* Priority at which the task is created. */
        xStackEncoder,        /* Array to use as the task's stack. */
        &xTaskBufferEncoder); /* Variable to hold the task's data structure. */
}

void loop()
{
    static uint32_t cycle_time_old;
    uint32_t loop_time_start;
    int16_t iCurrent;

    loop_time_start = micros();

    hbridge.setDriverDirection(hbridge_direction_t(can_open.in.byControl & 0x03));
    hbridge.setDriverSpeed16Bits(can_open.in.uiDutyCycle);
    if (can_open.in.byControl > 0)
    {
        iCurrent = int16_t(hbridge.getMotorCurrent() * 10000.0);
        if (iCurrent > 0)
            can_open.out.uiCurrent = uint16_t(iCurrent);
        else
            can_open.out.uiCurrent = 0;
    }
    else
        can_open.out.uiCurrent = 0;
    can_open.loop();

    can_open.obj_dict_base.refresh_time = micros() - cycle_time_old; // us
    cycle_time_old = micros();
    can_open.obj_dict_base.loop_time = micros() - loop_time_start; // us
    delay(1);                                                      // to allow background processes
}

void loop_display(void *pvParameters)
{
    while (1)
    {
        visu_loop();
        delay(10);
    }
}

int32_t encoder_value;
int32_t encoder_value_old;
uint32_t micros_;
uint32_t micros_old;
int32_t dt;
void loop_encoder(void *pvParameters)
{
    while (1)
    {
        encoder_value = encoder.getEncoderValue();
        micros_ = micros();
        if (((can_open.out.diPosition - encoder_value_old) >= 40) || ((can_open.out.diPosition - encoder_value_old) <= 10))
        {
            dt = int32_t(micros_ - micros_old);
            micros_old = micros_;
            if ((can_open.out.diPosition - encoder_value_old) != 0)
            {
                can_open.out.iSpeed = int16_t(float(dt) / float(can_open.out.diPosition - encoder_value_old));
            }
            else
            {
                can_open.out.iSpeed = 0;
            }
            encoder_value_old = can_open.out.diPosition;
        }
        if (abs(can_open.in.uiDutyCycle) <= 13000)
        {
            can_open.out.iSpeed = 0;
        }

        can_open.out.diPosition = encoder_value;
        delay(1);
    }
}

void IRAM_ATTR TimerHandler0()
{
    can_open.cyclic_isr_rx();
    can_open.cyclic_isr_tx();
}