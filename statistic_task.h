#ifndef STATISTIC_TASK_H
#define STATISTIC_TASK_H

#include "FreeRTOS.h"
#include "definitions.h"
#include <pthread.h>
#include "debug.h"
#include "mqtt_if.h"
#include "mqueue.h"
#include "statistics_thread_queue.h"
#include "hash-string.h"
#include "mqtt_publish_queue_def.h"
#include <stdio.h>
#include "debug_if.h"
#define PRIORITY 1
#define STATSTHREADSTACKSIZE 1024

void init_stats_thread( mqd_t handle );
void* stats_task();

#endif
