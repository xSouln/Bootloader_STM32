/*
 * BootTypes.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//=================================================================================================================================
#ifndef BOOT_TYPES_H_
#define BOOT_TYPES_H_
//=================================================================================================================================
#include <stdint.h>
#include <stdbool.h>
//=================================================================================================================================
enum ERRORS_BOOT
{
  BOOT_ACCEPT = 0,
  BOOT_ERROR_DATA,
  BOOT_ERROR_CONTENT_SIZE,
  BOOT_ERROR_REQUEST,
  BOOT_ERROR_RESOLUTION,
  BOOT_UNKNOWN_COMMAND,
  BOOT_TIMEOUT,
  BOOT_BUSY,
  BOOT_OUTSIDE,
  BOOT_ERROR_ACTION,
  BOOT_LOCKED
};
//=================================================================================================================================
#endif /* BOOT_TYPES_H_ */
