#include "chain_task.h"

static mqd_t QHandle;

void init_chain_thread( MQTTClient_Handle handle ) {
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

    retc |= pthread_attr_setstacksize(&attrs, CHAINTHREADSTACKSIZE);
    if (retc != 0) {
        //fail
    }

    priParam.sched_priority = PRIORITY;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread, &attrs, chain_task, NULL); // @suppress("Invalid arguments")
    if (retc != 0) {
        dbgEvent( FATAL_CHAIN_INIT );
    }

    QHandle = handle;

    //dbgEvent
}

void* chain_task() {
    //dbgEvent
    static ChainMessage_t          message_in;
    static msgQueue                message_out;
    static int                     count = 0;
    static int                     return_size;
    static unsigned int            hash;
    static int                     i;
    static StatsMessage_t          stats;
    static bool                    valid;


    message_out.event = 0;
    return_size = snprintf( message_out.topic, TOPIC_STRING_LENGTH, "Chain/3");
    message_out.topic[return_size] = 0;

    while(1) {
        dbgEvent( CHAIN_TASK_RECEIVE );
        valid = readChainMessage( &message_in );

        // test count
        if( count + ADD_THREE != message_in.count ) {
            stats.type = STATS_MISSED;
            valid = false;
            count = message_in.count + ADD_THREE;
        }
        else {
            count = message_in.count + ADD_THREE;
        }

        return_size = snprintf( message_out.payload, PAYLOAD_STRING_LENGTH, "{ \"Count\" : %d , ", message_in.count );
        hash = string_hash( message_out.payload );
        // test hash
        if( message_in.hash != hash ) {
            stats.type = STATS_CORRUPTED;
            valid = false;
        }

        return_size = snprintf( message_out.payload, PAYLOAD_STRING_LENGTH, "{ \"Count\" : %d , ", count );
        hash = string_hash( message_out.payload );
        return_size = snprintf( message_out.payload, PAYLOAD_STRING_LENGTH, "%s\"Hash\" : %u , \"Filler\" : \"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris mollis ligula finibus, maximus mauris eu, congue tortor. Praesent blandit lacus sit amet dui ornare auctor. Aenean tristique, massa et molestie elementum\" }", message_out.payload, hash );
        message_out.payload[return_size] = 0;
        message_out.payload_size = return_size;

        if( valid ) {
            stats.type = STATS_RECEIVE;
        }

        sendStatsMessage( &stats );

        dbgEvent( CHAIN_TASK_SEND );
        //if( valid ) {
            return_size = mq_send( QHandle, (char*)&message_out, sizeof(struct msgQueue), 0);
        //}

        for( i = 0 ; i < PAYLOAD_STRING_LENGTH ; i++ ) {
            message_out.payload[i] = 0;
        }

    }
}
