
#include "i2c.h"
#include "ee24.h"
#include "cmsis_os.h"
#define ee24_delay(x)   osDelay(x)


#define _EEPROM_PSIZE     8


uint8_t ee24_lock = 0;
//################################################################################################################
bool ee24_isConnected(void)
{
  if (HAL_I2C_IsDeviceReady(&_EEPROM_I2C, _EEPROM_ADDRESS, 2, 100)==HAL_OK)
    return true;
  else
    return false;	
}
//################################################################################################################
bool ee24_write(uint16_t address, uint8_t *data, size_t len, uint32_t timeout)
{
  if (ee24_lock == 1)
    return false;
  ee24_lock = 1; 
  uint16_t w;
  uint32_t startTime = HAL_GetTick();
  while (1)
  {
    w = _EEPROM_PSIZE - (address  % _EEPROM_PSIZE);
    if (w > len)
      w = len;        
    if (HAL_I2C_Mem_Write_DMA(&_EEPROM_I2C, _EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, data, w) == HAL_OK)
    {
      ee24_delay(10);
      len -= w;
      data += w;
      address += w;
      if (len == 0)
      {
        ee24_lock = 0;
        return true;
      }
      if (HAL_GetTick() - startTime >= timeout) 
      {
        ee24_lock = 0;
        return false;
      }
    }
    else
    {
      ee24_lock = 0;
      return false;		
    }
  }
}
//################################################################################################################
bool ee24_read(uint16_t address, uint8_t *data, size_t len, uint32_t timeout)
{
  if (ee24_lock == 1)
    return false;
  ee24_lock = 1;
  if (HAL_I2C_Mem_Read(&_EEPROM_I2C, _EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK)
  {
    ee24_lock = 0;
    return true;
  }
  else
  {
    ee24_lock = 0;
    return false;	
  }    
}
//################################################################################################################
bool ee24_eraseChip(void)
{
  const uint8_t eraseData[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF\
    , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  uint32_t bytes = 0;
  while ( bytes < (_EEPROM_SIZE_KBIT * 256))
  {
    if (ee24_write(bytes, (uint8_t*)eraseData, sizeof(eraseData), 100) == false)
      return false;
    bytes += sizeof(eraseData);           
  }
  return true;  
}
//################################################################################################################
