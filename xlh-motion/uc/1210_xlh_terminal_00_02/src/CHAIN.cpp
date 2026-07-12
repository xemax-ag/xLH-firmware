#include "M5Chain.h"
#include "CHAIN.h"

uint8_t chainColors[4][3] = {
    {0x00, 0x00, 0x00},
    {0xFF, 0x00, 0x00},
    {0x00, 0xFF, 0x00},
    {0x00, 0x00, 0xFF},
};

portMUX_TYPE chain1Mux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE chain2Mux = portMUX_INITIALIZER_UNLOCKED;

void printDeviceList(device_list_t *devices)
{
    if (devices == NULL)
    {
        Serial.println("devices is NULL");
        return;
    }

    Serial.print("devices count: ");
    Serial.println(devices->count);

    for (uint8_t i = 0; i < devices->count; i++)
    {
        Serial.print("devices ID: ");
        Serial.println(devices->devices[i].id);
        Serial.print("devices type: ");
        Serial.println(devices->devices[i].device_type);
    }
}