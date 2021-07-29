/*
 * LWIR_Task.h
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */

#ifndef INC_LWIR_TASK_H_
#define INC_LWIR_TASK_H_
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#define LWIRLens_StackDepth 512u
#define LWIRLens_Queue_Length 10
#define LWIRLens_Queue_ItemSize 7
TaskHandle_t LWIRLens_TaskHandle;
StaticTask_t LWIRLens_Task_Buffer;
StackType_t LWIRLens_Stack[128 * 4];
QueueHandle_t LWIRLens_QueueHandle;
StaticQueue_t LWIRLens_Queue;
uint8_t LWIRLens_QueueStorageArea[ LWIRLens_Queue_Length * LWIRLens_Queue_ItemSize ];


void LWIRLens_Task(void * arg);

#endif /* INC_LWIR_TASK_H_ */
