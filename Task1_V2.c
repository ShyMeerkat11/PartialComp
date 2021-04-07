/*
 * TaskV1_V2.c
 *
 *  Created on: Mar 1, 2021
 *      Author: drew1
 */

#include "Task1_V2.h"

static mqd_t QHandle;

void init_Task1_V2_thread( MQTTClient_Handle handle ) {
    //dbgEvent
    pthread_t thread;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        // fail
    }

    retc |= pthread_attr_setstacksize(&attrs, STATSTHREADSTACKSIZE);
    if (retc != 0) {
        //fail
    }

    priParam.sched_priority = PRIORITY;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread, &attrs, Task1_V2_task, NULL);
    if (retc != 0) {
        dbgEvent( FATAL_CHAIN_INIT );
    }

    QHandle = handle;

    //dbgEvent
}

void* Task1_V2_task() {
    //LOG_INFO("Start of T1V2Task\r\n");
    static version2_t message_in;
    static msgQueue message_out;
    static int messagesPerSecond = 0;
    static int return_size;
    static int prevReading = 0;
    static int i;
    static int average = 0;
    static int totalReadings= 0;
    static int messagesPerSecondPrint;
    message_out.event = 0;
    return_size = snprintf( message_out.topic, TOPIC_STRING_LENGTH, "Task1V2/Andrew");
    message_out.topic[return_size] = 0;
    static unsigned int            hash;

    static StatsMessage_t stats;
    while(1) {
        //LOG_TRACE("Before read\r\n");
        //Change to Timer
        //Timer send task1v2
        //Add data to indicate if sent from timer
        readTask1V2Message( &message_in );
        if( message_in.sendMes == SEND_MESSAGE) {
            //LOG_TRACE()
            //LOG_TRACE("messagesPersecond = %d \r\n", messagesPerSecond);
            //LOG_TRACE("PrevReading = %d \r\n", prevReading);
            messagesPerSecondPrint= messagesPerSecond - prevReading;
            prevReading = messagesPerSecond;
            return_size = snprintf( message_out.payload, PAYLOAD_STRING_LENGTH, "{ \"Messages/Second\" : %d , \"Total Sensor Readings\" : %d , \"Avg Sensor Reading\" : %d }", messagesPerSecondPrint, totalReadings, average);
            message_out.payload[return_size] = 0;
            message_out.payload_size = return_size;
            return_size = mq_send( QHandle, (char*)&message_out, sizeof(struct msgQueue), 0);
            for( i = 0 ; i < PAYLOAD_STRING_LENGTH ; i++ ) {
                message_out.payload[i] = 0;
            }
        }
        else {
            //Set Variables
            stats.type = STATS_RECEIVE;
            messagesPerSecond = message_in.messagesPerSecond;

            //Check For Missed Data
            if(totalReadings + 1 != message_in.totalNumMes) {
                stats.type = STATS_MISSED;
                totalReadings = message_in.totalNumMes;
            }


            /*LOG_TRACE("Average = %d \r\n", message_in.avgSensReadings);
            LOG_TRACE("Count = %d \r\n", message_in.totalNumMes);
            LOG_TRACE("Time = %d \r\n", message_in.time);
            LOG_TRACE("Value = %d \r\n", message_in.value);
            */
            return_size = snprintf(message_out.payload, PAYLOAD_STRING_LENGTH, "{ \"Average\" : %d , \"Count\" : %d , \"Time\" : %d , \"Value\" : %d , ", message_in.avgSensReadings,message_in.totalNumMes, message_in.time, message_in.value);
            hash = string_hash(message_out.payload);
            //LOG_TRACE("Message Hash = %d \r\n", message_in.hash);
            LOG_TRACE("Calculated Hash = %u \r\n", hash);

            if( message_in.hash != hash ) {
                stats.type = STATS_CORRUPTED;
            }
            else {
                totalReadings = message_in.totalNumMes;
                messagesPerSecond = message_in.messagesPerSecond;
                if(message_in.avgSensReadings != AVG_ERROR) {
                    average = message_in.avgSensReadings;
                }
            }

            message_out.payload[return_size] = 0;
            message_out.payload_size = return_size;
            for( i = 0 ; i < PAYLOAD_STRING_LENGTH ; i++ ) {
                message_out.payload[i] = 0;
            }

            sendStatsMessage( &stats );

        }

    }
}





