#include <string.h>
#include "M5Chain.h"
#include "CHAIN.h"
#include "CHAIN1.h"
#include "CONFIG.h"
#include "CAN_OPEN.h"

// J3
#define TXD_PIN GPIO_NUM_38 // Tx
#define RXD_PIN GPIO_NUM_39 // Rx

CHAIN1::CHAIN1()
{
    // this->device_nums = 0;
    // this->operation_status = 0;
    // this->chain_status = CHAIN_OK;
    // this->devices_list = NULL;
    // this->M5Chain = Chain();
}

void CHAIN1::setup()
{
    this->M5Chain.begin(&Serial1, 115200, RXD_PIN, TXD_PIN);
}

void CHAIN1::init(void)
{
    if (this->M5Chain.isDeviceConnected())
    {
        this->chainStatus = this->M5Chain.getDeviceNum(&this->deviceNums);
        if (this->chainStatus == CHAIN_OK)
        {
            this->devicesList = (device_list_t *)malloc(sizeof(device_list_t));
            this->devicesList->count = this->deviceNums;
            this->devicesList->devices = (device_info_t *)malloc(sizeof(device_info_t) * this->deviceNums);
        }

        if (this->M5Chain.getDeviceList(this->devicesList))
        {
            for (uint8_t i = 0; i < this->devicesList->count; i++)
            {
                if ((this->devicesList->devices[i].device_type == CHAIN_KEY_TYPE_CODE) ||
                    (this->devicesList->devices[i].device_type == CHAIN_ENCODER_TYPE_CODE))
                {
                    this->M5Chain.setKeyButtonTriggerInterval(1, BUTTON_DOUBLE_CLICK_TIME_100MS, BUTTON_LONG_PRESS_TIME_10S, &this->operationStatus);
                    delay(25);
                    this->M5Chain.setRGBValue(this->devicesList->devices[i].id, 0, 1, chainColors[CHAIN_RGB_OFF], 3, &this->operationStatus);
                    delay(25);
                    this->M5Chain.setRGBLight(this->devicesList->devices[i].id, 20, &this->operationStatus, CHAIN_SAVE_FLASH_DISABLE);
                    delay(25);
                    this->M5Chain.setKeyButtonMode(this->devicesList->devices[i].id, CHAIN_BUTTON_NONE_REPORT_MODE, &this->operationStatus);
                    delay(25);
                }

                if (this->devicesList->devices[i].device_type == CHAIN_ANGLE_TYPE_CODE)
                {
                    this->M5Chain.setAngleRotationDirection(
                        this->devicesList->devices[i].id, ANGLE_ROTATION_INCREASING, &this->operationStatus,
                        CHAIN_SAVE_FLASH_DISABLE); // Angle rotation direction increasing, not save
                }
            }
        }
    }
}

void CHAIN1::cyclic(void)
{
    if (this->initDone == 0)
    {
        this->initDone = 1;
        delay(500);
        this->init();
        delay(25);
    }
    else
    {
        // Snapshot `in` once so the UART loop below works from a stable view —
        // the CAN ISR may write `in.rgbLed[]` / `in.rgbLedBrightness[]` on a different core.
        sChainIn inSnap;
        portENTER_CRITICAL(&chain2Mux);
        memcpy(&inSnap, &this->in, sizeof(inSnap));
        portEXIT_CRITICAL(&chain2Mux);

        if (this->devicesList)
        {
            for (uint8_t i = 0; i < this->devicesList->count; i++)
            {
                if (i < 16)
                {
                    if (inSnap.rgbLedBrightness[i] != this->inOld.rgbLedBrightness[i])
                    {
                        this->M5Chain.setRGBLight(this->devicesList->devices[i].id, inSnap.rgbLedBrightness[i], &this->operationStatus, CHAIN_SAVE_FLASH_DISABLE);
                    }
                    this->inOld.rgbLedBrightness[i] = inSnap.rgbLedBrightness[i];

                    if (inSnap.rgbLed[i] != this->inOld.rgbLed[i])
                    {
                        this->M5Chain.setRGBValue(this->devicesList->devices[i].id, 0, 1, chainColors[inSnap.rgbLed[i]], 3, &this->operationStatus);
                    }
                    this->inOld.rgbLed[i] = inSnap.rgbLed[i];

                    if ((this->devicesList->devices[i].device_type == CHAIN_KEY_TYPE_CODE) ||
                        (this->devicesList->devices[i].device_type == CHAIN_ENCODER_TYPE_CODE))
                    {
                        uint8_t keyButtonStatus = 0;
                        this->chainStatus = this->M5Chain.getKeyButtonStatus(this->devicesList->devices[i].id, &keyButtonStatus);
                        if (this->chainStatus == CHAIN_OK)
                        {
                            this->out.keyButtonStatus[i] = uint8_t(keyButtonStatus);
                        }
                    }
                    if (this->devicesList->devices[i].device_type == CHAIN_ANGLE_TYPE_CODE)
                    {
                        uint16_t angle12Bit = 0;
                        this->chainStatus = this->M5Chain.getAngle12BitAdc(this->devicesList->devices[i].id, &angle12Bit);
                        if (this->chainStatus == CHAIN_OK)
                        {
                            this->out.angleValue[i] = int16_t(angle12Bit);
                        }
                    }
                }
            }
        }
        uint8_t value8Bit0 = 0;
        value8Bit0 += this->out.keyButtonStatus[5];
        value8Bit0 = value8Bit0 << 1;
        value8Bit0 += this->out.keyButtonStatus[4];
        value8Bit0 = value8Bit0 << 1;
        value8Bit0 += this->out.keyButtonStatus[3];
        value8Bit0 = value8Bit0 << 1;
        value8Bit0 += this->out.keyButtonStatus[2];
        value8Bit0 = value8Bit0 << 1;
        value8Bit0 += this->out.keyButtonStatus[1];
        value8Bit0 = value8Bit0 << 1;
        value8Bit0 += this->out.keyButtonStatus[0];

        // Publish the TX fields atomically so the CAN ISR on another core sees
        // a consistent set of value8Bit/value16Bit, not a half-updated mix.
        portENTER_CRITICAL(&chain1Mux);
        this->out.value8Bit[0] = value8Bit0;
        portEXIT_CRITICAL(&chain1Mux);
    }
}

CHAIN1 chain1;