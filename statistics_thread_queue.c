#include "statistics_thread_queue.h"

static QueueHandle_t statsQueueHandle = NULL;

void createStatsQueue() {
    //dbgEvent( CHAIN_QUEUE_CREATE );

    statsQueueHandle = xQueueCreate( STATS_QUEUE_LENGTH, STATS_QUEUE_ITEM_SIZE );

    if (statsQueueHandle == NULL) {
        //dbgEvent( FATAL_CHAIN_QUEUE );
    }

    //dbgEvent( CHAIN_QUEUE_SUCCESS );
}

bool readStatsMessage( StatsMessage_t* message_ptr ) {
    //dbgEvent( CHAIN_QUEUE_READ_ENTER );
    if( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_READ_ISR );
        BaseType_t xtask = pdFALSE;

        if( xQueueReceiveFromISR( statsQueueHandle, message_ptr, &xtask ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
    else {
        //dbgEvent( SENSOR_THREAD_QUEUE_READ_NOT_ISR );
        if( xQueueReceive( statsQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
}

void sendStatsMessage( StatsMessage_t* message_ptr ) {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    //dbgEvent( CHAIN_QUEUE_SEND_ENTER );
    if ( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_SEND_ISR );
        if( xQueueSendFromISR( statsQueueHandle, message_ptr,  &xHigherPriorityTaskWoken ) != pdPASS ) {
            // Queue is full, fail
            return;
        }

        //dbgEvent( SENSOR_THREAD_QUEUE_SEND_ISR_SUCCESS );
    }
    else {
        //dbgEvent( CHAIN_QUEUE_SEND_NOT_ISR );
        if( xQueueSend( statsQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            //dbgEvent( FATAL_CHAIN_QUEUE_SEND );
            return;
        }
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    //dbgEvent( CHAIN_QUEUE_SEND_SUCCESS );
}
