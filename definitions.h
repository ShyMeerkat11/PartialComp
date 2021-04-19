#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//////////////////////////////////////////////////////////////////////////////////////////////
// SENSOR TASK
typedef enum eSensorState_t {
    INIT_AVERAGE,
    UPDATE_AVERAGE
} eSensorState_t;
//////////////////////////////////////////////////////////////////////////////////////////////
// Servo task
typedef struct Servo_PWM_Message {
    double angle1;
    double angle2;
    double angle3;
    double angle4;
    unsigned int hash;
} Servo_PWM_Message;
//////////////////////////////////////////////////////////////////////////////////////////////
// SENSOR_THREAD_QUEUE
#define SENSOR_TIMER70_MESSAGE 1
#define SENSOR_TIMER500_MESSAGE 2
typedef struct SensorMessage_t {
    char message_type;
    int  value;
} SensorMessage_t;


//////////////////////////////////////////////////////////////////////////////////////////////
// SENSOR_THREAD_STATE
//#define MAX_SENSOR_STRING_SIZE 50

//////////////////////////////////////////////////////////////////////////////////////////////
// QUEUE_THREAD
#define MAX_STRING_SIZE 50
typedef struct UartMessage_t {
    int size;
    char string[ MAX_STRING_SIZE ];
} UartMessage_t;
#define QUEUE_LENGTH 3
#define QUEUE_ITEM_SIZE sizeof( UartMessage_t )

//////////////////////////////////////////////////////////////////////////////////////////////
// THREADS


//////////////////////////////////////////////////////////////////////////////////////////////
// CHAIN
typedef struct ChainMessage_t {
    int count;
    unsigned int hash;
} ChainMessage_t;

/////////////////////////////////////////////////////////////////////////////////////////////
// STATS
typedef enum eStatsType_t {
    STATS_PUBLISH,
    STATS_RECEIVE,
    STATS_MISSED,
    STATS_CORRUPTED,
    STATS_SEND
} eStatsType_t;

typedef struct StatsMessage_t {
    eStatsType_t type;
} StatsMessage_t;

/////////////////////////////////////////////////////////////////////////////////////////////
typedef struct version2_t {
    int totalNumMes;
    int messagesPerSecond;
    int avgSensReadings;
    int sendMes;
    unsigned int hash;
    int time;
    int value;
} version2_t;

typedef struct highLevelMessage {
    int angle;
    double x;
    double y;
    int complete;
} highLevelMessage;

#define AVG_ERROR -2
#endif

