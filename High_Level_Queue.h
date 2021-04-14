/*
 * High_Level_Queue.h
 *
 *  Created on: Apr 14, 2021
 *      Author: drew1
 */

#ifndef HIGH_LEVEL_QUEUE_H_
#define HIGH_LEVEL_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "definitions.h"
#include "debug.h"
#include <stdbool.h>
#include <ti/drivers/dpl/HwiP.h>

#define HIGH_LEVEL_QUEUE_LENGTH 3
#define HIGH_LEVEL_QUEUE_ITEM_SIZE sizeof( struct highLevelMessage )

void createHighLevelQueue();

bool readHighLevelMessage( highLevelMessage* message_ptr );
void sendHighLevelMessage( highLevelMessage* message_ptr );

#endif /* HIGH_LEVEL_QUEUE_H_ */
