/*
 * TV_Task.h
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */

#ifndef INC_TV_TASK_H_
#define INC_TV_TASK_H_
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#define TV_StackDepth 512u
#define TV_Queue_Length 10
#define TV_Queue_ItemSize 3
TaskHandle_t TV_TaskHandle;
StaticTask_t TV_Task_Buffer;
StackType_t TV_Stack[128 * 4];
QueueHandle_t TV_QueueHandle;
StaticQueue_t TV_Queue;
uint8_t TV_QueueStorageArea[ TV_Queue_Length * TV_Queue_ItemSize ];


void TV_Task(void * arg);

#endif /* INC_TV_TASK_H_ */
