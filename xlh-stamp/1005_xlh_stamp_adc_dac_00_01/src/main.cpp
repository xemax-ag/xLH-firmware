// size 8MB 0x800000
// https://github.com/m5stack/M5Stack/tree/master/examples/Advanced
// https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Storage/EEPROM/EEPROM.ino
#include <Arduino.h>
#include <M5AtomS3.h>
#include <EEPROM.h>
#include "M5_ADS1100.h"
#include <DFRobot_GP8XXX.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "main.h"

#define DEVICE_ID_SWITCH 0x46 // SWITCH
#define DEVICE_ID_BUTTON 0x47 // BUTTON

hw_timer_t *timer0 = NULL;

#define STACK_SIZE_DISPLAY 5000
StackType_t xStackDisplay[STACK_SIZE_DISPLAY];
StaticTask_t xTaskBufferDisplay;

// DAC2
DFRobot_GP8XXX_IIC dac2_ext(RESOLUTION_15_BIT, 0x59, &Wire1);

// ADS
ADS1100 ads;

void setDacVoltage(uint16_t vol, uint8_t ch) {
    uint16_t setting_vol = 0;
    if (vol > 10000) {
        vol = 10000;
    }
    if (ch > 1) ch = 1;
    setting_vol = (int16_t)((float)vol / 10000.0f * 32767.0f);
    if (setting_vol > 32767) {
        setting_vol = 32767;
    }
    dac2_ext.setDACOutVoltage(setting_vol, ch);
}

void setup()
{
    AtomS3.begin();
    visu_setup();
    pinMode(GPIO_TOUCH_BTN, INPUT);
    EEPROM.begin(1);

    Serial.begin(921600);
    // Wire1.begin(G38, G39, 400000L);

        // The address can be changed making the option of connecting multiple
    // devices 地址可以改变，以连接多个设备
    ads.getAddr_ADS1100(
        ADS1100_DEFAULT_ADDRESS);  // 0x48, 1001 000 (ADDR = GND)

    // The ADC gain (PGA).  ADC增益(PGA)
    ads.setGain(GAIN_ONE);  // 1x gain(default)
    // ads.setGain(GAIN_TWO);       // 2x gain
    // ads.setGain(GAIN_FOUR);      // 4x gain
    // ads.setGain(GAIN_EIGHT);     // 8x gain

    // Device operating mode.  设备工作模式
    ads.setMode(MODE_CONTIN);  // Continuous conversion mode (default)
    // ads.setMode(MODE_SINGLE);    // Single-conversion mode

    // Data rate.  数据速率
    ads.setRate(RATE_8);  // 8SPS (default)
    // ads.setRate(RATE_16);        // 16SPS
    // ads.setRate(RATE_32);        // 32SPS
    // ads.setRate(RATE_128);       // 128SPS

    ads.setOSMode(
        OSMODE_SINGLE);  // Set to start a single-conversion.  设置开始一次转换

    ads.begin();  // Sets up the Hardware.  设置硬件

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
}

void loop()
{
    static uint32_t cycle_time_old;
    uint32_t loop_time_start;
    byte error;
    int8_t address;

    loop_time_start = micros();

    address = ads.ads_i2cAddress;
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    can_open.out.iAdc = ads.Measure_Differential();

    dac2_ext.setDACOutVoltage(can_open.in.uiDac0, 0);
    dac2_ext.setDACOutVoltage(can_open.in.uiDac1, 1);

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