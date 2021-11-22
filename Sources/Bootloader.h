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
//=================================================================================================================================
typedef void (*AppFuncT)();
//=================================================================================================================================
typedef struct RequestWriteDataT
{
  uint32_t Address;
  uint32_t Action;
}RequestWriteDataT;
//=================================================================================================================================
typedef struct BootloaderInfoT
{
  uint32_t StartAddress;
  uint32_t EndAddress;
  uint16_t Crc;
  uint16_t Handler;
}BootloaderInfoT;
//=================================================================================================================================
typedef struct BootloaderT
{
  BootloaderInfoT Info;
}BootloaderT;

extern BootloaderT Bootloader;
//=================================================================================================================================
#endif /* BOOTLOADER_STM32_H_ */
