/*
 * Servo_PWM_Task.h
 *
 *  Created on: Apr 4, 2021
 *      Author: drew1
 */

#ifndef SERVO_PWM_TASK_H_
#define SERVO_PWM_TASK_H_
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
#include "High_Level_Queue.h"

/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <ti/devices/cc32xx/driverlib/pin.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include <ti/devices/cc32xx/driverlib/prcm.h>
#include <ti/devices/cc32xx/driverlib/rom_map.h>

#define PRIORITY 1
#define SENSORTHREADSTACKSIZE 1024
#define TOPIC_STRING_LENGTH 50
void init_servo_PWM_thread( mqd_t handle );
void* servo_PWM_task();
void setDirections(int angle1, int angle2, int angle3, int angle4, int *dutyInc1, int *dutyInc2, int *dutyInc3, int *dutyInc4, uint16_t duty1, uint16_t duty2, uint16_t duty3, uint16_t duty4);
bool checkViableAngles(int angle1, int angle2, int angle3, int angle4);
#endif /* SERVO_PWM_TASK_H_ */
