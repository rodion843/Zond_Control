#include "debug.h"
#include <stdint.h>
#include "ee24.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "can.h"
#include <string.h>
void DebugTask(void* arg){
	(void)arg;
	while(true){
		if (debug == Debug_one){
			float result_x, result_y;
			ee24_read(0, (uint8_t *)&result_x, 4, 100);
			ee24_read(4, (uint8_t *)&result_y, 4, 100);
//			printf("x: %f, y: %f", result_x, result_y);
			debug = Debug_None;
		}
		if (debug == Debug_two){
			float angle = 84.3;
			uint8_t x[8];
			x[0] = 1 << 5;
			memcpy(&x[1], &angle, sizeof(float));
			HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &x[0], &TxMailbox);
			debug = Debug_None;
		}
		else {
			osDelay(1);
		}
	}

}
