#include "chain_thread_queue.h"

static QueueHandle_t chainQueueHandle = NULL;

void createChainQueue() {
    dbgEvent( CHAIN_QUEUE_CREATE );

    chainQueueHandle = xQueueCreate( CHAIN_QUEUE_LENGTH, CHAIN_QUEUE_ITEM_SIZE );

    if (chainQueueHandle == NULL) {
        dbgEvent( FATAL_CHAIN_QUEUE );
    }

    dbgEvent( CHAIN_QUEUE_SUCCESS );
}

bool readChainMessage( ChainMessage_t* message_ptr ) {
    dbgEvent( CHAIN_QUEUE_READ_ENTER );
    if( HwiP_inISR() ) {
        dbgEvent( CHAIN_QUEUE_READ_ISR );
        BaseType_t xtask = pdFALSE;

        if( xQueueReceiveFromISR( chainQueueHandle, message_ptr, &xtask ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent
        dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
    else {
        //dbgEvent
        dbgEvent( SENSOR_THREAD_QUEUE_READ_NOT_ISR );
        if( xQueueReceive( chainQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent
        dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
}

void sendChainMessage( ChainMessage_t* message_ptr ) {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    dbgEvent( CHAIN_QUEUE_SEND_ENTER );
    if ( HwiP_inISR() ) {
        dbgEvent( CHAIN_QUEUE_SEND_ISR );
        if( xQueueSendFromISR( chainQueueHandle, message_ptr,  &xHigherPriorityTaskWoken ) != pdPASS ) {
            // Queue is full, fail
            return;
        }

        //dbgEvent( SENSOR_THREAD_QUEUE_SEND_ISR_SUCCESS );
    }
    else {
        dbgEvent( CHAIN_QUEUE_SEND_NOT_ISR );
        if( xQueueSend( chainQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            dbgEvent( FATAL_CHAIN_QUEUE_SEND );
            return;
        }
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    dbgEvent( CHAIN_QUEUE_SEND_SUCCESS );
}
