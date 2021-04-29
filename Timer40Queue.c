/*
 * timer40Queue.c
 *
 *  Created on: Apr 27, 2021
 *      Author: drew1
 */

#include "Timer40Queue.h"

static QueueHandle_t timer40QueueHandle = NULL;

void createTimer40Queue() {
    //dbgEvent( CHAIN_QUEUE_CREATE );

    timer40QueueHandle = xQueueCreate( TIMER_40_QUEUE_LENGTH, TIMER_40_QUEUE_ITEM_SIZE );

    if (timer40QueueHandle == NULL) {
        //dbgEvent( FATAL_CHAIN_QUEUE );
    }

    //dbgEvent( CHAIN_QUEUE_SUCCESS );
}

bool readTimer40Message( timer40Message* message_ptr ) {
    //dbgEvent( CHAIN_QUEUE_READ_ENTER );
    if( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_READ_ISR );
        BaseType_t xtask = pdFALSE;

        if( xQueueReceiveFromISR( timer40QueueHandle, message_ptr, &xtask ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
    else {
        //dbgEvent( SENSOR_THREAD_QUEUE_READ_NOT_ISR );
        if( xQueueReceive( timer40QueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
}

void sendTimer40Message( timer40Message* message_ptr ) {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    //dbgEvent( CHAIN_QUEUE_SEND_ENTER );
    if ( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_SEND_ISR );
        if( xQueueSendFromISR( timer40QueueHandle, message_ptr,  &xHigherPriorityTaskWoken ) != pdPASS ) {
            // Queue is full, fail
            return;
        }

        //dbgEvent( SENSOR_THREAD_QUEUE_SEND_ISR_SUCCESS );
    }
    else {
        //dbgEvent( CHAIN_QUEUE_SEND_NOT_ISR );
        if( xQueueSend( timer40QueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            //dbgEvent( FATAL_CHAIN_QUEUE_SEND );
            return;
        }
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    //dbgEvent( CHAIN_QUEUE_SEND_SUCCESS );
}

