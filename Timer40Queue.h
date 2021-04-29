/*
 * Timer40Queue.h
 *
 *  Created on: Apr 27, 2021
 *      Author: drew1
 */

#ifndef TIMER40QUEUE_H_
#define TIMER40QUEUE_H_
#include "FreeRTOS.h"
#include "queue.h"
#include "definitions.h"
#include "debug.h"
#include <stdbool.h>
#include <ti/drivers/dpl/HwiP.h>
#include "debug_if.h"

#define TIMER_40_QUEUE_LENGTH 20
#define TIMER_40_QUEUE_ITEM_SIZE sizeof( struct timer40Message )

void createTimer40Queue();

void sendTimer40Message( timer40Message* message_ptr );
bool readTimer40Message( timer40Message* message_ptr );



#endif /* TIMER40QUEUE_H_ */
