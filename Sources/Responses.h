/*
 * CK_Responses.h
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */

#ifndef _BL_RESPONSES_H_
#define _BL_RESPONSES_H_
//=================================================================================================================================
#include "xType.h"
//=================================================================================================================================
enum REQUESTS{
  //============================
  REQUESTS_START = 0,
    
  GET_X = 100,
  GET_INFO,
  GET_HANDLER,
  
  SET_X = 200,
  SET_OPTIONS,
  SET_HANDLER,
  
  TRY_X = 300,
  TRY_ERASE,
  TRY_WRITE,
  TRY_READ,
  TRY_STOP,
  TRY_JUMP_TO_MAIN,
  TRY_JUMP_TO_BOOT,
  
  EVT_X = 400,
  EVT_WRITE_COMPLITE,
  EVT_READ_COMPLITE,
  
  REQUESTS_END = 500
  //============================
};
//=================================================================================================================================
extern xRequestT Requests[];
//=================================================================================================================================
int rx_endline(xObject context, uint8_t *obj, uint16_t size);
//=================================================================================================================================
#endif /* _BL_RESPONSES_H_ */
