/*
 * timer1000.c
 *
 *  Created on: Mar 3, 2021
 *      Author: drew1
 */

#include "timer1000.h"
#include "Task1_V2_Queue.h"
#include "Servo_PWM_Queue.h"
void timer1000Setup(void *arg0)
{
    Timer_Handle timer1;
    Timer_Params params;

    /* Call driver init functions */
    Timer_init();
    /*
     * Setting up the timer in continuous callback mode that calls the callback
     * function every 1000000 microseconds, or 1000 ms.
     */
    Timer_Params_init(&params);
    params.period = TIMER1000_LENGTH;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback1000;

    timer1 = Timer_open(CONFIG_TIMER_1, &params);

    if (timer1 == 0) {
        /* Failed to initialized timer */
        while (1) {
            //Debug
            dbgEvent(FAILURE_TO_INITIALIZE_TIMER500);
        }
    }

    if (Timer_start(timer1) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {
            //Debug
            dbgEvent(FAILURE_TO_START_TIMER500);
        }
    }
}


void timerCallback1000(Timer_Handle myHandle, int_fast16_t status)
{
    //static version2_t message
//    static Servo_PWM_Message message;
//    message.angle1 = -1;
//    message.angle2 = -1;
//    message.angle3 = -1;
//    message.hash = 0;
    //message.sendMes = SEND_MESSAGE;
    //message.avgSensReadings = TIMER1000_SEND2;
    //message.messagesPerSecond = TIMER1000_SEND1;
    //message.totalNumMes = TIMER1000_SEND1;
    //sendTask1V2Message( &message );
    //static StatsMessage_t stats;
    //stats.type = STATS_SEND;
    dbgEvent( ENTER_TIMER500_CALLBACK );
    //sendStatsMessage( &stats );
//    sendServoPWMMessage(&message);
}


