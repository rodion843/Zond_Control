#include "swirlwir.h"

uint8_t SWIR_Filter_Flag = 0;

void SWIR_Filter_init(void){
    if ((HAL_GPIO_ReadPin(LIMIT_SWITCH_1_GPIO_Port, LIMIT_SWITCH_1_Pin) == GPIO_PIN_SET) & (HAL_GPIO_ReadPin(LIMIT_SWITCH_2_GPIO_Port, LIMIT_SWITCH_2_Pin) == GPIO_PIN_SET)){
        return;
    }
    HAL_GPIO_WritePin(SWIR_FILTER_IN_GPIO_Port, SWIR_FILTER_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SWIR_FILTER_OUT_GPIO_Port, SWIR_FILTER_OUT_Pin, GPIO_PIN_RESET);
    SWIR_Filter_Flag = 0;
}

void SWIR_Filter_Driver(void){
    if (SWIR_Filter_Flag == 0){
        while(HAL_GPIO_ReadPin(LIMIT_SWITCH_1_GPIO_Port, LIMIT_SWITCH_1_Pin) == GPIO_PIN_SET){
            HAL_GPIO_WritePin(SWIR_FILTER_OUT_GPIO_Port, SWIR_FILTER_OUT_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SWIR_FILTER_IN_GPIO_Port, SWIR_FILTER_IN_Pin, GPIO_PIN_RESET);
        }
        SWIR_Filter_Flag = 1;
    }
    else {
        while(HAL_GPIO_ReadPin(LIMIT_SWITCH_2_GPIO_Port, LIMIT_SWITCH_2_Pin) == GPIO_PIN_SET){
            HAL_GPIO_WritePin(SWIR_FILTER_OUT_GPIO_Port, SWIR_FILTER_OUT_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SWIR_FILTER_IN_GPIO_Port, SWIR_FILTER_IN_Pin, GPIO_PIN_SET);
        }
        SWIR_Filter_Flag = 0;
    }
    HAL_GPIO_WritePin(SWIR_FILTER_OUT_GPIO_Port, SWIR_FILTER_OUT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SWIR_FILTER_IN_GPIO_Port, SWIR_FILTER_IN_Pin, GPIO_PIN_RESET);
}
