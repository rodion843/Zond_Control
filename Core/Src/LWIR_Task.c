/*
 * LWIR_Task.c
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */


#include "LWIR_Task.h"
#include "usart.h"


void LWIRLens_Task(void * arg){
	(void)(arg);
	uint8_t package[7];
	while(true){
		if( xQueueReceive( LWIRLens_QueueHandle, package, portMAX_DELAY) == pdPASS){
			while(huart4.gState != HAL_UART_STATE_READY) vTaskDelay(1);
			SendToRS(LWIR_Lens_ID, package, 7);
		}
	}
}
