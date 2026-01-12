
// size 8MB 0x800000
#include <Arduino.h>
#include <M5AtomS3.h>
#include <EEPROM.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include <MCP23017.h>
#include "main.h"

hw_timer_t *timer0 = NULL;

#define GPIO_RESET G5
#define MCP23017_ADDR 0x20
MCP23017 mcp = MCP23017(MCP23017_ADDR, Wire1);

#define ADDRESS_DAC1 0x4C
#define ADDRESS_DAC2 0x4D
#define ADDRESS_ADC1 0x51
#define ADDRESS_ADC2 0x52

void setup()
{
    AtomS3.begin();
    visu_setup();
    EEPROM.begin(1);
    Serial.begin(921600);

    pinMode(GPIO_TOUCH_BTN, INPUT); // USER BTN
    pinMode(G6, INPUT);
    pinMode(GPIO_RESET, OUTPUT);
    digitalWrite(GPIO_RESET, 1);

    Wire1.begin(G38, G39, 400000L);

    mcp.init();
    mcp.portMode(MCP23017Port::A, 0xFF); // Port A as input
    mcp.portMode(MCP23017Port::B, 0x00); // Port B as output

    mcp.writeRegister(MCP23017Register::GPIO_A, 0x00); // Reset port A
    mcp.writeRegister(MCP23017Register::GPIO_B, 0x00); // Reset port B

    mcp.writeRegister(MCP23017Register::IPOL_A, 0xFF);

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
    uint16_t uiDac, uiAdc;
    uint8_t byHigh, byLow, byIn, byOut;
    static uint32_t cycle_time_old;
    uint32_t loop_time_start;

    loop_time_start = micros();

    byOut = can_open.in.byOutput;
    mcp.writePort(MCP23017Port::B, byOut);

    byIn = mcp.readPort(MCP23017Port::A);
    can_open.out.byInput = reverse_byte(byIn);

    // DAC
    Wire1.beginTransmission(ADDRESS_DAC1);
    uiDac = can_open.in.uiDac1;
    byLow = uint8_t(uiDac & 0xFF);
    byHigh = uint8_t((uiDac >> 8) & 0x0F);
    Wire1.write(byHigh);
    Wire1.write(byLow);
    Wire1.endTransmission();

    Wire1.beginTransmission(ADDRESS_DAC2);
    uiDac = can_open.in.uiDac2;
    byLow = uint8_t(uiDac & 0xFF);
    byHigh = uint8_t((uiDac >> 8) & 0x0F);
    Wire1.write(byHigh);
    Wire1.write(byLow);
    Wire1.endTransmission();

    // ADC
    Wire1.beginTransmission(ADDRESS_ADC1);
    Wire1.write(0x00);
    Wire1.endTransmission();
    Wire1.requestFrom(ADDRESS_ADC1, 2);
    // if(Wire1.available() == 2)
    byHigh = Wire1.read();
    byLow = Wire1.read();
    uiAdc = (uint16_t(byHigh) << 8) + uint16_t(byLow);
    can_open.out.uiAdc1 = uiAdc;

    Wire1.beginTransmission(ADDRESS_ADC2);
    Wire1.write(0x00);
    Wire1.endTransmission();
    Wire1.requestFrom(ADDRESS_ADC2, 2);
    // if(Wire1.available() == 2)
    byHigh = Wire1.read();
    byLow = Wire1.read();
    uiAdc = (uint16_t(byHigh) << 8) + uint16_t(byLow);
    can_open.out.uiAdc2 = uiAdc;

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