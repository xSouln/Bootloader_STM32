//=================================================================================================
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//=================================================================================================
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "xFlash.h"
//=================================================================================================
//=================================================================================================
int8_t xFlashErasePages(uint32_t start_address, uint32_t end_address, uint32_t timeout)
{
  xFlash.Status.Operation = FlashOperationErase;
  xFlash.OperationTimeout = timeout;
  xFlash.Status.OperationTime = 0;
  xFlash.Status.OperationResult = BOOT_ACCEPT;
  
  if(!xFlash.Status.FlashUnlocked) { xFlash.Status.OperationResult = BOOT_LOCKED; goto end; }
  
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
  {
    if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
  }
  
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  while(start_address < end_address && start_address < XFLASH_END_ADDRESS)
  {
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    WRITE_REG(FLASH->AR, start_address);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
    
    xFlash.Status.EraseAddress = start_address;
    start_address += XFLASH_PAGE_SIZE;
    
    while(!READ_BIT(FLASH->SR, FLASH_SR_EOP))
    {
      if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
    }
    WRITE_REG(FLASH->SR, FLASH_SR_EOP);
  }
  
  end:;
  CLEAR_BIT(FLASH->CR, FLASH_CR_PER);  
  xFlash.Status.OperationTime = timeout - xFlash.OperationTimeout;
  return xFlash.Status.OperationResult;
}
//=================================================================================================
int8_t xFlashWrite(uint32_t address, uint8_t* data, uint16_t len, uint32_t timeout)
{
  uint16_t i = 0;
  xFlash.Status.Operation = FlashOperationWrite;
  xFlash.OperationTimeout = timeout;
  xFlash.Status.OperationTime = 0;
  xFlash.Status.OperationResult = BOOT_ACCEPT;
  
  if(!xFlash.Status.FlashUnlocked) { xFlash.Status.OperationResult = BOOT_LOCKED; goto end; }
  
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
  {
    if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
  }
  
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  SET_BIT(FLASH->CR, FLASH_CR_PG);
  
  while(i < len && address < XFLASH_END_ADDRESS)
  {
    uint16_t value = data[i + 1];
    value <<= 8;
    value |= data[i];
    
    xFlash.Status.WriteAddress = address;
    *(volatile uint16_t*)(address) = value;
    
    address += sizeof(uint16_t);
    i += sizeof(uint16_t);
    
    while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
    {
      if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
    }
    
    while(!READ_BIT(FLASH->SR, FLASH_SR_EOP))
    {
      if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
    }
    
    WRITE_REG(FLASH->SR, FLASH_SR_EOP);
  }
  
  end:;
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);  
  xFlash.Status.OperationTime = timeout - xFlash.OperationTimeout;
  return xFlash.Status.OperationResult;
}
//=================================================================================================
int8_t xFlashRead(uint32_t address, volatile uint8_t* data, uint16_t len)
{
  volatile uint8_t* ptr = (volatile uint8_t*)address;
  uint16_t i = 0;
  while(i < len && address < 0x0801FFFF)
  {
    data[i] = ptr[i];
    address++;
    i++;
  }
  return BOOT_ACCEPT;
}
//=================================================================================================
uint16_t xFlashGetCrc(uint32_t start_address, uint32_t end_address)
{
  uint16_t crc = 0;
  while(start_address < end_address && start_address < 0x0801FFFF)
  {
    crc += *(volatile uint8_t*)start_address;
    start_address++;
  }
  return crc;
}
//=================================================================================================
int8_t xFlashSetLock(bool state)
{
  xFlash.Status.FlashUnlocked = !state;
  xFlash.Status.Operation = FlashOperationLock;
  xFlash.Status.OperationTime = 0;
  xFlash.Status.OperationResult = BOOT_ACCEPT;
  
  if(state)
  {
    SET_BIT(FLASH->CR, FLASH_CR_LOCK);
  }
  else
  {
    WRITE_REG(FLASH->KEYR, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR, FLASH_KEY2);
  }
  
  return xFlash.Status.OperationResult;
}
//=================================================================================================
xFlashT xFlash;
//===================================================================================================