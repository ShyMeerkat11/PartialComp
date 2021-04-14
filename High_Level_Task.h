/*
 * High_Level_Task.h
 *
 *  Created on: Apr 14, 2021
 *      Author: drew1
 */

#ifndef HIGH_LEVEL_TASK_H_
#define HIGH_LEVEL_TASK_H_

#include "FreeRTOS.h"
#include "definitions.h"
#include <pthread.h>
#include "debug.h"
#include "mqtt_if.h"
#include "mqueue.h"
#include <unistd.h>
#include <stddef.h>
#include "Servo_PWM_Queue.h"
#include "uart_term.h"
#include "debug_if.h"
#include <stdbool.h>
#include "mqtt_publish_queue_def.h"
#include <stdio.h>

#define PRIORITY 1
#define HIGHLEVELTHREADSTACKSIZE 1024
#define TOPIC_STRING_LENGTH 50
void init_high_level_thread( mqd_t handle );
void* high_level_task();


#endif /* HIGH_LEVEL_TASK_H_ */
