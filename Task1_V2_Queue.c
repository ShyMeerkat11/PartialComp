/*
 * Task1_V2_Queue.c
 *
 *  Created on: Mar 1, 2021
 *      Author: drew1
 */


/*
 * statistics_thread_queue.c
 *
 *  Created on: Mar 1, 2021
 *      Author: drew1
 */

#include "Task1_V2_Queue.h"

static QueueHandle_t TaskV1_V2_QueueHandle = NULL;

void createTask1V2Queue() {
    //dbgEvent( CHAIN_QUEUE_CREATE );

    TaskV1_V2_QueueHandle = xQueueCreate( Task1_V2_QUEUE_LENGTH, Task1_V2_QUEUE_ITEM_SIZE );

    if (TaskV1_V2_QueueHandle == NULL) {
        //dbgEvent( FATAL_CHAIN_QUEUE );
    }

    //dbgEvent( CHAIN_QUEUE_SUCCESS );
}

bool readTask1V2Message( version2_t* message_ptr ) {
    //dbgEvent( CHAIN_QUEUE_READ_ENTER );
    //LOG_TRACE("In Read Function\r \n");
    if( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_READ_ISR );
        BaseType_t xtask = pdFALSE;

        if( xQueueReceiveFromISR( TaskV1_V2_QueueHandle, message_ptr, &xtask ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
    else {
        //dbgEvent( SENSOR_THREAD_QUEUE_READ_NOT_ISR );
        if( xQueueReceive( TaskV1_V2_QueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
}

void sendTask1V2Message( version2_t* message_ptr ) {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    //dbgEvent( CHAIN_QUEUE_SEND_ENTER );
    if ( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_SEND_ISR );
        if( xQueueSendFromISR( TaskV1_V2_QueueHandle, message_ptr,  &xHigherPriorityTaskWoken ) != pdPASS ) {
            // Queue is full, fail
            return;
        }

        //dbgEvent( SENSOR_THREAD_QUEUE_SEND_ISR_SUCCESS );
    }
    else {
        //dbgEvent( CHAIN_QUEUE_SEND_NOT_ISR );
        if( xQueueSend( TaskV1_V2_QueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            //dbgEvent( FATAL_CHAIN_QUEUE_SEND );
            return;
        }
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    //dbgEvent( CHAIN_QUEUE_SEND_SUCCESS );
}




