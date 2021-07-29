#ifndef FREERTOSHEADER_H
#define FREERTOSHEADER_H
#include "status_reg.h"
#include <stdio.h>
#include "swirlwir.h"

#include "api.h"
#include <string.h>
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "lwip.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern CAN_FilterTypeDef CAN_FILTER;    
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim7;
extern struct netif gnetif;
//extern Status_Register status_reg;


void CAN_Init(void);
//extern void vTaskClearUsage(void);
void Head_Motors_STOP(void);
void fstatus_reg(void);
#endif /*FREERTOSHEADER_H*/
