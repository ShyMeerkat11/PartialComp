#ifndef CHAIN_THREAD_QUEUE_H
#define CHAIN_THREAD_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "definitions.h"
#include "debug.h"
#include <stdbool.h>
#include <ti/drivers/dpl/HwiP.h>

#define CHAIN_QUEUE_LENGTH 3
#define CHAIN_QUEUE_ITEM_SIZE sizeof( struct ChainMessage_t )

void createChainQueue();

bool readChainMessage( ChainMessage_t* message_ptr );
void sendChainMessage( ChainMessage_t* message_ptr );

#endif
