/*
 * Timer_40_SW.c
 *
 *  Created on: Apr 14, 2021
 *      Author: drew1
 */
#include "Timer_40_SW.h"


#define NUM_TIMERS 5

 /* An array to hold handles to the created timers. */


 /* Define a callback function that will be used by multiple timer
 instances.  The callback function does nothing but count the number
 of times the associated timer expires, and stop the timer once the
 timer has expired 10 times.  The count is saved as the ID of the
 timer. */
 void vTimerCallback( TimerHandle_t xTimer )
 {
     uint32_t ulCount;

    /* Optionally do something if the pxTimer parameter is NULL. */
    //configASSERT( xTimer );

    /* The number of times this timer has expired is saved as the
    timer's ID.  Obtain the count. */
    ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );

    /* Increment the count, then test to see if the timer has expired
    ulMaxExpiryCountBeforeStopping yet. */
    ulCount++;
    static Servo_PWM_Message message;
    message.angle1 = -1;
    message.angle2 = -1;
    message.angle3 = -1;
    message.angle4 = -1;
    message.hash = 0;
    sendServoPWMMessage(&message);
   /* Store the incremented count back into the timer's ID field
   so it can be read back again the next time this software timer
   expires. */
   vTimerSetTimerID( xTimer, ( void * ) ulCount );

 }

 void swTimerSetup(void)
 {
     static  TimerHandle_t xTimer;
     /* Create then start some timers.  Starting the timers before
     the RTOS scheduler has been started means the timers will start
     running immediately that the RTOS scheduler starts. */
     xTimer = xTimerCreate
               ( /* Just a text name, not used by the RTOS
                 kernel. */
                 "Timer",
                 /* The timer period in ticks, must be
                 greater than 0. */
                 pdMS_TO_TICKS( 20 ),
                 /* The timers will auto-reload themselves
                 when they expire. */
                 pdTRUE,
                 /* The ID is used to store a count of the
                 number of times the timer has expired, which
                 is initialised to 0. */
                 ( void * ) 0,
                 /* Each timer calls the same callback when
                 it expires. */
                 vTimerCallback
               );

     if( xTimer == NULL )
     {
         /* The timer was not created. */
     }
     else
     {
         if( HwiP_inISR() ) {
             BaseType_t xHigherPriorityTaskWoken = pdFALSE;
             /* Start the timer.  No block time is specified, and
             even if one was it would be ignored because the RTOS
             scheduler has not yet been started. */
             if(xTimerStartFromISR( xTimer,
                                    &xHigherPriorityTaskWoken ) != pdPASS )
             {
                 /* The timer could not be set into the Active
                 state. */
             }
         }
         else {
             if( xTimerStart( xTimer, 0 ) != pdPASS )
              {
                  /* The timer could not be set into the Active
                  state. */
              }
         }
     }

     /* ...
     Create tasks here.
     ... */

     /* Starting the RTOS scheduler will start the timers running
     as they have already been set into the active state. */
     //vTaskStartScheduler();

     /* Should not reach here. */
     //for( ;; );
 }

