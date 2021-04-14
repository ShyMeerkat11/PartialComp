/*
 * Timer_40_SW.h
 *
 *  Created on: Apr 14, 2021
 *      Author: drew1
 */

#ifndef TIMER_40_SW_H_
#define TIMER_40_SW_H_

#include "stdint.h"
#include "projdefs.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdlib.h>
#include "timers.h"
#include <pthread.h>
#include <unistd.h>
#include <mqueue.h>
#include "FreeRTOSConfig.h"
#include "Servo_PWM_Queue.h"
#include <ti/drivers/dpl/HwiP.h>
void vTimerCallback( TimerHandle_t xTimer );
void swTimerSetup(void);
#endif /* TIMER_40_SW_H_ */
