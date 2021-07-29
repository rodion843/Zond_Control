/*
 * StepperMotors_Task.h
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */

#ifndef INC_STEPMOTORS_TASK_H_
#define INC_STEPMOTORS_TASK_H_
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#define StepMotors_StackDepth 512u
#define StepMotors_Queue_Length 10
#define StepMotors_Queue_ItemSize 5
TaskHandle_t StepMotors_TaskHandle;
StaticTask_t StepMotors_Task_Buffer;
StackType_t StepMotors_Stack[128 * 4];
QueueHandle_t StepMotors_QueueHandle;
StaticQueue_t StepMotors_Queue;
uint8_t StepMotors_QueueStorageArea[ StepMotors_Queue_Length * StepMotors_Queue_ItemSize ];


void StepMotors_Task(void * arg);

#endif /* INC_STEPMOTORS_TASK_H_ */
