/*
 * SWIR_Task.c
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */


#include "SWIR_Task.h"
#include "usart.h"


void SWIRLens_Task(void * arg){
	(void)(arg);
	uint8_t package[7];
	while(true){
		if( xQueueReceive( SWIRLens_QueueHandle, package, portMAX_DELAY) == pdPASS){
			while(huart4.gState != HAL_UART_STATE_READY) vTaskDelay(1);
			SendToRS(SWIR_Lens_ID, package, 7);
		}
	}
}

void SWIRCam_Task(void * arg){
	(void)(arg);
	uint8_t package[7];
	while(true){
		if( xQueueReceive( SWIRCam_QueueHandle, package, portMAX_DELAY) == pdPASS){
			while(huart4.gState != HAL_UART_STATE_READY) vTaskDelay(1);
			SendToRS(SWIR_Camera_ID, package, 16);
		}
	}
}
