//===================================================================================================
#ifndef X_FLASH_H
#define X_FLASH_H
//===================================================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
//===================================================================================================
int8_t xFlashErase(uint32_t page_address);
int8_t xFlashErasePages(uint32_t page_address, uint16_t page_size, uint16_t page_count);

int8_t xFlashWrite(uint32_t address, uint8_t* data, uint16_t len);
int8_t xFlashWriteHalfWord(uint32_t address, uint16_t data);
//===================================================================================================
//===================================================================================================
#endif
