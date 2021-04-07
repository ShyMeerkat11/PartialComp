#ifndef MQTT_PUBLISH_QUEUE_DEF_H
#define MQTT_PUBLISH_QUEUE_DEF_H

typedef enum {
    SENSOR_TOPIC,
    CALC_0_TOPIC,
    CALC_1_TOPIC,
    CHAIN_1_TOPIC,
    CHAIN_2_TOPIC,
    CHAIN_3_TOPIC,
} MQTT_topic_e;


#define TOPIC_STRING_LENGTH 50
#define PAYLOAD_STRING_LENGTH 300

typedef struct msgQueue
{
    int           event;
    char          topic[TOPIC_STRING_LENGTH];
    char          payload[PAYLOAD_STRING_LENGTH];
    int           payload_size;
} msgQueue;

#endif
