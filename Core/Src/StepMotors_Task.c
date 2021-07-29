/*
 * StepMotors_Task.c
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */


#include "StepMotors_Task.h"
#include "can.h"


void StepMotors_Task(void * arg){
	(void)(arg);
	uint8_t package[5];
	while(true){
		if( xQueueReceive( StepMotors_QueueHandle, package, portMAX_DELAY) == pdPASS){
			if (*package == 0x10){
				HAL_CAN_AbortTxRequest(&hcan1, TxMailbox);
			}
			while(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, package, &TxMailbox) != HAL_OK) {
				vTaskDelay(1);
			}
		}
	}
}
