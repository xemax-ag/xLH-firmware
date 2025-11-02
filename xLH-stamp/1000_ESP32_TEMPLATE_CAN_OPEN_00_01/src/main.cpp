// size 8MB 0x800000
// https://github.com/m5stack/M5Stack/tree/master/examples/Advanced
// https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Storage/EEPROM/EEPROM.ino
#include <Arduino.h>
#include <M5AtomS3.h>
#include <EEPROM.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "main.h"

#define DEVICE_ID_SWITCH 0x46 // SWITCH
#define DEVICE_ID_BUTTON 0x47 // BUTTON

hw_timer_t *timer0 = NULL;

void setup()
{
    AtomS3.begin();
    visu_setup();
    pinMode(GPIO_TOUCH_BTN, INPUT);
    EEPROM.begin(1);

    Serial.begin(921600);
    //Wire.begin(G2, G1, 400000L);
    //Wire1.begin(G38, G39, 400000L);

    can_open.setup(0);

    timer0 = timerBegin(0, 80, true);
    timerAttachInterrupt(timer0, &TimerHandler0, true);
    timerAlarmWrite(timer0, ISR_TIMER_0_TIME_US, true);
    timerAlarmEnable(timer0);

    delay(25);
    can_open.bootup();

    xTaskCreatePinnedToCore(
        loop_display,   // Function to implement the task
        "loop_display", // Name of the task
        5000,           // Stack size in bytes
        NULL,           // Task input parameter
        10,             // Priority of the task
        NULL,           // Task handle.
        0               // Core where the task should run
    );
}

void loop()
{
    static uint32_t cycle_time_old;
    uint32_t loop_time_start;

    loop_time_start = micros();

    can_open.out.byInputButton = can_open.in.byOutputButton;
    can_open.out.byInputSwitch = can_open.in.byOutputSwitch;

    can_open.loop();

    can_open.obj_dict_base.refresh_time = micros() - cycle_time_old; // us
    cycle_time_old = micros();
    can_open.obj_dict_base.loop_time = micros() - loop_time_start; // us

    delay(1); // to allow background processes
}

void loop_display(void *pvParameters)
{
    while (1)
    {
        visu_loop();
        delay(10);
    }
}

void IRAM_ATTR TimerHandler0()
{
    can_open.cyclic_isr_rx();
    can_open.cyclic_isr_tx();
}