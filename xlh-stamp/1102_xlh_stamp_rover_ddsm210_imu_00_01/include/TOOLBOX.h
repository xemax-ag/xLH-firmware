#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

#include <Arduino.h>

void visu_setup(void);
void visu_loop(void);

typedef struct struct_message_txd
{
    //uint16_t cycleTime;
    int32_t yaw;
    int32_t pitch;
    int32_t roll;
} struct_message_txd;

struct euler_t
{
    float yaw;
    float pitch;
    float roll;
};

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr);

#endif
