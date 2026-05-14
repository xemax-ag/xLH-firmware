#ifndef _MAIN_ARDUINO_H_
#define _MAIN_ARDUINO_H_

void IRAM_ATTR TimerHandler0();
void loop_chain1(void *pvParameters);
void loop_chain2(void *pvParameters);
void loop_display(void *pvParameters);

#endif