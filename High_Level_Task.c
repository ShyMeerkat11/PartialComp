/*
 * High_Level_Task.c
 *
 *  Created on: Apr 14, 2021
 *      Author: drew1
 */


#include "High_Level_Task.h"

static mqd_t QHandle;

void init_high_level_thread( MQTTClient_Handle handle ) {
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

    retc |= pthread_attr_setstacksize(&attrs, HIGHLEVELTHREADSTACKSIZE);
    if (retc != 0) {
        //fail
    }

    priParam.sched_priority = PRIORITY;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread, &attrs, high_level_task, NULL); // @suppress("Invalid arguments")
    if (retc != 0) {
        dbgEvent( FATAL_CHAIN_INIT );
    }

    QHandle = handle;

    //dbgEvent
}

void* high_level_task() {
    highLevelMessage message_in;
    Servo_PWM_Message message_out;
    static int angle;
    static double distance;
    static int complete;
    static int pwm1;
    static int pwm2;
    static int pwm3;
    static int moveCount = 0;
    static int presetAng[6] = {0, 0, 0, 180, 180, 90};
    static double presetDist[6]= {0, 0, 0.1, 0.1, 0.1, 0.1};
    static int clawState[6] = {OPEN_CLAW, CLOSED_CLAW, CLOSED_CLAW, CLOSED_CLAW, OPEN_CLAW, OPEN_CLAW};
    while (1) {
        readHighLevelMessage( &message_in );
        angle = message_in.angle;
        distance = message_in.distance;
        complete = message_in.complete;
        if((angle <= 180 && angle >= 0 && distance >= 0 && distance <= 5) || (angle == -1 && distance == -1 && complete == 1)) {
            if(distance == -1 && angle == -1 && complete == 1) {
                if(moveCount == 6) {
                    moveCount = 0;
                }
                else {
                    pwm1 = (int)(((2100.0/180.0)*(float)presetAng[moveCount])+500);
                    //Cubic order Equation for Distance
                    pwm2 = (int)((16.18807*pow(presetDist[moveCount], 3.00))-(93.421923*pow(presetDist[moveCount], 2.00))+((270.10013*presetDist[moveCount])+1107.793747));
                    pwm3 = clawState[moveCount];
                    LOG_TRACE("pwm1 = %d \r\n", pwm1); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                    LOG_TRACE("pwm2 = %d \r\n", pwm2); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                    LOG_TRACE("pwm3 = %d \r\n", pwm3); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                    message_out.angle1 = pwm1;
                    message_out.angle2 = pwm2;
                    message_out.angle3 = pwm3;
                    sendServoPWMMessage(&message_out);
                    moveCount++;
                }
            }
            else {
                //First Order Equation for Angles
                pwm1 = (int)(((2100.0/180.0)*(float)angle)+500);
                //Cubic order Equation for Distance
                pwm2 = (int)((16.18807*pow(distance, 3.00))-(93.421923*pow(distance, 2.00))+((270.10013*distance)+1107.793747));
                pwm3 = OPEN_CLAW;
                LOG_TRACE("pwm1 = %d \r\n", pwm1); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                LOG_TRACE("pwm2 = %d \r\n", pwm2); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                message_out.angle1 = pwm1;
                message_out.angle2 = pwm2;
                message_out.angle3 = pwm3;
                sendServoPWMMessage(&message_out);
                presetAng[0] = angle;
                presetAng[1] = angle;
                presetAng[2] = angle;
                presetDist[0] = distance;
                presetDist[1] = distance;
                //Close Claw at Position

            }
        }
    }
}
