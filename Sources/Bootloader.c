/*
 * Bootloader.c
 *
 *  Created on: Dec 17, 2020
 *      Author: rekuts
 */
//=================================================================================================================================
#include <string.h>
#include <stdlib.h>
#include "Bootloader.h"
#include "xRequest.h"
#include "xFlash.h"
//#include "stm32f1xx_hal_flash_ex.h"
extern void AppMain();
//=================================================================================================================================
int16_t ActionTryWrite(xObject context, RequestWriteT* request, uint16_t object_size)
{
  if(!request){ return BOOT_ERROR_REQUEST; }
  if(request->StartAddress < BOOT_END_ADDRESS){ return BOOT_OUTSIDE; }
  if((request->StartAddress + request->DataSize) > APP_END_ADDRESS){ return BOOT_OUTSIDE; }
  if(object_size - sizeof(RequestWriteT) < request->DataSize){ return BOOT_ERROR_REQUEST; }
  
  uint8_t* data = (uint8_t*)request;
  data += sizeof(RequestWriteT);
  
  return xFlashWrite(request->StartAddress, data, request->DataSize, 100);
}
//=================================================================================================================================
int16_t ActionTryRead(xObject context, RequestReadT* request, uint16_t object_size)
{  
  return BOOT_ACCEPT;
}
//=================================================================================================================================
int16_t ActionTryErase(xObject context, RequestEraseT* request, uint16_t object_size)
{
  if(!request){ return BOOT_ERROR_REQUEST; }
  if(request->StartAddress < BOOT_END_ADDRESS){ return BOOT_OUTSIDE; }
  if(request->StartAddress > request->EndAddress){ return BOOT_ERROR_ACTION; }
  //request->EndAddress - request->StartAddress) / XFLASH_PAGE_SIZE * 20
  return xFlashErasePages(request->StartAddress, request->EndAddress, 2000);
}
//=================================================================================================================================
int16_t ActionSetLockState(xObject context, uint8_t* request)
{
  if(!request){ return BOOT_ERROR_REQUEST; }
  
  return xFlashSetLock(*request > 0);
}
//=================================================================================================================================
int16_t ActionTryJumpToMain(xObject context)
{
  Bootloader.Status.JumpToMain = true;
  
  return BOOT_ACCEPT;
}
//=================================================================================================================================
int16_t ActionTryUpdateInfo(xObject context)
{
  uint16_t crc;
  xFlashRead(BOOT_END_ADDRESS, (volatile uint8_t*)&Bootloader.Info, sizeof(Bootloader.Info));
  
  crc = xFlashGetCrc(Bootloader.Info.StartAddress, Bootloader.Info.EndAddress);
  if(crc == Bootloader.Info.Crc)
  {
    Bootloader.AppInfo.StartAddress = Bootloader.Info.StartAddress;
    Bootloader.AppInfo.EndAddress = Bootloader.Info.EndAddress;
    Bootloader.AppInfo.Crc = Bootloader.Info.Crc;
    
    return BOOT_ACCEPT;
  }
  return BOOT_ERROR_DATA;
}
//=================================================================================================================================
int16_t ActionReadCrc(xObject context, RequestEraseT* request, uint16_t object_size)
{
  uint16_t crc = 0;
  
  return BOOT_ACCEPT;
}
//=================================================================================================================================
int16_t ActionTryReset(xObject context)
{
  uint16_t crc = 0;
  
  return BOOT_ACCEPT;
}
//=================================================================================================================================
BootloaderT Bootloader = {
  .AppInfo =
  {
    .StartAddress = APP_START_ADDRESS,
    .EndAddress = APP_END_ADDRESS,
    .PageSize = SYSTEM_FLASH_PAGE_SIZE
  },
};
//=================================================================================================================================
