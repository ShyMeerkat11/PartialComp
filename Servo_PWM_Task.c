/*
 * Servo_PWM_Task.c
 *
 *  Created on: Apr 4, 2021
 *      Author: drew1
 */
#include "Servo_PWM_Task.h"
#define IDLE 0
#define MOVING 1
#define INCREMENT 25
#define VIABLE 1
#define NOT_VIABLE 0
static mqd_t QHandle;

void init_servo_PWM_thread( MQTTClient_Handle handle ) {
    //dbgEvent( SENSOR_TASK_CREATE_ENTER );
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

    retc |= pthread_attr_setstacksize(&attrs, SENSORTHREADSTACKSIZE);
    if (retc != 0) {
        //fail
    }

    priParam.sched_priority = PRIORITY;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread, &attrs, servo_PWM_task, NULL); // @suppress("Invalid arguments")
    if (retc != 0) {
        //fail
    }

    QHandle = handle;

    //dbgEvent( SENSOR_TASK_CREATE_SUCCESS );
}

void* servo_PWM_task() {
    //LOG_TRACE("Start Servo PWM Task\r\n");
    //dbgEvent( SENSOR_TASK_INIT );
    // initialization code
    Servo_PWM_Message message;

    //dbgEvent( SENSOR_TASK_PRE_WHILE );
    //while(1) {
        //dbgEvent( SENSOR_TASK_READ_ATTEMPT );
        // Make one subroutine call to receive message from sensor_thread_queue
        //readSensorMessage( &message );

        // Send message (by reference) to FSM in sensor_thread_state
        //dbgEvent( SENSOR_TASK_READ_SUCCESS );
        //update_FSM( &message, QHandle );
        //dbgEvent( SENSOR_TASK_POST_FSM_UPDATE );

        // do nothing if queue is empty

    //}
    // Period and duty in microseconds
    uint16_t   pwmPeriod = 20000;
    uint16_t   duty1 = 1550;
    uint16_t   duty2 = 1400;
    uint16_t   duty3 = 2400;
    uint16_t   duty4 = 700;
    int   dutyInc1 = INCREMENT;
    int   dutyInc2 = INCREMENT;
    int   dutyInc3 = INCREMENT;
    int   dutyInc4 = INCREMENT;
    //Sleep time in microseconds
    //uint32_t   time = 40000;
    PWM_Handle pwm1 = NULL;
    PWM_Handle pwm2 = NULL;
    PWM_Handle pwm3 = NULL;
    PWM_Handle pwm4 = NULL;
    PWM_Params params;

    // Call driver init functions.
    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    params.idleLevel = PWM_IDLE_LOW;
    pwm1 = PWM_open(CONFIG_PWM_0, &params);
    if (pwm1 == NULL) {
        // CONFIG_PWM_0 did not open
        while (1)
            LOG_TRACE("PWM DIDN'T OPEN\r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    } // @suppress("Function cannot be resolved")
    PWM_start(pwm1);

    pwm2 = PWM_open(CONFIG_PWM_1, &params);
    if (pwm2 == NULL) {
        // CONFIG_PWM_1 did not open
        while (1)
            LOG_TRACE("PWM DIDN'T OPEN\r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    } // @suppress("Function cannot be resolved")
    PWM_start(pwm2);

    pwm3 = PWM_open(CONFIG_PWM_2, &params);
    if (pwm3 == NULL) {
        // CONFIG_PWM_2 did not open
        while (1)
            LOG_TRACE("PWM DIDN'T OPEN\r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    }
    PWM_start(pwm3);

    pwm4 = PWM_open(CONFIG_PWM_3, &params);
    if (pwm4 == NULL) {
        while(1)
            LOG_TRACE("PWM DIDN'T OPEN\r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    }
    PWM_start(pwm4);
    //LOG_TRACE("Before While Loop PWM Task\r\n");
    // Loop forever incrementing the PWM duty
    int armState = IDLE;
    int pwmAngle1;
    int pwmAngle2;
    int pwmAngle3;
    int pwmAngle4;
    PWM_setDuty(pwm1, duty1);
    PWM_setDuty(pwm2, duty2);
    PWM_setDuty(pwm3, duty3);
    PWM_setDuty(pwm4, duty4);
    static msgQueue message_out;
    static int return_size;
    message_out.event = 0;
    return_size = snprintf( message_out.topic, TOPIC_STRING_LENGTH, "Arm/Status");
    message_out.topic[return_size] = 0;
    int viable;
    static int numMoves = 0;
    static int numNonViableMoves = 0;
    static int messageEverySec = 0;
    static int i;
    static highLevelMessage doneMessage;
    while (1) {
        readServoPWMMessage( &message );
        messageEverySec++;
//        LOG_TRACE("angle1 = %d \r\n", message.angle1); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
//        LOG_TRACE("angle2 = %d \r\n", message.angle2); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
//        LOG_TRACE("angle3 = %d \r\n", message.angle3); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
//        LOG_TRACE("angle4 = %d \r\n", message.angle4); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
        if (armState == IDLE) {
            //LOG_TRACE("I read PWM message\r\n");
            pwmAngle1 = (int)(((2100.0/180.0)*(float)message.angle1)+500);
            pwmAngle2 = (int)((-(1000.00/90.00)*(float)message.angle2)+2400);
            pwmAngle3 = (int)(((1000.00/90.00)*(float)message.angle3)+1400);
            pwmAngle4 = (int)((-(1000.00/90.00)*(float)message.angle4)+1700);

            if (checkViableAngles(pwmAngle1, pwmAngle2, pwmAngle3, pwmAngle4)) {
                setDirections(pwmAngle1, pwmAngle2, pwmAngle3, pwmAngle4, &dutyInc1, &dutyInc2, &dutyInc3, &dutyInc4, duty1, duty2, duty3, duty4);
                armState = MOVING;
                viable = VIABLE;
                numMoves++;
            }
            else if(pwmAngle1 != -1 && pwmAngle2 != -1 && pwmAngle3 != -1 && pwmAngle4 != -1) {
                //Send message saying unviable angles
                //LOG_TRACE("Not in range of motion \r\n");
                viable = NOT_VIABLE;
                numNonViableMoves++;
            }
        }
        else {
            if (pwmAngle1 <= duty1 + INCREMENT && pwmAngle1 >= duty1 - INCREMENT)
            {
                duty1 = pwmAngle1;
                PWM_setDuty(pwm1, duty1);
            }
            else if(pwmAngle1 != duty1) {
                duty1 = duty1 + dutyInc1;
                PWM_setDuty(pwm1, duty1);
            }
            if (pwmAngle2 <= duty2 + INCREMENT && pwmAngle2 >= duty2 - INCREMENT)
            {
                duty2 = pwmAngle2;
                PWM_setDuty(pwm2, duty2);
            }
            else if(pwmAngle2 != duty2) {
                duty2 = duty2 + dutyInc2;
                PWM_setDuty(pwm2, duty2);
            }
            if (pwmAngle3 <= duty3 + INCREMENT && pwmAngle3 >= duty3 - INCREMENT)
            {
                duty3 = pwmAngle3;
                PWM_setDuty(pwm3, duty3);
            }
            else if(pwmAngle3 != duty3) {
                duty3 = duty3 + dutyInc3;
                PWM_setDuty(pwm3, duty3);
            }
            if (pwmAngle4 <= duty4 + INCREMENT && pwmAngle4>= duty4 - INCREMENT)
            {
                duty4 = pwmAngle4;
                PWM_setDuty(pwm4, duty4);
            }
            else if(pwmAngle4 != duty4) {
                duty4 = duty4 + dutyInc4;
                PWM_setDuty(pwm4, duty4);
            }
            if(pwmAngle1 == duty1 &&pwmAngle2 == duty2 && pwmAngle3 == duty3 && pwmAngle4 == duty4) {
                armState = IDLE;
                //send done message to the high level task.
                doneMessage.angle = -1;
                doneMessage.complete = 1;
                doneMessage.x = -1;
                sendHighLevelMessage(&doneMessage);
                LOG_TRACE("pwm1 = %d \r\n", duty1); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                LOG_TRACE("pwm2 = %d \r\n", duty2); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                LOG_TRACE("pwm3 = %d \r\n", duty3); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                LOG_TRACE("pwm4 = %d \r\n", duty4); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
            }
        }



        if(messageEverySec == 25) {
            //LOG_TRACE("Armsate = %d \r\n", armState); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
            return_size = snprintf( message_out.payload, PAYLOAD_STRING_LENGTH, "{ \"Moving\" : %d , \"Viable Instruction\" : %d , \"Number Moves\" : %d , \"Number Non-Viable Moves\" : %d }", armState, viable, numMoves, numNonViableMoves );
            message_out.payload[return_size] = 0;
            message_out.payload_size = return_size;
            return_size = mq_send( QHandle, (char*)&message_out, sizeof(struct msgQueue), 0);
            for( i = 0 ; i < PAYLOAD_STRING_LENGTH ; i++ ) {
                        message_out.payload[i] = 0;
            }
            messageEverySec = 0;
        }
    }
}
void setDirections(int angle1, int angle2, int angle3, int angle4, int *dutyInc1, int *dutyInc2, int *dutyInc3, int *dutyInc4, uint16_t duty1, uint16_t duty2, uint16_t duty3, uint16_t duty4) {
    if(angle1 > duty1) {
        *dutyInc1 = INCREMENT;
    }
    else {
        *dutyInc1 = -INCREMENT;
    }
    if(angle2 > duty2) {
        *dutyInc2 = INCREMENT;
    }
    else {
        *dutyInc2 = -INCREMENT;
    }
    if(angle3 > duty3) {
        *dutyInc3 = INCREMENT;
    }
    else {
        *dutyInc3 = -INCREMENT;
    }
    if(angle4 > duty4) {
        *dutyInc4 = INCREMENT;
    }
    else {
        *dutyInc4 = -INCREMENT;
    }
}
bool checkViableAngles(int angle1, int angle2, int angle3, int angle4) {
    bool viable = true;
    if(angle1 > 2600 || angle1 < 500)
        viable = false;
    else if(angle2 > 2400 || angle2 < 1400)
        viable = false;
    else if(angle3 > 2400 || angle3 < 1400)
        viable = false;
    else if(angle4 > 1700 || angle4 < 700)
        viable = false;
    return viable;

}
