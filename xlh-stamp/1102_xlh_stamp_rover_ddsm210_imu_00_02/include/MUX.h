#ifndef _MUX_H_
#define _MUX_H_

#include <Arduino.h>

// Spinlocks guarding the cross-core shared fields between the CAN ISR
// and the loop_chainN FreeRTOS tasks. Use portENTER_CRITICAL in task context and
// portENTER_CRITICAL_ISR in ISR context.
extern portMUX_TYPE ddsm210Mux;
extern portMUX_TYPE imuMux;

#endif