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
#include <Dynamixel2Arduino.h>
#include <FastLED.h>

#define UART_PORT Serial1
#define UART_TX_PIN G6
#define UART_RX_PIN G5
#define UART_DIR_PIN G39
#define CAN_RX_PIN GPIO_NUM_8
#define CAN_TX_PIN GPIO_NUM_7
#define RGB_PIN G38
#define RGB_NUM_LEDS 64

CRGB leds[RGB_NUM_LEDS];

const uint8_t DXL_ID_1 = 11;
const uint8_t DXL_ID_2 = 12;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(UART_PORT, UART_DIR_PIN);
// This namespace is required to use Control table item names
using namespace ControlTableItem;

hw_timer_t *timer0 = NULL;

#define STACK_SIZE_DISPLAY 5000
StackType_t xStackDisplay[STACK_SIZE_DISPLAY];
StaticTask_t xTaskBufferDisplay;

#define STACK_SIZE_RGB 5000
StackType_t xStackRgb[STACK_SIZE_RGB];
StaticTask_t xTaskBufferRgb;

void setup()
{
    AtomS3.begin();

    visu_setup();
    pinMode(GPIO_TOUCH_BTN, INPUT);
    pinMode(UART_DIR_PIN, OUTPUT);
    EEPROM.begin(1);
    Serial.begin(921600);
    UART_PORT.setPins(UART_RX_PIN, UART_TX_PIN);
    // Wire1.begin(G38, G39, 400000L);

    can_open.setup(0);

    timer0 = timerBegin(0, 80, true);
    timerAttachInterrupt(timer0, &TimerHandler0, true);
    timerAlarmWrite(timer0, ISR_TIMER_0_TIME_US, true);
    timerAlarmEnable(timer0);

    dxl.begin(1000000);
    delay(100);

    // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
    dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
    // Get DYNAMIXEL information
    // dxl.ping(DXL_ID_1);
    dxl.torqueOff(DXL_ID_1);
    dxl.torqueOff(DXL_ID_2);

    dxl.setOperatingMode(DXL_ID_1, OP_POSITION); // OP_VELOCITY
    dxl.writeControlTableItem(POSITION_P_GAIN, DXL_ID_1, 1000);
    dxl.writeControlTableItem(POSITION_I_GAIN, DXL_ID_1, 300);
    dxl.writeControlTableItem(POSITION_D_GAIN, DXL_ID_1, 50);

    dxl.setOperatingMode(DXL_ID_2, OP_POSITION); // OP_VELOCITY
    dxl.writeControlTableItem(POSITION_P_GAIN, DXL_ID_2, 1000);
    dxl.writeControlTableItem(POSITION_I_GAIN, DXL_ID_2, 300);
    dxl.writeControlTableItem(POSITION_D_GAIN, DXL_ID_2, 50);

    dxl.torqueOn(DXL_ID_1);
    dxl.torqueOn(DXL_ID_2);

    FastLED.addLeds<WS2812B, RGB_PIN, RGB>(leds, RGB_NUM_LEDS);
    FastLED.setBrightness(255);

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

    TaskHandle_t xHandleRgb = NULL;
    xHandleRgb = xTaskCreateStatic(
        loop_rgb,         /* Function that implements the task. */
        "loop_rgb",       /* Text name for the task. */
        STACK_SIZE_RGB,   /* Number of indexes in the xStack array. */
        NULL,             /* Parameter passed into the task. */
        10,               /* Priority at which the task is created. */
        xStackRgb,        /* Array to use as the task's stack. */
        &xTaskBufferRgb); /* Variable to hold the task's data structure. */
}

void loop()
{
    static uint32_t cycle_time_old;
    uint32_t loop_time_start;

    loop_time_start = micros();

    // Dynamixel
    if (can_open.in.byDxl1Torque != false)
        dxl.torqueOn(DXL_ID_1);
    else
        dxl.torqueOff(DXL_ID_1);
    if (can_open.in.byDxl1Led != false)
        dxl.ledOn(DXL_ID_1);
    else
        dxl.ledOff(DXL_ID_1);
    dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID_1, can_open.in.uiDxl1Velocity);
    dxl.setGoalPosition(DXL_ID_1, can_open.in.uiDxl1Position);
    can_open.out.uiDxl1Position = dxl.getPresentPosition(DXL_ID_1, UNIT_RAW);
    can_open.out.iDxl1Current = dxl.getPresentCurrent(DXL_ID_1, UNIT_MILLI_AMPERE); // UNIT_MILLI_AMPERE UNIT_RAW

    if (can_open.in.byDxl2Torque != false)
        dxl.torqueOn(DXL_ID_2);
    else
        dxl.torqueOff(DXL_ID_2);
    if (can_open.in.byDxl2Led != false)
        dxl.ledOn(DXL_ID_2);
    else
        dxl.ledOff(DXL_ID_2);
    dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID_2, can_open.in.uiDxl2Velocity);
    dxl.setGoalPosition(DXL_ID_2, can_open.in.uiDxl2Position);
    can_open.out.uiDxl2Position = dxl.getPresentPosition(DXL_ID_2, UNIT_RAW);
    can_open.out.iDxl2Current = dxl.getPresentCurrent(DXL_ID_2, UNIT_MILLI_AMPERE); // UNIT_MILLI_AMPERE UNIT_RAW

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

void loop_rgb(void *pvParameters)
{
    while (1)
    {
        uint32_t color;

        color = uint32_t(can_open.in.byRgbGreen);
        color = color << 8;
        color += uint32_t(can_open.in.byRgbRed);
        color = color << 8;
        color += uint32_t(can_open.in.byRgbBlue);

        fill_solid(leds, RGB_NUM_LEDS, color);
        FastLED.show();
        delay(1);
    }
}

void IRAM_ATTR TimerHandler0()
{
    can_open.cyclic_isr_rx();
    can_open.cyclic_isr_tx();
}