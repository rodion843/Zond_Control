#include "PackageManager.h"

#include "ee24.h"
uint8_t RxDriver(uint8_t* x);

extern CAN_TxHeaderTypeDef TxHeader;
extern uint32_t TxMailbox;
extern Status_Register status_reg;

void SendToRS(CMD_IDs id, uint8_t * data, size_t size);

uint32_t Motor_Stop;
uint32_t Motor_gogo;
uint32_t CAN_Error_par_counter;
uint8_t RxDriver(uint8_t* x){
	uint8_t Zeroing_angles_CMD[8] = {1 << 5, 0, 0, 0, 0, 0, 0, 0};
	//    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &x, &TxMailbox);          //CAN BUS
	switch ((CMD_IDs)x[0]){
	case SWIR_Filter_ID:
		status_reg ^= 1UL << SWIR_Filter;
		SWIR_Filter_Driver();
		return SWIR_Filter_Package_Size;
	case SWIR_Lens_ID:
		if(huart4.gState == HAL_UART_STATE_READY){
			SendToRS(SWIR_Lens_ID, &x[1], 7);
		}
		return SWIR_Lens_Package_Size;
	case SWIR_Camera_ID:
		if(huart4.gState == HAL_UART_STATE_READY){
			SendToRS(SWIR_Camera_ID, &x[1], 16);
		}
		return SWIR_Camera_Package_Size;
	case LWIR_Lens_ID:
		if(huart4.gState == HAL_UART_STATE_READY){
			SendToRS(LWIR_Lens_ID, &x[1], 7);
		}
		return LWIR_Lens_Package_Size;
	case Motor_Package_ID:
		if(x[1] == 0x10){
			Motor_Stop++;
			Motor_gogo = 0;
		}
		else{
			Motor_gogo++;
		}
		if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &x[1], &TxMailbox) != HAL_OK){
			CAN_Error_par_counter++;
		}else{
			CAN_Error_par_counter = 0;
		}
		return Motors_Package_Size;
	case TV_OR_LED_Package_ID:
		HAL_I2C_Master_Transmit(&hi2c1, x[1], &x[2], 2, HAL_MAX_DELAY);
		//	printf("i2c: %d, %d\r\n", x[2], x[3]);
		return TV_Package_Size;
//	case 'a':
//		//		printf("Zeroing angles\r\n");
//		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, Zeroing_angles_CMD, &TxMailbox);
//		Zeroing_angles_CMD[0] = 1 << 6;
//		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, Zeroing_angles_CMD, &TxMailbox);
//		return 1;
	default:
		return 1;
	}
}
