/*
 * Task1_V2_Queue.h
 *
 *  Created on: Mar 1, 2021
 *      Author: drew1
 */

#ifndef TASK1_V2_QUEUE_H_
#define TASK1_V2_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "definitions.h"
#include "debug.h"
#include <stdbool.h>
#include <ti/drivers/dpl/HwiP.h>
#include "debug_if.h"
#define Task1_V2_QUEUE_LENGTH 20
#define Task1_V2_QUEUE_ITEM_SIZE sizeof( struct version2_t )

void createTask1V2Queue();

void sendTask1V2Message( version2_t* message_ptr );
bool readTask1V2Message( version2_t* message_ptr );




#endif /* TASK1_V2_QUEUE_H_ */
