/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <StepMotors_Task.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TCPTask.h"
#include "freertosheader.h"
#include "can.h"

//#include "SWIR_Task.h"
//#include "LWIR_Task.h"
//#include "TV_Task.h"
#include "StepMotors_Task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint8_t TxData[7] = {0xFF, 0x01, 0x00, 0x5D, 0x00, 0x00, 0x00};



StaticTask_t TCP_Rx_Task_Buffer;
StackType_t TCP_Rx_Stack[2048];


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */

  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* Create a queue capable of containing 10 uint64_t values. */
	StepMotors_QueueHandle = xQueueCreateStatic(StepMotors_Queue_Length,
												StepMotors_Queue_ItemSize,
												StepMotors_QueueStorageArea,
												&StepMotors_Queue );
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  StepMotors_TaskHandle = xTaskCreateStatic(StepMotors_Task, "Stepper Motors Task", StepMotors_StackDepth, 
                                            (void*)NULL, osPriorityAboveNormal, StepMotors_Stack, &StepMotors_Task_Buffer );
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  (void)argument;
  struct netconn* nc = netconn_new(NETCONN_TCP);

  netconn_bind(nc, IP_ADDR_ANY, 1300);

  netconn_listen(nc);

  struct netconn* in_nc = netconn_new(NETCONN_TCP);
  /* Infinite loop */
  for(;;)
  {
      netconn_accept(nc, &in_nc);
      xTaskCreateStatic(TCP_RxThread, "TCP Rx Task", 2048, (void*)in_nc, osPriorityNormal, TCP_Rx_Stack, &TCP_Rx_Task_Buffer );
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Head_Motors_STOP(void){	//TODO write data to i2c eeporm
	uint8_t x[] = {0x10, 0, 0, 0, 0, 0, 0, 0};
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, x, &TxMailbox);
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
