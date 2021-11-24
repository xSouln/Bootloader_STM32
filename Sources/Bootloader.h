/*
 * Bootloader.h
 *
 *  Created on: Dec 17, 2020
 *      Author: rekuts
 */

#ifndef BOOTLOADER_STM32_H_
#define BOOTLOADER_STM32_H_
//=================================================================================================================================
#include "xType.h"
#include "xPort.h"//stm32f1xx_hal_flash_ex
#include "BootTypes.h"
//=================================================================================================================================
//=================================================================================================================================
#define BOOT_START_ADDRESS      0x08000000
#define BOOT_END_ADDRESS        0x08007C00

#define APP_START_ADDRESS       0x08008000
#define APP_END_ADDRESS         0x0801FFFF

#define SYSTEM_FLASH_PAGE_SIZE  0x400

static char FIRMWARE_VERSION[] = "boot:1.0.0";
//=================================================================================================================================
typedef void (*AppFuncT)();
//=================================================================================================================================
typedef struct RequestWriteT
{
  uint32_t StartAddress;
  uint16_t DataSize;
  uint16_t Action;
} RequestWriteT;
//=================================================================================================================================
typedef struct RequestReadT
{
  uint32_t StartAddress;
  uint16_t DataSize;
  uint16_t Action;
} RequestReadT;
//=================================================================================================================================
typedef struct RequestEraseT
{
  uint32_t StartAddress;
  uint32_t EndAddress;
  uint32_t Action;
} RequestEraseT;
//=================================================================================================================================
typedef struct RequestCrcT
{
  uint32_t StartAddress;
  uint32_t EndAddress;
  uint32_t Action;
} RequestCrcT;
//=================================================================================================================================
typedef struct BootloaderInfoT
{
  uint32_t StartAddress;
  uint32_t EndAddress;
  uint32_t Requests;
  uint16_t PageSize;
  uint16_t Crc;
} BootloaderInfoT;
//=================================================================================================================================
typedef struct AppInfoT
{
  uint32_t StartAddress;
  uint32_t EndAddress;
  uint16_t PageSize;
  uint16_t Crc;
} AppInfoT;
//=================================================================================================================================
typedef struct BootloaderStatusT
{
  union
  {
    struct
    {
      uint32_t JumpToMain : 1;
    };
    uint32_t Value;
  };
  
  uint32_t WriteAddress;
  uint32_t ReadAddress;
} BootloaderStatusT;
//=================================================================================================================================
typedef struct BootloaderT
{
  BootloaderStatusT Status;  
  BootloaderInfoT Info;
  AppInfoT AppInfo;
  
} BootloaderT;
//=================================================================================================================================
extern BootloaderT Bootloader;
//=================================================================================================================================
int16_t ActionTryWrite(xObject context, RequestWriteT* request, uint16_t object_size);
int16_t ActionTryErase(xObject context, RequestEraseT* request, uint16_t object_size);
int16_t ActionTryJumpToMain(xObject context);
int16_t ActionTryReset(xObject context);
int16_t ActionTryUpdateInfo(xObject context);

int16_t ActionSetLockState(xObject context, uint8_t* request);
//=================================================================================================================================
#endif /* BOOTLOADER_STM32_H_ */
