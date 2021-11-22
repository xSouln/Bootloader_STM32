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
int8_t xFlashErase(uint32_t page_address)
{
  WRITE_REG(FLASH->KEYR, FLASH_KEY1);
  WRITE_REG(FLASH->KEYR, FLASH_KEY2);
  
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY)){ };
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  SET_BIT(FLASH->CR, FLASH_CR_PER);
  WRITE_REG(FLASH->AR, page_address);
  SET_BIT(FLASH->CR, FLASH_CR_STRT);
  
  while(!READ_BIT(FLASH->SR, FLASH_SR_EOP)){};
  WRITE_REG(FLASH->SR, FLASH_SR_EOP);
  
  CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
  
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);
  return ACCEPT;
}
//=================================================================================================
int8_t xFlashErasePages(uint32_t page_address, uint16_t page_size, uint16_t page_count)
{
  WRITE_REG(FLASH->KEYR, FLASH_KEY1);
  WRITE_REG(FLASH->KEYR, FLASH_KEY2);
  
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY)){ };
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  while(page_count)
  {
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    WRITE_REG(FLASH->AR, page_address);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
  
    page_address += page_size;
    page_count--;
    
    while(!READ_BIT(FLASH->SR, FLASH_SR_EOP)){};
    WRITE_REG(FLASH->SR, FLASH_SR_EOP);
  }
  
  CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
  
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);
  return ACCEPT;
}
//=================================================================================================
int8_t xFlashWriteHalfWord(uint32_t address, uint16_t data)
{
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY)){ };
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  SET_BIT(FLASH->CR, FLASH_CR_PG);
  *(volatile uint16_t*)address = data;
  
  while(!READ_BIT(FLASH->SR, FLASH_SR_EOP)){};
  WRITE_REG(FLASH->SR, FLASH_SR_EOP);
  
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
  return ACCEPT;
}
//=================================================================================================
int8_t xFlashWrite(uint32_t address, uint8_t* data, uint16_t len)
{
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY)){ };
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  SET_BIT(FLASH->CR, FLASH_CR_PG);
  
  for(uint16_t i = 0; i < len; i += sizeof(uint16_t))
  {
    uint16_t value = data[i + 1];
    value <<= 8;
    value |= data[i];
    
    *(volatile uint16_t*)(address + i) = value;
    
    while(!READ_BIT(FLASH->SR, FLASH_SR_EOP)){ };
    WRITE_REG(FLASH->SR, FLASH_SR_EOP);
  }
  
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
  return ACCEPT;
}
//=================================================================================================
/*
void PrivateSaveObject(xFlashSaveObjectParamsT *Params){
  uint8_t *ptr = (uint8_t*)Params->Object;
  uint16_t Crc = 0;
  
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)){};
  if(!(FLASH->SR & FLASH_SR_EOP)) FLASH->SR = FLASH_SR_EOP;
  
  FLASH->CR |= FLASH_CR_PG;
  
  for(int i = 0; i < Params->ObjectSize; i++){
    Crc += ptr[i];
    xFlashProgramHalfWord(Params->Addres, ptr[i]);
    Params->Addres += 2;
  }  
  
  xFlashProgramHalfWord(Params->Addres, Crc);
  
  FLASH->CR &= ~FLASH_CR_PG;
}
//=================================================================================================
bool xFlashSaveObject(xFlashSaveObjectParamsT *Params){
  if(HAL_FLASH_Unlock() != HAL_OK) return false;  
  PrivateErase(Params->Addres);
  PrivateSaveObject(Params);
  HAL_FLASH_Lock();
  return true;
}
//=================================================================================================
bool xFlashReadObject(xFlashSaveObjectParamsT *Params){
  HAL_FLASH_Unlock();
  uint16_t Crc = 0;
  uint16_t CrcCalculate = 0;
  uint8_t *ptr = (uint8_t*)Params->Object;
  uint8_t *AddresPtr = (uint8_t*)Params->Addres;
  
  for(int i = 0; i < Params->ObjectSize; i++){
    ptr[i] = (*AddresPtr)&0xff;
    CrcCalculate += ptr[i];
    AddresPtr += 2;
  }
  
  memcpy(&Crc, AddresPtr, sizeof(Crc));
  HAL_FLASH_Lock();
  return Crc == CrcCalculate;
}
//===================================================================================================
*/