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

#include "SWIR_Task.h"
#include "LWIR_Task.h"
#include "TV_Task.h"
#include "StepMotors_Task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
	  Status_Register status_reg = None;
uint32_t FreeHeap = 0;


uint8_t TxData[7] = {0xFF, 0x01, 0x00, 0x5D, 0x00, 0x00, 0x00};



StaticTask_t TCP_Rx_Task_Buffer;
StackType_t TCP_Rx_Stack[2048];

TaskHandle_t Debug_TaskHandle;
StaticTask_t Debug_Task_Buffer;
StackType_t Debug_Stack[128 * 4];


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
void vApplicationIdleHook( void )
{
	if (FreeHeap != xPortGetFreeHeapSize()){
        FreeHeap = xPortGetFreeHeapSize();
//		//printf("Free heap size is: %d\r\n",FreeHeap);
	}
}
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
	SWIRLens_QueueHandle = xQueueCreateStatic(SWIRLens_Queue_Length,
											 SWIRLens_Queue_ItemSize,
											 SWIRLens_QueueStorageArea,
											 &SWIRLens_Queue );
	SWIRCam_QueueHandle = xQueueCreateStatic(SWIRCam_Queue_Length,
											 SWIRCam_Queue_ItemSize,
											 SWIRCam_QueueStorageArea,
											 &SWIRCam_Queue );
	LWIRLens_QueueHandle = xQueueCreateStatic(LWIRLens_Queue_Length,
											  LWIRLens_Queue_ItemSize,
											  LWIRLens_QueueStorageArea,
											  &LWIRLens_Queue );
	TV_QueueHandle = xQueueCreateStatic(TV_Queue_Length,
										TV_Queue_ItemSize,
										TV_QueueStorageArea,
										&TV_Queue );
	StepMotors_QueueHandle = xQueueCreateStatic(StepMotors_Queue_Length,
												StepMotors_Queue_ItemSize,
												StepMotors_QueueStorageArea,
												&StepMotors_Queue );
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
//  Debug_TaskHandle = xTaskCreateStatic(DebugTask, "Debug Task", 4 * 128, (void*)NULL, osPriorityNormal, Debug_Stack, &Debug_Task_Buffer );
  SWIRLens_TaskHandle = xTaskCreateStatic(SWIRLens_Task, "SWIR Lens Task", SWIRLens_StackDepth, (void*)NULL, osPriorityAboveNormal, SWIRLens_Stack, &SWIRLens_Task_Buffer );
  SWIRCam_TaskHandle = xTaskCreateStatic(SWIRCam_Task, "SWIR Cam Task", SWIRCam_StackDepth, (void*)NULL, osPriorityAboveNormal, SWIRCam_Stack, &SWIRCam_Task_Buffer );
  LWIRLens_TaskHandle = xTaskCreateStatic(LWIRLens_Task, "LWIR Lens Task", LWIRLens_StackDepth, (void*)NULL, osPriorityAboveNormal, LWIRLens_Stack, &LWIRLens_Task_Buffer );
  TV_TaskHandle = xTaskCreateStatic(TV_Task, "TV Lens Task", TV_StackDepth, (void*)NULL, osPriorityAboveNormal, TV_Stack, &TV_Task_Buffer );
  StepMotors_TaskHandle = xTaskCreateStatic(StepMotors_Task, "Stepper Motors Task", StepMotors_StackDepth, (void*)NULL, osPriorityAboveNormal, StepMotors_Stack, &StepMotors_Task_Buffer );
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
//  xTimerStart(DebugTimer, 0);
  FreeHeap = xPortGetFreeHeapSize();
  //printf("Free Heap = %ld\r\n", FreeHeap);
  status_reg |= TV_Driver_Power;
//  HAL_GPIO_WritePin(TV_DRIVER_GPIO_Port, TV_DRIVER_Pin, GPIO_PIN_SET);
  status_reg |= TV_Camera_Power;
  HAL_GPIO_WritePin(TV_CAM_GPIO_Port, TV_CAM_Pin, GPIO_PIN_SET);
  status_reg |= LWIR_Driver_Power;
  HAL_GPIO_WritePin(LWIR_CTRL_GPIO_Port, LWIR_CTRL_Pin, GPIO_PIN_SET);
  status_reg |= LWIR_Camera_Power;
  HAL_GPIO_WritePin(LWIR_POWER_GPIO_Port, LWIR_POWER_Pin, GPIO_PIN_SET);
  status_reg |= SWIR_Driver_Power;
  HAL_GPIO_WritePin(SWIR_CTRL_GPIO_Port, SWIR_CTRL_Pin, GPIO_PIN_SET);
  status_reg |= SWIR_Camera_Power;
  HAL_GPIO_WritePin(SWIR_POWER_GPIO_Port, SWIR_POWER_Pin, GPIO_PIN_SET);
  status_reg |= SWIR_Camera_Power;
  HAL_GPIO_WritePin(SWIR_BIS_PAL_GPIO_Port, SWIR_BIS_PAL_Pin, GPIO_PIN_SET);
  status_reg |= SWIR_BIS_PAL;
  fstatus_reg();


  err_t err;

  while(gnetif.ip_addr.addr == 0) osDelay(1);
  ////printf("MCU: DHCP sucks! IP: %s\r\n", ip4addr_ntoa(&gnetif.ip_addr));
  struct netconn* nc = netconn_new(NETCONN_TCP);
  if (nc == NULL)
  {
	  ////printf("MCU: NC POOP\r\n");
	  netconn_delete(nc);
      vTaskSuspend(NULL);
  }
  else{
      //printf("MCU: NC NORM\r\n");
  }
  err = netconn_bind(nc, IP_ADDR_ANY, 1300);
  if (err != 0)
  {
	  //printf("MCU: Bind POOP - %d\r\n", err);
	  netconn_delete(nc);
      vTaskSuspend(NULL);
  }
  else{
      //printf("MCU: Bind NORM\r\n");
  }
  err = netconn_listen(nc);
  if (err != 0)
  {
	  //printf("MCU: listen POOP - %d\r\n", err);
	  netconn_delete(nc);
      vTaskSuspend(NULL);
  }
  else{
      //printf("MCU: listen NORM\r\n");
  }
  struct netconn* in_nc = netconn_new(NETCONN_TCP);
  /* Infinite loop */
  for(;;)
  {

      err = netconn_accept(nc, &in_nc);
      if(err != 0){
	 	 //printf("MCU: accept POOP - %d\r\n", err);
      }
      else{
		  //printf("MCU: accept NORM\r\n");
	        xTaskCreateStatic(TCP_RxThread, "TCP Rx Task", 2048, (void*)in_nc, osPriorityNormal, TCP_Rx_Stack, &TCP_Rx_Task_Buffer );
      }
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Head_Motors_STOP(void){	//TODO write data to i2c eeporm
	uint8_t x[] = {0x10, 0, 0, 0, 0, 0, 0, 0};
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, x, &TxMailbox);
}

void fstatus_reg(void){
//    //printf("\r\nSTATUS:\r\n");
//    if (status_reg & SWIR_Filter)
//        //printf("SWIR Filter     ON\r\n");
//    else
//        //printf("SWIR Filter     OFF\r\n");
//
//    if (status_reg & SWIR_BIS_PAL)
//        //printf("SWIR BIS PAL    ON\r\n");
//    else
//        //printf("SWIR BIS PAL    OFF\r\n");
//
//    if (status_reg & Switch_Power)
//        //printf("Switch Power    ON\r\n");
//    else
//        //printf("Switch Power    OFF\r\n");
//
//    if (status_reg & TV_Driver_Power)
//        //printf("TV Driver       ON\r\n");
//    else
//        //printf("TV Driver       OFF\r\n");
//
//    if (status_reg & SWIR_Driver_Power)
//        //printf("SWIR Driver     ON\r\n");
//    else
//        //printf("SWIR Driver     OFF\r\n");
//
//    if (status_reg & LWIR_Driver_Power)
//        //printf("LWIR Driver     ON\r\n");
//    else
//        //printf("LWIR Driver     OFF\r\n");
//
//    if (status_reg & SWIR_Camera_Power)
//        //printf("SWIR Camera     ON\r\n");
//    else
//        //printf("SWIR Camera     OFF\r\n");
//
//    if (status_reg & LWIR_Camera_Power)
//        //printf("LWIR Camera     ON\r\n");
//    else
//        //printf("LWIR Camera     OFF\r\n");
//
//    if (status_reg & TV_Camera_Power)
//        //printf("TV Camera Power ON\r\n");
//    else
//        //printf("TV Camera Power OFF\r\n");
//
//    if (status_reg & ZOND_OPTO_Block)
//        //printf("ZOND OPTO Block ON\r\n");
//    else
//        //printf("ZOND OPTO Block OFF\r\n");
//
//    if (status_reg & AUDIO_Power)
//        //printf("Audio Power     ON\r\n");
//    else
//        //printf("Audio Power     OFF\r\n");
//
//    if (status_reg & Thorlabs_Power)
//        //printf("Thorlabs Power  ON\r\n");
//    else
//        //printf("Thorlabs Power  OFF\r\n");
//    //printf("\r\n");
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
