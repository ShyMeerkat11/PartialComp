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
    uint16_t   duty1 = 1600;
    uint16_t   duty2 = 1000;
    uint16_t   duty3 = 2000;
    int   dutyInc1 = INCREMENT;
    int   dutyInc2 = INCREMENT;
    int   dutyInc3 = INCREMENT;

    //Sleep time in microseconds
    //uint32_t   time = 40000;
    PWM_Handle pwm1 = NULL;
    PWM_Handle pwm2 = NULL;
    PWM_Handle pwm3 = NULL;
    PWM_Params params;

    // Call driver init functions.
    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;

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
    //LOG_TRACE("Before While Loop PWM Task\r\n");
    // Loop forever incrementing the PWM duty
    int armState = IDLE;
    int angle1;
    int angle2;
    int angle3;
    PWM_setDuty(pwm1, duty1);
    PWM_setDuty(pwm2, duty2);
    PWM_setDuty(pwm3, duty3);
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
        if (armState == IDLE) {
            //LOG_TRACE("I read PWM message\r\n");
            angle1 = message.angle1;
            angle2 = message.angle2;
            angle3 = message.angle3;

            if (checkViableAngles(angle1, angle2, angle3)) {
                setDirections(angle1, angle2, angle3, &dutyInc1, &dutyInc2, &dutyInc3, duty1, duty2, duty3);
                armState = MOVING;
                viable = VIABLE;
                numMoves++;
            }
            else if(angle1 != -1 && angle2 != -1 && angle3 != -1) {
                //Send message saying unviable angles
                //LOG_TRACE("Not in range of motion \r\n");
                viable = NOT_VIABLE;
                numNonViableMoves++;
            }
        }
        else {
            if (angle1 <= duty1 + INCREMENT && angle1 >= duty1 - INCREMENT)
            {
                duty1 = angle1;
                PWM_setDuty(pwm1, duty1);
            }
            else if(angle1 != duty1) {
                duty1 = duty1 + dutyInc1;
                PWM_setDuty(pwm1, duty1);
            }
            if (angle1 <= duty1 + INCREMENT && angle1 >= duty1 - INCREMENT)
            {
                duty2 = angle2;
                PWM_setDuty(pwm2, duty2);
            }
            else if(angle2 != duty2) {
                duty2 = duty2 + dutyInc2;
                PWM_setDuty(pwm2, duty2);
            }
            if(angle3 != duty3) {
                duty3 = duty3 + dutyInc3;
                PWM_setDuty(pwm3, duty3);
            }
            if(angle1 == duty1 &&angle2 == duty2 && angle3 == duty3) {
                armState = IDLE;
                //send done message to the high level task.
                doneMessage.angle = -1;
                doneMessage.complete = 1;
                doneMessage.distance = -1;
                sendHighLevelMessage(&doneMessage);
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
void setDirections(int angle1, int angle2, int angle3, int *dutyInc1, int *dutyInc2, int *dutyInc3, uint16_t duty1, uint16_t duty2, uint16_t duty3) {
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
}
bool checkViableAngles(int angle1, int angle2, int angle3) {
    bool viable = true;
    if(angle1 > 2600 || angle1 < 500)
        viable = false;
    if(angle2 > 2200 || angle2 < 1100)
        viable = false;
    if(angle3 > 2000 || angle3 < 1400)
        viable = false;
    return viable;

}
