/*
 * statistics_thread_queue.h
 *
 *  Created on: Mar 1, 2021
 *      Author: drew1
 */

#ifndef STATISTICS_THREAD_QUEUE_H
#define STATISTICS_THREAD_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "definitions.h"
#include "debug.h"
#include <stdbool.h>
#include <ti/drivers/dpl/HwiP.h>

#define STATS_QUEUE_LENGTH 3
#define STATS_QUEUE_ITEM_SIZE sizeof( struct StatsMessage_t )

void createStatsQueue();

bool readStatsMessage( StatsMessage_t* message_ptr );
void sendStatsMessage( StatsMessage_t* message_ptr );

#endif
