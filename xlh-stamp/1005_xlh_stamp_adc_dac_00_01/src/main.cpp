// size 8MB 0x800000
// https://github.com/m5stack/M5Stack/tree/master/examples/Advanced
// https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Storage/EEPROM/EEPROM.ino
#include <Arduino.h>
#include <M5AtomS3.h>
#include <EEPROM.h>
#include <DFRobot_GP8XXX.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "main.h"

hw_timer_t *timer0 = NULL;

#define STACK_SIZE_DISPLAY 5000
StackType_t xStackDisplay[STACK_SIZE_DISPLAY];
StaticTask_t xTaskBufferDisplay;

#define STACK_SIZE_DAC 5000
StackType_t xStackDac[STACK_SIZE_DAC];
StaticTask_t xTaskBufferDac;

// DAC2
DFRobot_GP8XXX_IIC dac2_ext(RESOLUTION_15_BIT, 0x59, &Wire1);

// ADS
const int ADS1110_ADDRESS = 0x48; // I2C address of the ADS1110

void setup()
{
    AtomS3.begin();
    visu_setup();
    pinMode(GPIO_TOUCH_BTN, INPUT);
    EEPROM.begin(1);

    Serial.begin(921600);
    // Wire1.begin(G38, G39, 400000L);

    // ADS1110
    Wire.begin(G2, G1, 100000L);
    // Configure the ADS1110
    //Wire.beginTransmission(ADS1110_ADDRESS);
    //Wire.write(0x85); // Config register address
    //Wire.write(0x83); // Single-ended measurement on AIN0, gain = 2/3, mode = continuous conversion, data rate = 860 SPS
    //Wire.endTransmission();
    
    dac2_ext.begin();

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

    TaskHandle_t xHandleDac = NULL;
    xHandleDac = xTaskCreateStatic(
        loop_dac,         /* Function that implements the task. */
        "loop_dac",       /* Text name for the task. */
        STACK_SIZE_DAC,   /* Number of indexes in the xStack array. */
        NULL,             /* Parameter passed into the task. */
        10,               /* Priority at which the task is created. */
        xStackDac,        /* Array to use as the task's stack. */
        &xTaskBufferDac); /* Variable to hold the task's data structure. */
}

void loop()
{
    static uint32_t cycle_time_old;
    uint32_t loop_time_start;
    byte error;

    loop_time_start = micros();

    // https://docs.m5stack.com/en/unit/Unit-ADC_V1.1
    // https://www.robotics.org.za/ADS1110-MOD
    // https://www.ti.com/lit/ds/symlink/ads1110.pdf page 11
    // Start a conversion
    Wire.beginTransmission(ADS1110_ADDRESS);
    Wire.write(0b10000100); // Conversion register address PGA 1, SPS 60, 14-Bit
    Wire.endTransmission();
    
    // Wait for the conversion to finish
    delay(18);
    
    // Read the conversion result (MSB first)
    Wire.requestFrom(ADS1110_ADDRESS, 2);
    byte msb = Wire.read();
    byte lsb = Wire.read();
    int result = (msb << 8) | lsb;
    result = result * 4;// scale +- 2^15
    can_open.out.iAdc = result;


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

void loop_dac(void *pvParameters)
{
    while (1)
    {
        dac2_ext.setDACOutVoltage(can_open.in.uiDac0, 0);
        dac2_ext.setDACOutVoltage(can_open.in.uiDac1, 1);
        delay(4);
    }
}

void IRAM_ATTR TimerHandler0()
{
    can_open.cyclic_isr_rx();
    can_open.cyclic_isr_tx();
}