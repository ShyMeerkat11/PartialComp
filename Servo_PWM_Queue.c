/*
 * Servo_PWM_Queue.c
 *
 *  Created on: Apr 5, 2021
 *      Author: drew1
 */


#include "Servo_PWM_Queue.h"

static QueueHandle_t servoPWMQueueHandle = NULL;

void createServoPWMQueue() {
    //dbgEvent( CHAIN_QUEUE_CREATE );

    servoPWMQueueHandle = xQueueCreate( SERVO_PWM_QUEUE_LENGTH, SERVO_PWM_QUEUE_ITEM_SIZE );

    if (servoPWMQueueHandle == NULL) {
        //dbgEvent( FATAL_CHAIN_QUEUE );
    }

    //dbgEvent( CHAIN_QUEUE_SUCCESS );
}

bool readServoPWMMessage( Servo_PWM_Message* message_ptr ) {
    //dbgEvent( CHAIN_QUEUE_READ_ENTER );
    if( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_READ_ISR );
        BaseType_t xtask = pdFALSE;

        if( xQueueReceiveFromISR( servoPWMQueueHandle, message_ptr, &xtask ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
    else {
        //dbgEvent( SENSOR_THREAD_QUEUE_READ_NOT_ISR );
        if( xQueueReceive( servoPWMQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            return false;
        }
        // message now contains new info
        //dbgEvent( CHAIN_QUEUE_READ_SUCCESS );
        return true;
    }
}

void sendServoPWMMessage( Servo_PWM_Message* message_ptr ) {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    //dbgEvent( CHAIN_QUEUE_SEND_ENTER );
    if ( HwiP_inISR() ) {
        //dbgEvent( CHAIN_QUEUE_SEND_ISR );
        if( xQueueSendFromISR( servoPWMQueueHandle, message_ptr,  &xHigherPriorityTaskWoken ) != pdPASS ) {
            // Queue is full, fail
            return;
        }

        //dbgEvent( SENSOR_THREAD_QUEUE_SEND_ISR_SUCCESS );
    }
    else {
        //dbgEvent( CHAIN_QUEUE_SEND_NOT_ISR );
        if( xQueueSend( servoPWMQueueHandle, message_ptr, portMAX_DELAY ) != pdPASS ) {
            //dbgEvent( FATAL_CHAIN_QUEUE_SEND );
            return;
        }
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    //dbgEvent( CHAIN_QUEUE_SEND_SUCCESS );
}
