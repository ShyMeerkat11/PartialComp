#ifndef DEBUG_H
#define DEBUG_H

// Definitions for debug events
#define BINARY_EVENT_ARRAY_SIZE 8
#define BINARY_CONVERSION 2
#define HIGH 1
#define LOW 0
#define BIT6 6
#define BIT5 5
#define BIT4 4
#define BIT3 3
#define BIT2 2
#define BIT1 1
#define BIT0 0
#define FATAL_ERROR_LIMIT 0x7F

#include "FreeRTOS.h"
#include "task.h"
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"

// MQTT INTERFACE
#define MQTT_IF_SUB_ENTER 0x00
#define MQTT_IF_SUB_EXIT  0x01
#define MQTT_CB_ENTER 0x02
#define MQTT_CB_SEND 0x03
#define MQTT_CB_DONE 0x04
#define MQTT_APP_THREAD_PRE_RECEIVE 0x05
#define MQTT_APP_THREAD_PST_RECEIVE 0x06
#define MQTT_APP_THREAD_EVENT_RECEIVED 0x07
#define MQTT_APP_THREAD_EVENT_DESTROY 0x08
#define MQTT_APP_THREAD_EVENT_RECEIVED_LOCKED 0x09

// MQTT CLIENT
#define MQTT_CLIENT_PRE_RECEIVE 0x10
#define MQTT_CLIENT_PST_RECEIVE 0x11
#define MQTT_CLIENT_PRE_PUB 0x12
#define MQTT_CLIENT_EVENT_CB_ENTER 0x13
#define MQTT_CLIENT_EVENT_CB_CONNACK 0x14
#define MQTT_CLIENT_EVENT_CB_SUBACK 0x15
#define MQTT_CLIENT_EVENT_CB_PUBACK 0x16
#define MQTT_CLIENT_PUBLISH 0x17

#define CHAIN_TASK_RECEIVE 0x70
#define CHAIN_TASK_RECEIVE_SUCCESS 0x7F
#define CHAIN_TASK_SEND 0x71
#define CHAIN_QUEUE_CREATE 0x72
#define CHAIN_QUEUE_SUCCESS 0x73
#define CHAIN_CB_ENTER 0x74
#define CHAIN_CB_SEND 0x75
#define CHAIN_CB_SUCCESS 0x76
#define CHAIN_QUEUE_SEND_ENTER 0x77
#define CHAIN_QUEUE_SEND_ISR 0x78
#define CHAIN_QUEUE_SEND_NOT_ISR 0x79
#define CHAIN_QUEUE_SEND_SUCCESS 0x7A
#define CHAIN_QUEUE_READ_ENTER 0x7B
#define CHAIN_QUEUE_READ_ISR 0x7C
#define CHAIN_QUEUE_READ_SUCCESS 0x7E

#define FATAL_MQTT_DISCONNECT 0x80
#define FATAL_PTHREAD_DETACH_FAIL 0x81
#define FATAL_PTHREAD_STACK_FAIL 0x82
#define FATAL_PTHREAD_CREATE_FAIL 0x83
#define FATAL_APP_QUEUE 0x84
#define FATAL_WIFI_INIT 0x85
#define FATAL_TIMER 0x86
#define FATAL_SUB 0x87
#define FATAL_CONNECT 0x88
#define FATAL_TIMER70_INIT 0x89
#define FATAL_TIMER70_START 0x8A
#define FATAL_TIMER70_ADC 0x8B
#define FATAL_CHAIN_QUEUE 0x8C
#define FATAL_CHAIN_QUEUE_SEND 0x8D
#define FATAL_CHAIN_INIT 0x8E

#define FATAL_RTOS_MALLOC 0x90
#define FATAL_RTOS_STACK 0x91

//////////////////////////////////////////////////////////////////
// DEBUG VALUES
// UART_THREAD 0
/*
#define UART_THREAD_TASK_ENTER                      0x00
#define UART_THREAD_WHILE_LOOP_ENTER                0x01
#define UART_THREAD_BEFORE_RECEIVE                  0x02
#define UART_THREAD_AFTER_RECEIVE                   0x03
#define UART_CREATE_ENTER 0x4
#define UART_CREATE_SUCCESS 0x5
// UART_QUEUE 1
#define FAILURE_TO_CREATE_QUEUE                     0x10
#define FAILURE_TO_READ_QUEUE_ISR                   0x11
#define FAILURE_TO_READ_QUEUE_NON_ISR               0x12
#define CREATED_QUEUE                               0x13
#define READ_MESSAGE_ISR                            0x14
#define READ_MESSAGE_NON_ISR                        0x15
#define SENT_MESSAGE_ISR                            0x16
#define SENT_MESSAGE_NON_ISR                        0X17
*/
// SENSOR_TASK 2
#define SENSOR_TASK_CREATE_ENTER                    0x20
#define SENSOR_TASK_CREATE_SUCCESS                  0x21
#define SENSOR_TASK_INIT                            0x22
#define SENSOR_TASK_PRE_WHILE                       0x23
#define SENSOR_TASK_READ_ATTEMPT                    0x24
#define SENSOR_TASK_READ_SUCCESS                    0x25
#define SENSOR_TASK_READ_EMPTY                      0x26
#define SENSOR_TASK_POST_FSM_UPDATE                 0x27
// SENSOR_THREAD_QUEUE 3
#define SENSOR_THREAD_QUEUE_CREATE_ENTER            0x30
#define SENSOR_THREAD_QUEUE_CREATE_SUCCESS          0x31
#define SENSOR_THREAD_QUEUE_READ_ENTER              0x32
#define SENSOR_THREAD_QUEUE_READ_ISR                0x33
#define SENSOR_THREAD_QUEUE_READ_ISR_SUCCESS        0x34
#define SENSOR_THREAD_QUEUE_READ_NOT_ISR            0x35
#define SENSOR_THREAD_QUEUE_READ_NOT_ISR_SUCCESS    0x36
#define SENSOR_THREAD_QUEUE_SEND_ENTER              0x37
#define SENSOR_THREAD_QUEUE_SEND_ISR                0x38
#define SENSOR_THREAD_QUEUE_SEND_ISR_SUCCESS        0x39
#define SENSOR_THREAD_QUEUE_SEND_NOT_ISR            0x3A
#define SENSOR_THREAD_QUEUE_SEND_NOT_ISR_SUCCESS    0x3B
// SENSOR_THREAD_STATE 4
#define SENSOR_THREAD_STATE_ENTER                   0x40
#define SENSOR_THREAD_STATE_INIT                    0x41
#define SENSOR_THREAD_STATE_INIT_500                0x42
#define SENSOR_THREAD_STATE_UPDATE                  0x43
#define SENSOR_THREAD_STATE_UPDATE_70               0x44
#define SENSOR_THREAD_STATE_UPDATE_70_SEND          0x45
#define SENSOR_THREAD_STATE_UPDATE_70_POST_SEND     0x46
#define SENSOR_THREAD_STATE_UPDATE_500              0x47
#define SENSOR_THREAD_STATE_UPDATE_500_SEND         0x48
#define SENSOR_THREAD_STATE_UPDATE_500_POST_SEND    0x49

// TIMER500 5
#define FAILURE_TO_INITIALIZE_TIMER500              0x50
#define FAILURE_TO_START_TIMER500                   0x51
#define CURRENT_TIME_ISR                            0x53
#define CURRENT_TIME_NON_ISR                        0x54
#define COMPLETED_TIMERCALLBACK500                  0x55
#define ENTER_TIMER500_CALLBACK 0x56

// TIMER70 6
#define FAILURE_TO_INITIALIZE_TIMER70               0x60
#define FAILURE_TO_START_TIMER70                    0x61
#define FAILURE_TO_START_ADC                        0x62
#define FAILURE_TO_READ_ADC                         0x63
#define COMPLETED_TIMERCALLBACK70                   0x64
#define ENTER_TIMER70_CALLBACK 0x66



void dbgInit();
void dbgEvent(unsigned int event);

#endif
