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
#define XFLASH_ADDRES_FOR_SAVE_PARAMS (FLASH_BASE | OB_WRP_PAGES32TO33)
//=================================================================================================
int8_t xFlashProgramHalfWord(uint32_t Address, uint16_t Data){
  *(volatile uint16_t*)Address = Data;
  while(!(FLASH->SR & FLASH_SR_EOP)){};
  FLASH->SR = FLASH_SR_EOP;
  return ACCEPT;
}
//=================================================================================================
int8_t xFlashErase(uint32_t PageAddress){  
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)){};
  if(!(FLASH->SR & FLASH_SR_EOP)) FLASH->SR = FLASH_SR_EOP;
  SET_BIT(FLASH->CR, FLASH_CR_PER);
  WRITE_REG(FLASH->AR, PageAddress);
  SET_BIT(FLASH->CR, FLASH_CR_STRT);
  
  while(!(FLASH->SR & FLASH_SR_EOP)){};
  FLASH->SR = FLASH_SR_EOP;  
  CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
  
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)){};
  return ACCEPT;
}
//=================================================================================================
/*
void PrivateErase(uint32_t Addres){
  xFlashErase(Addres);
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)){};
}
//=================================================================================================
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
//=================================================================================================
bool xFlashSaveParams(xSaveParamsT *SaveParams){
  xFlashSaveObjectParamsT Params = { 0 };
  Params.Addres = XFLASH_ADDRES_FOR_SAVE_PARAMS;
  Params.Object = SaveParams;
  Params.ObjectSize = sizeof(xSaveParamsT);
  return xFlashSaveObject(&Params);
}
//=================================================================================================
bool xFlashReadParams(xSaveParamsT *SaveParams){
  xFlashSaveObjectParamsT Params = { 0 };
  Params.Addres = XFLASH_ADDRES_FOR_SAVE_PARAMS;
  Params.Object = SaveParams;
  Params.ObjectSize = sizeof(xSaveParamsT);
  return xFlashReadObject(&Params);
}
//===================================================================================================
xFlashIpT xFlash = {
  .SaveObject = xFlashSaveObject,
  .ReadObject = xFlashReadObject,
  .SaveParams = xFlashSaveParams,
  .ReadParams = xFlashReadParams,
  
  .Addresses = {
    .FreeList1= FLASH_BASE | OB_WRP_PAGES32TO33
  }
};
//===================================================================================================
*/