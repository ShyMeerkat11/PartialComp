/*
 * High_Level_Queue.c
 *
 *  Created on: Apr 14, 2021
 *      Author: drew1
 */


/*
 * Servo_PWM_Queue.c
 *
 *  Created on: Apr 5, 2021
 *      Author: drew1
 */


#include "High_Level_Queue.h"

static QueueHandle_t highLevelQueueHandle = NULL;

void createHighLevelQueue() {
    //dbgEvent( CHAIN_QUEUE_CREATE );

    highLevelQueueHandle = xQueueCreate( HIGH_LEVEL_QUEUE_LENGTH, HIGH_LEVEL_QUEUE_ITEM_SIZE );

    if (highLevelQueueHandle == NULL) {
        //dbgEvent( FATAL_CHAIN_QUEUE );
    }

    //dbgEvent( CHAIN_QUEUE_SUCCESS );
}

bool readHighLevelMessage( highLevelMessage* message_ptr ) {
    //dbgEvent( CHAIN_QUEUE_READ_ENTER );
    if( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_READ_ISR );
        BaseType_t xtask = pdFALSE;

        if( xQueueReceiveFromISR( highLevelQueueHandle, message_ptr, &xtask ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
    else {
        //dbgEvent( SENSOR_THREAD_QUEUE_READ_NOT_ISR );
        if( xQueueReceive( highLevelQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
}

void sendHighLevelMessage( highLevelMessage* message_ptr ) {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    //dbgEvent( CHAIN_QUEUE_SEND_ENTER );
    if ( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_SEND_ISR );
        if( xQueueSendFromISR( highLevelQueueHandle, message_ptr,  &xHigherPriorityTaskWoken ) != pdPASS ) {
            // Queue is full, fail
            return;
        }

        //dbgEvent( SENSOR_THREAD_QUEUE_SEND_ISR_SUCCESS );
    }
    else {
        //dbgEvent( CHAIN_QUEUE_SEND_NOT_ISR );
        if( xQueueSend( highLevelQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            //dbgEvent( FATAL_CHAIN_QUEUE_SEND );
            return;
        }
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    //dbgEvent( CHAIN_QUEUE_SEND_SUCCESS );
}
