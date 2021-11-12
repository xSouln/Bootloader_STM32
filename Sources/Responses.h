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
enum REQUEST{
  //============================
  SYSTEM = 0,
    
  GET_BY_ID = 100,
  GET_DRV_STATE,
  GET_HANDLER,
  
  SET_HANDLER = 200,
  SET_OPTIONS,
  SET_POSITION,
  SET_REQUESTS,
  SET_MOTION_STEPS,
  
  TRY = 300,
  TRY_CLEAR,
  TRY_MOTION_START,
  TRY_STOP,
  TRY_RESET_STEPS,
  
  EVT_POSITION_SET = 400,
  EVT_STOPPED,
  
  SYSTEM_END = 500
  //============================
};

enum ERRORS{
  ACCEPT = 0,
  ERROR_DATA,
  ERROR_CONTENT_SIZE,
  ERROR_REQUEST,
  ERROR_RESOLUTION,
  UNKNOWN_COMMAND,
  BUSY,
  OUTSIDE,
  ERROR_ACTION,
  ERROR_POSITION
};
//=================================================================================================================================
extern xRequestT Requests[];
//=================================================================================================================================
int rx_endline(uint8_t *obj, uint16_t size);
//=================================================================================================================================
#endif /* _BL_RESPONSES_H_ */
