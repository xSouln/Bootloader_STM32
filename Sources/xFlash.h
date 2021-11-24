//===================================================================================================
#ifndef X_FLASH_H
#define X_FLASH_H
//===================================================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
#include "BootTypes.h"
//===================================================================================================
#define XFLASH_PAGE_SIZE 0x400
#define XFLASH_END_ADDRESS 0x0801FFFF
//===================================================================================================
enum xFlashOperationT
{
  FlashOperationFree,
  FlashOperationErase,
  FlashOperationWrite,
  FlashOperationRead,
  FlashOperationLock
};
//===================================================================================================
typedef struct xFlashStatusT
{
  union
  {
    struct
    {
      uint32_t Write : 1;
      uint32_t Read : 1;
      uint32_t Erase : 1;
      uint32_t FlashUnlocked : 1;
      
      uint32_t Busy : 1;
    };
    uint32_t Value;
  };
  
  uint32_t WriteAddress;
  uint32_t EraseAddress;
  uint32_t ReadAddress;
  
  uint16_t Operation;
  uint16_t OperationTime;
  
  uint16_t OperationResult;
} xFlashStatusT;
//===================================================================================================
typedef struct
{
  xFlashStatusT Status;
  volatile uint32_t OperationTimeout;
}xFlashT;
//===================================================================================================
extern xFlashT xFlash;
//===================================================================================================
int8_t xFlashErasePages(uint32_t start_address, uint32_t end_address, uint32_t timeout);

int8_t xFlashWrite(uint32_t address, uint8_t* data, uint16_t len, uint32_t timeout);
int8_t xFlashWriteHalfWord(uint32_t address, uint16_t data, uint32_t timeout);

int8_t xFlashRead(uint32_t address, volatile uint8_t* data, uint16_t len);

int8_t xFlashSetLock(bool state);

uint16_t xFlashGetCrc(uint32_t start_address, uint32_t end_address);
//===================================================================================================
//===================================================================================================
#endif
