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
    bool   transferOK;
    double dTheta;
    double x;
    double y;
    //unsigned char transmitBuffer[SPI_MSG_LENGTH];
    static char masterRxBuffer[SPI_MSG_LENGTH];
    //LOG_TRACE("STUPID, SO DUMMY\"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    SPI_Params_init(&spiParams);
    GPIO_setConfig(CONFIG_GPIO_SS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.bitRate = 20000000;
    char Nothing[] = "Nothing";
    spiParams.transferMode = SPI_MODE_BLOCKING;
    spiParams.mode = SPI_MASTER;
    transaction.count = SPI_MSG_LENGTH;
    transaction.rxBuf = (void *) masterRxBuffer;

    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);
    timer40Message message_in;
    static highLevelMessage high_level_message_out;

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
                if(strcmp(Nothing, masterRxBuffer)!=0) {
                    convertData(&high_level_message_out, masterRxBuffer);
                }
                LOG_TRACE("angle %d \r\n", high_level_message_out.angle); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                LOG_TRACE("x %f \r\n", high_level_message_out.x); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
                LOG_TRACE("y %f \r\n", high_level_message_out.y); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")

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
void convertData(highLevelMessage *message,  char *buffer) {
    LOG_TRACE("Beginning of Function \r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    static char delim[] = " ";
    static char Color[] = "Color", Distance[] = "Distance", Height[] = "Height", Angle[] = "Angle";
    char *ptr = strtok(buffer, delim);
    LOG_TRACE(" \r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
    char parsedData[8][30] = {{0}};
    static int i = 0;
    while(ptr != NULL)
    {
        strncpy(parsedData[i], ptr, strlen(ptr)+1);
        ptr = strtok(NULL, delim);
        i++;
        LOG_TRACE("while \r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")

    }

    /* This loop will show that there are zeroes in the str after tokenizing */
    for (i = 0; i < sizeof(parsedData) / sizeof(parsedData[0]); i++)
    {
        LOG_TRACE("for \r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
        if(strcmp(parsedData[i], Color)==0) {
            i++;
        }
        else if(strcmp(parsedData[i], Distance)==0) {
            i++;
            message->x = atof(parsedData[i]);
        }
        else if(strcmp(parsedData[i], Height)==0) {
            i++;
            message->y = atof(parsedData[i]);
        }
        else if(strcmp(parsedData[i], Angle)==0) {
            i++;
            message->angle = (int)(round(atof(parsedData[i])));
        }
    }

    LOG_TRACE("End of Function \r\n"); // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")


}
