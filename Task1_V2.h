/*
 * Task1_V2.h
 *
 *  Created on: Mar 1, 2021
 *      Author: drew1
 */

#ifndef TASK1_V2_H_
#define TASK1_V2_H_
#include "FreeRTOS.h"
#include "definitions.h"
#include <pthread.h>
#include "debug.h"
#include "mqtt_if.h"
#include "mqueue.h"
#include "Task1_V2_Queue.h"
#include "hash-string.h"
#include "mqtt_publish_queue_def.h"
#include <stdio.h>
#include "debug_if.h"
#include <ti/drivers/dpl/HwiP.h>
#include "statistics_thread_queue.h"
#include "hash-string.h"
#define PRIORITY 1
#define TIMER1000_MESSAGE -1
#define SEND_MESSAGE 1
#define STATSTHREADSTACKSIZE 1024
#define ONE_SECOND 1000 //1000 HZ
void init_Task1_V2_thread( mqd_t handle );
void* Task1_V2_task();





#endif /* TASK1_V2_H_ */
