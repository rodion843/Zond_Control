#ifndef STATUS_REG_H
#define STATUS_REG_H
enum status_bits{
	None = 0,
    SWIR_Filter = 1UL << 0,
    SWIR_BIS_PAL = 1UL << 1,
    Switch_Power = 1UL << 2,
    TV_Driver_Power = 1UL << 3,
    SWIR_Driver_Power = 1UL << 4,
    LWIR_Driver_Power = 1UL << 5, 
    SWIR_Camera_Power = 1UL << 6,
    LWIR_Camera_Power = 1UL << 7,
    TV_Camera_Power = 1UL << 8,
    ZOND_OPTO_Block = 1UL << 9, 
    AUDIO_Power = 1UL << 10, 
    Thorlabs_Power = 1UL << 11
};
typedef enum status_bits Status_Register;
#endif /* STATUS_REG_H */