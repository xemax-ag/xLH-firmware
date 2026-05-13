#include "CHAIN.h"
#include "CHAIN2.h"
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "M5Chain.h"

// J4
#define TXD_PIN GPIO_NUM_6 // Tx
#define RXD_PIN GPIO_NUM_5 // Rx

CHAIN2::CHAIN2()
{
    // this->device_nums = 0;
    // this->operation_status = 0;
    // this->chain_status = CHAIN_OK;
    // this->devices_list = NULL;
    // this->M5Chain = Chain();
}

void CHAIN2::setup()
{
    this->M5Chain.begin(&Serial2, 115200, RXD_PIN, TXD_PIN);
}

void CHAIN2::init(void)
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
                    this->M5Chain.setRGBValue(this->devicesList->devices[i].id, 0, 1, chainColors[CHAIN_RGB_OFF], 3, &this->operationStatus);
                    this->M5Chain.setRGBLight(this->devicesList->devices[i].id, 20, &this->operationStatus, CHAIN_SAVE_FLASH_DISABLE);
                    this->M5Chain.setKeyButtonMode(this->devicesList->devices[i].id, CHAIN_BUTTON_NONE_REPORT_MODE, &this->operationStatus);
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

void CHAIN2::cyclic(void)
{
    if (this->initDone == 0)
    {
        this->initDone = 1;
        delay(1000);
        this->init();
    }
    else
    {
        if (this->devicesList)
        {
            for (uint8_t i = 0; i < this->devicesList->count; i++)
            {
                if (i < 16)
                {
                    if (this->in.rgbLedBrightness[i] != this->inOld.rgbLedBrightness[i])
                    {
                        this->M5Chain.setRGBLight(this->devicesList->devices[i].id, this->in.rgbLedBrightness[i], &this->operationStatus, CHAIN_SAVE_FLASH_DISABLE);
                    }
                    this->inOld.rgbLedBrightness[i] = this->in.rgbLedBrightness[i];

                    if (this->in.rgbLed[i] != this->inOld.rgbLed[i])
                    {
                        this->M5Chain.setRGBValue(this->devicesList->devices[i].id, 0, 1, chainColors[this->in.rgbLed[i]], 3, &this->operationStatus);
                    }
                    this->inOld.rgbLed[i] = this->in.rgbLed[i];

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
        uint8_t value;

        value = 0;
        value += this->out.keyButtonStatus[8];
        value = value << 1;
        value += this->out.keyButtonStatus[7];
        value = value << 1;
        value += this->out.keyButtonStatus[6];
        value = value << 1;
        value += this->out.keyButtonStatus[5];
        value = value << 1;
        value += this->out.keyButtonStatus[4];
        value = value << 1;
        value += this->out.keyButtonStatus[3];
        value = value << 1;
        value += this->out.keyButtonStatus[1];
        value = value << 1;
        value += this->out.keyButtonStatus[0];
        this->out.value8Bit[0] = value;

        value = 0;
        value += this->out.keyButtonStatus[9];
        this->out.value8Bit[1] = value;

        this->out.value16Bit[0] = this->out.angleValue[2];
    }
}

CHAIN2 chain2;