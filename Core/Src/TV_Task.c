/*
 * TV_Task.c
 *
 *  Created on: Jul 13, 2021
 *      Author: user0051
 */


#include "TV_Task.h"
#include "i2c.h"
#include <string.h>

void TV_Task(void * arg){
	(void)(arg);
	uint8_t qmsg[3];
	uint8_t package[3];
	while(true){
		if( xQueueReceive( TV_QueueHandle, package, portMAX_DELAY) == pdPASS){
			memcpy(qmsg, package, 3);
			while(hi2c1.State != HAL_I2C_STATE_READY) vTaskDelay(1);
			HAL_I2C_Master_Transmit_IT(&hi2c1, qmsg[0], &qmsg[1], 2);
			vTaskDelay(10);
		}
	}
}
