#include "TCPTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"

#include <stdbool.h>

#include "api.h"
#include <string.h>
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "lwip.h"

#include "PackageManager.h"
#include "can.h"

#include "SWIR_Task.h"
#include "LWIR_Task.h"
#include "TV_Task.h"
#include "StepMotors_Task.h"

void RxParser(CMD_IDs id, uint8_t *x);
extern void Head_Motors_STOP(void);

//extern uint8_t angle[5];

StackType_t TCP_Tx_Stack[1024];
StaticTask_t TCP_Tx_Task_Buffer;

#define MSG_BUFFER 512

void TCP_RxThread(void* arg){
	struct netconn *nc = arg;
	struct netbuf *nb;

	char Rx_msg[MSG_BUFFER];
//	Rx_msg = netbuf_alloc(nb, MSG_BUFFER);
	uint16_t len = 0;
	err_t err;
	struct ip4_addr ClientAddr;
	uint16_t ClientPort;

	while(true)
	{
		err = netconn_recv(nc, &nb);

		if(err != ERR_OK)
		{
			//printf("MCU: recv error: %d; freeing heap\r\n",err);
			netconn_close(nc);
			Head_Motors_STOP();
			netbuf_delete(nb);
			netconn_delete(nc);
			vTaskDelete(NULL);
		}

		len = nb->p->len;

		if (len > MSG_BUFFER)
		{
			len = MSG_BUFFER;
		}

		memcpy(Rx_msg, nb->p->payload, len);
		char* search = Rx_msg;
		
		while((search != NULL) && (search != &Rx_msg[len])){
			RxParser((CMD_IDs)*(search + 1), (uint8_t*)search + 2);
			search++; //search += SizeOf(CMD_IDs);
			search = memchr(search, 'e',  &Rx_msg[len] - search);
		}
		memset(Rx_msg, '\0', len);
		netbuf_delete(nb);
	}
}

void TCP_TxThread(void* arg){
	struct netconn *nc = arg;
	HAL_TIM_Base_Start_IT(&htim13);
	while(true)
	{
		uint8_t data[5];
		if (Heartbeat == true){
			netconn_write(nc, 0, 0, NETCONN_NOCOPY);
			Heartbeat = false;
		}
		else if (New_Angle == Axis_None){
			osDelay(1);
		}
		else if (New_Angle == Axis_X){
			data[0] = Axis_X;
			memcpy(&data[1], &(Angle_X), sizeof(Angle_X));
			netconn_write(nc, data, 5, NETCONN_NOCOPY);
			//printf("Axis: X, Angle: %f\r\n", Angle_X);
			New_Angle = Axis_None;
		}
		else if (New_Angle == Axis_Y){
			data[0] = Axis_Y;
			memcpy(&data[1], &(Angle_Y), sizeof(Angle_Y));
			netconn_write(nc, data, 5, NETCONN_NOCOPY);
			//printf("Axis: Y, Angle: %f\r\n", Angle_Y);
			New_Angle = Axis_None;
		}
	}
}

void RxParser(CMD_IDs id, uint8_t *x){
//	uint8_t Zeroing_angles_CMD[8] = {1 << 5, 0, 0, 0, 0, 0, 0, 0};
	//    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &x, &TxMailbox);          //CAN BUS
	switch (id){
	case SWIR_Filter_ID:
		SWIR_Filter_Driver();
	break;
	case SWIR_Lens_ID:
		xQueueSend(SWIRLens_QueueHandle, x, portMAX_DELAY);
		break;
	case SWIR_Camera_ID:
		xQueueSend(SWIRCam_QueueHandle, x, portMAX_DELAY);
		break;
	case LWIR_Lens_ID:
		xQueueSend(LWIRLens_QueueHandle, x, portMAX_DELAY);
		break;
	case Motor_Package_ID:
		xQueueSend(StepMotors_QueueHandle, x, portMAX_DELAY);
		break;
	case TV_OR_LED_Package_ID:
		xQueueSend(TV_QueueHandle, x, portMAX_DELAY);
		break;
	default:
		break;
	}
}
