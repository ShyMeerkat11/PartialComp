/*
 * spi_task.h
 *
 *  Created on: Apr 26, 2021
 *      Author: drew1
 */

#ifndef SPI_TASK_H_
#define SPI_TASK_H_

#include "FreeRTOS.h"
#include "definitions.h"
#include <pthread.h>
#include "debug.h"
#include "mqtt_if.h"
#include "mqueue.h"
#include <unistd.h>
#include <stddef.h>
#include "High_Level_Queue.h"
#include "uart_term.h"
#include "debug_if.h"
#include <stdbool.h>
#include "mqtt_publish_queue_def.h"
#include <stdio.h>
#include "Servo_PWM_Queue.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "Timer40Queue.h"
#include "High_Level_Task.h"
/* POSIX Header files */
//#include <pthread.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include "ti_drivers_config.h"
#define SPI_MSG_LENGTH  300
#define PRIORITY 1
#define HIGHLEVELTHREADSTACKSIZE 1024
#define TOPIC_STRING_LENGTH 50
void init_spi_task_thread( mqd_t handle );
void* spi_task();

#endif /* SPI_TASK_H_ */
