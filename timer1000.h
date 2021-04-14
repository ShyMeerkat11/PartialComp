/*
 * timer1000.h
 *
 *  Created on: Mar 3, 2021
 *      Author: drew1
 */

#ifndef TIMER1000_H_
#define TIMER1000_H_
#define TIMER1000_SEND2 -2
#define TIMER1000_SEND1 -1
#define SEND_MESSAGE 1
#define TIMER1000_LENGTH 20000
#include <stddef.h>
#include "statistics_thread_queue.h"
/* Driver Header files */
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include "FreeRTOS.h"
#include "task.h"
#include "definitions.h"
#include "debug.h"

/* Board Header file */
#include "ti_drivers_config.h"

void timer1000Setup();
void timerCallback1000(Timer_Handle myHandle, int_fast16_t status);





#endif /* TIMER1000_H_ */
