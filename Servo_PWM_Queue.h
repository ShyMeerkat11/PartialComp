/*
 * Servo_PWM_Queue.h
 *
 *  Created on: Apr 5, 2021
 *      Author: drew1
 */

#ifndef SERVO_PWM_QUEUE_H_
#define SERVO_PWM_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "definitions.h"
#include "debug.h"
#include <stdbool.h>
#include <ti/drivers/dpl/HwiP.h>

#define SERVO_PWM_QUEUE_LENGTH 3
#define SERVO_PWM_QUEUE_ITEM_SIZE sizeof( struct Servo_PWM_Message )

void createServoPWMQueue();

bool readServoPWMMessage( Servo_PWM_Message* message_ptr );
void sendServoPWMMessage( Servo_PWM_Message* message_ptr );


#endif /* SERVO_PWM_QUEUE_H_ */
