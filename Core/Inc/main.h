/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "status_reg.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern Status_Register status_reg;



typedef enum {
	Axis_X,
	Axis_Y,
	Axis_None = -1,
}Axis;

typedef float Angle;

Angle Angle_X;
Angle Angle_Y;

typedef struct _Step_Driver{
	Angle Angle;
	Axis Axis;
}Step_Driver;

Axis New_Angle;
bool Heartbeat;

typedef enum{
	SWIR_Filter_ID = 1,
	SWIR_Lens_ID,
	SWIR_Camera_ID,
	LWIR_Lens_ID,
	TV_OR_LED_Package_ID,
	Motor_Package_ID
}CMD_IDs;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SWIR_FILTER_IN_Pin GPIO_PIN_3
#define SWIR_FILTER_IN_GPIO_Port GPIOE
#define SWIR_FILTER_OUT_Pin GPIO_PIN_4
#define SWIR_FILTER_OUT_GPIO_Port GPIOE
#define ETH_RST_Pin GPIO_PIN_0
#define ETH_RST_GPIO_Port GPIOB
#define THORLABS_CTRL_Pin GPIO_PIN_14
#define THORLABS_CTRL_GPIO_Port GPIOD
#define Audio_CTRL_Pin GPIO_PIN_15
#define Audio_CTRL_GPIO_Port GPIOD
#define SWIR_POWER_Pin GPIO_PIN_6
#define SWIR_POWER_GPIO_Port GPIOC
#define SWIR_CTRL_Pin GPIO_PIN_7
#define SWIR_CTRL_GPIO_Port GPIOC
#define Switch_Pin GPIO_PIN_8
#define Switch_GPIO_Port GPIOC
#define SWIR_BIS_PAL_Pin GPIO_PIN_9
#define SWIR_BIS_PAL_GPIO_Port GPIOC
#define UART_TO_RS_TX_Pin GPIO_PIN_10
#define UART_TO_RS_TX_GPIO_Port GPIOC
#define UART_TO_RS_RX_Pin GPIO_PIN_11
#define UART_TO_RS_RX_GPIO_Port GPIOC
#define SCC_RS422_DIR_Pin GPIO_PIN_12
#define SCC_RS422_DIR_GPIO_Port GPIOC
#define LIMIT_SWITCH_2_Pin GPIO_PIN_0
#define LIMIT_SWITCH_2_GPIO_Port GPIOD
#define LIMIT_SWITCH_1_Pin GPIO_PIN_1
#define LIMIT_SWITCH_1_GPIO_Port GPIOD
#define RS_485_DIR_Pin GPIO_PIN_2
#define RS_485_DIR_GPIO_Port GPIOD
#define LLC_RS485_DIR_Pin GPIO_PIN_3
#define LLC_RS485_DIR_GPIO_Port GPIOD
#define LWIR_POWER_Pin GPIO_PIN_6
#define LWIR_POWER_GPIO_Port GPIOD
#define LWIR_CTRL_Pin GPIO_PIN_7
#define LWIR_CTRL_GPIO_Port GPIOD
#define Zond_opto_v1_Pin GPIO_PIN_4
#define Zond_opto_v1_GPIO_Port GPIOB
#define TV_CAM_Pin GPIO_PIN_5
#define TV_CAM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define UART_TimeOut 100

#define SWIR_Filter_Package_Size 1
#define SWIR_Camera_Package_Size 16
#define SWIR_Lens_Package_Size 7
#define LWIR_Lens_Package_Size 7
#define TV_Package_Size 3
#define Motors_Package_Size 3

#define Ethernet_Package 'e'
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
