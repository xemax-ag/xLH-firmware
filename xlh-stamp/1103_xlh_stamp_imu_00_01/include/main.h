#ifndef _MAIN_ARDUINO_H_
#define _MAIN_ARDUINO_H_

void IRAM_ATTR TimerHandler0();
void loop_can(void *pvParameters);
void loop_ddsm210(void *pvParameters);
void loop_imu(void *pvParameters);
void loop_display(void *pvParameters);

#endif