/*
 * SWIR_Task.h
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */

#ifndef INC_SWIR_TASK_H_
#define INC_SWIR_TASK_H_

#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#define SWIRLens_StackDepth 512u
#define SWIRLens_Queue_Length 10
#define SWIRLens_Queue_ItemSize 7
TaskHandle_t SWIRLens_TaskHandle;
StaticTask_t SWIRLens_Task_Buffer;
StackType_t SWIRLens_Stack[SWIRLens_StackDepth];
QueueHandle_t SWIRLens_QueueHandle;
StaticQueue_t SWIRLens_Queue;
uint8_t SWIRLens_QueueStorageArea[ SWIRLens_Queue_Length * SWIRLens_Queue_ItemSize ];

#define SWIRCam_StackDepth 512u
#define SWIRCam_Queue_Length 10
#define SWIRCam_Queue_ItemSize 16
TaskHandle_t SWIRCam_TaskHandle;
StaticTask_t SWIRCam_Task_Buffer;
StackType_t SWIRCam_Stack[128 * 4];
QueueHandle_t SWIRCam_QueueHandle;
StaticQueue_t SWIRCam_Queue;
uint8_t SWIRCam_QueueStorageArea[ SWIRCam_Queue_Length * SWIRCam_Queue_ItemSize ];


void SWIRLens_Task(void * arg);
void SWIRCam_Task(void * arg);
#endif /* INC_SWIR_TASK_H_ */
