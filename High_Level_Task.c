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
    static double x;
    static double y;
    static int complete;
    static double angle1;
    static double angle2;
    static double angle3;
    static double angle4;
    static int moveCount = 1;
    static double L1 = 3.1875;
    static double L2 = 3.1875;
    static double val = 180.0 / PI;
    static int presetAng[9] = {  0, 0, 0,  0, 180, 180, 180, 180, 90};
    static double presetX[9] = {90, 0, 0, 90,  90,  0,  0,  90, 90};
    static double presetY[9] = {90, 0, 0, 0,    0,  0,  0,  90, 90};
    static int clawState[9] = {OPEN_CLAW, OPEN_CLAW, CLOSED_CLAW, CLOSED_CLAW, CLOSED_CLAW, CLOSED_CLAW, OPEN_CLAW, OPEN_CLAW, OPEN_CLAW};
    while (1) {
        readHighLevelMessage( &message_in );
        angle = message_in.angle;
        x = message_in.x;
        y = message_in.y;
        complete = message_in.complete;
        if(x == -1 && angle == -1 && complete == 1) {
            if(moveCount == 9) {
                moveCount = 1;
                //Send Done
            }
            else {
                message_out.angle1 = presetAng[moveCount];
                message_out.angle2 = presetX[moveCount];
                message_out.angle3 = clawState[moveCount];
                message_out.angle4 = presetY[moveCount];
                sendServoPWMMessage(&message_out);
                moveCount++;
            }
        }
        else if(angle <= 180 && angle >= 0 && x >= 0 && x <= (L1+L2) && y >= -L1 && y <= L1){

            //LOG_TRACE("ELSE"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
            //LOG_TRACE("angle1 = %d \r\n", angle1); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
            //LOG_TRACE("angle2 = %d \r\n", angle2); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
            //LOG_TRACE("angle3 = %d \r\n", angle3); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
            //LOG_TRACE("angle4 = %d \r\n", angle4); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
            angle1 = angle;
            angle3 = 0.00;
            if(x == 0 && y == 0) {
                angle2 = 90.00;
                angle4 = 90.00;
            }
            else{
               angle4 = atan2(x,y) - atan2((pow(x,2)+pow(y,2))/(2*L2), sqrt(pow(x,2)+pow(y,2)-pow(((pow(x,2)+pow(y,2))/(2*L2)),2)));
               angle2 = acos((x-(L2*cos(angle4)))/L1);
               angle2 = angle2*val;
               angle4 = angle4*val;

           }
           if( angle4 >= 0 && angle <= 90 && angle2 >= 0 && angle2 <= 90) {
               message_out.angle1 = angle1;
               message_out.angle2 = presetX[0];
               message_out.angle3 = clawState[0];
               message_out.angle4 = presetY[0];
               LOG_TRACE("angle1 = %f \r\n", angle1); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
               LOG_TRACE("angle2 = %f \r\n", angle2); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
               LOG_TRACE("angle3 = %f \r\n", angle3); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
               LOG_TRACE("angle4 = %f \r\n", angle4); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
               sendServoPWMMessage(&message_out);
               presetAng[0] = angle1;
               presetAng[1] = angle1;
               presetAng[2] = angle1;
               presetAng[3] = angle1;
               presetX[1] = angle2;
               presetX[2] = angle2;
               presetY[1] = angle4;
               presetY[2] = angle4;
           }
        }
    }
}
