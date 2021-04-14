    #include "statistic_task.h"

static mqd_t QHandle;

void init_stats_thread( MQTTClient_Handle handle ) {
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

    retc = pthread_create(&thread, &attrs, stats_task, NULL); // @suppress("Invalid arguments")
    if (retc != 0) {
        dbgEvent( FATAL_CHAIN_INIT );
    }

    QHandle = handle;

    //dbgEvent
}

void* stats_task() {
    static StatsMessage_t message_in;
    static msgQueue message_out;
    static unsigned int published = 0;
    static unsigned int received = 0;
    static unsigned int missed = 0;
    static unsigned int corrupted = 0;
    static int return_size;
    static int i;

    while(1) {
        readStatsMessage( &message_in );
        //LOG_TRACE("Read Stats Message\r\n");
        //LOG_TRACE("MESSAGE_IN.type %d \r\n",message_in.type);
        switch( message_in.type ) {
        case STATS_PUBLISH:
            published++;
            break;
        case STATS_RECEIVE:
            received++;
            break;
        case STATS_MISSED:
            missed++;
            break;
        case STATS_CORRUPTED:
            corrupted++;
            break;
        case STATS_SEND:
            message_out.event = 0;
            return_size = snprintf( message_out.topic, TOPIC_STRING_LENGTH, "Stats/Andrew");
            message_out.topic[return_size] = 0;
            return_size = snprintf( message_out.payload, PAYLOAD_STRING_LENGTH, "{ \"Published\" : %d , \"Received\" : %d , \"Missed\" : %d , \"Corrupted\" : %d }", published, received, missed, corrupted);
            message_out.payload[return_size] = 0;
            message_out.payload_size = return_size;

            return_size = mq_send( QHandle, (char*)&message_out, sizeof(struct msgQueue), 0);

            for( i = 0 ; i < PAYLOAD_STRING_LENGTH ; i++ ) {
                message_out.payload[i] = 0;
            }
            published = 0;
            received = 0;
            missed = 0;
            corrupted = 0;

        }
    }

}
