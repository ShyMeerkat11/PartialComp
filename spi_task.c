/*
 * spi_task.c
 *
 *  Created on: Apr 26, 2021
 *      Author: drew1
 */
#include "spi_task.h"

static mqd_t QHandle;

void init_spi_task_thread( MQTTClient_Handle handle ) {
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

    retc = pthread_create(&thread, &attrs, spi_task, NULL); // @suppress("Invalid arguments")
    if (retc != 0) {
        dbgEvent( FATAL_CHAIN_INIT );
    }

    QHandle = handle;

    //dbgEvent
}

void* spi_task() {
    SPI_init();

    static SPI_Handle      masterSpi;
    static SPI_Params      spiParams;
    static SPI_Transaction transaction;
    bool            transferOK;
    double dTheta;
    double x;
    double y;
    static int i = 0;
    //unsigned char transmitBuffer[SPI_MSG_LENGTH];
    static unsigned char masterRxBuffer[SPI_MSG_LENGTH];
    //LOG_TRACE("STUPID, SO DUMMY\"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    SPI_Params_init(&spiParams);
    GPIO_setConfig(CONFIG_GPIO_SS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.bitRate = 20000000;

    spiParams.transferMode = SPI_MODE_BLOCKING;
    spiParams.mode = SPI_MASTER;
    transaction.count = SPI_MSG_LENGTH;
    transaction.rxBuf = (void *) masterRxBuffer;

    char parsedData[8][SPI_MSG_LENGTH] = {{0}};
    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);
    timer40Message message_in;
    highLevelMessage high_level_message_out;

    if (masterSpi == NULL) {
        LOG_TRACE("SPI was not initialized"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    }

    //while(1) {
        //ReadInColorQueue(&color_message_in);
        while(1) {
            readTimer40Message(&message_in);
            if(message_in.getCamData == 1){
                //LOG_TRACE("In While loop"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")

                //Start Slave Select
                GPIO_write(CONFIG_GPIO_SS, 0);
                transferOK = SPI_transfer(masterSpi, &transaction);
                //End Slave Select
                GPIO_write(CONFIG_GPIO_SS, 1);
                LOG_TRACE("Spi Received %s \r\n", masterRxBuffer); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")


                /* Ideas #include <string.h>
                if (strstr(request, "color of interest") != NULL) {
                    //contains color of interest
                    //now parse the data for x, y, and theta
                    sendHighLevelMessage( &high_level_message_out );
                    break;
                }
                else {
                    //doesn't contain object of interest increment theta and keep x and y

                    sendHighLevelMessage( &high_level_message_out );
                }
                */
            }
        }

    //}
}
