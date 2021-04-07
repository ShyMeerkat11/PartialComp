#ifndef CHAIN_TASK_H
#define CHAIN_TASK_H

#include "FreeRTOS.h"
#include "definitions.h"
#include <pthread.h>
#include "debug.h"
#include "mqtt_if.h"
#include "mqueue.h"
#include "chain_thread_queue.h"
#include "hash-string.h"
#include "mqtt_publish_queue_def.h"
#include <stdio.h>
#include "debug_if.h"
#include "uart_term.h"
#include "statistics_thread_queue.h"
#define PRIORITY 1
#define CHAINTHREADSTACKSIZE 1024
#define INCREMENT 1
#define ADD_SIX 6
#define ADD_THREE 3
void init_chain_thread( mqd_t handle );
void* chain_task();

#endif
