/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_uart4_tx;

/* UART4 init function */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = UART_TO_RS_TX_Pin|UART_TO_RS_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* UART4 DMA Init */
    /* UART4_TX Init */
    hdma_uart4_tx.Instance = DMA1_Stream4;
    hdma_uart4_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_tx.Init.MemInc = DMA_MINC_DISABLE;
    hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_tx.Init.Mode = DMA_NORMAL;
    hdma_uart4_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_uart4_tx);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOC, UART_TO_RS_TX_Pin|UART_TO_RS_RX_Pin);

    /* UART4 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void CheckAndSetBR(const CMD_IDs x){
	switch (x){
	case SWIR_Lens_ID:
		HAL_GPIO_WritePin(RS_485_DIR_GPIO_Port, RS_485_DIR_Pin, GPIO_PIN_SET);
		if (huart4.Init.BaudRate != 9600){
			huart4.Init.BaudRate = 9600;
			HAL_UART_Init(&huart4);
		}
		break;
	case LWIR_Lens_ID:
		HAL_GPIO_WritePin(LLC_RS485_DIR_GPIO_Port, LLC_RS485_DIR_Pin, GPIO_PIN_SET);
		if (huart4.Init.BaudRate != 9600){
			huart4.Init.BaudRate = 9600;
			HAL_UART_Init(&huart4);
		}
		break;
	case SWIR_Camera_ID:
		HAL_GPIO_WritePin(SCC_RS422_DIR_GPIO_Port, SCC_RS422_DIR_Pin, GPIO_PIN_SET);
		if (huart4.Init.BaudRate != 115200){
			huart4.Init.BaudRate = 115200;
			HAL_UART_Init(&huart4);
		}
		break;
	default:
		break;
	}
}

void SendToRS(CMD_IDs id, uint8_t * data, size_t size){
	CheckAndSetBR(id);
	HAL_UART_Transmit_IT(&huart4, data, size);
//	HAL_GPIO_WritePin(RS_485_DIR_GPIO_Port, RS_485_DIR_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(LLC_RS485_DIR_GPIO_Port, LLC_RS485_DIR_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(SCC_RS422_DIR_GPIO_Port, SCC_RS422_DIR_Pin, GPIO_PIN_RESET);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	HAL_GPIO_WritePin(RS_485_DIR_GPIO_Port, RS_485_DIR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LLC_RS485_DIR_GPIO_Port, LLC_RS485_DIR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SCC_RS422_DIR_GPIO_Port, SCC_RS422_DIR_Pin, GPIO_PIN_RESET);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
