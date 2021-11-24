/*
 * Responses.c
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */
//=================================================================================================================================
#include "xRequest.h"
#include "Responses.h"
#include "xTx.h"
#include "UsartX.h"
#include "Bootloader.h"
#include "xFlash.h"
//=================================================================================================================================
#define VA_ARGS_SHIFT(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,PN,...) PN
#define VA_ARGS_COUNT(...) VA_ARGS_SHIFT(-1,##__VA_ARGS__,9,8,7,6,5,4,3,2,1,0)

//#define CONTENT_PUT(arg_obj, arg_size) {   }
#define xRequestDef(name, size1, ...) \
xContent RequestContent_##name[VA_ARGS_COUNT(__VA_ARGS__)] = { { .obj = &#__VA_ARGS__, .size = sizeof(#__VA_ARGS__) } }; \
xObject RequestObjects_##name[VA_ARGS_COUNT(__VA_ARGS__)] = { &#__VA_ARGS__ }; \
uint8_t RequestObjectSize_##name[VA_ARGS_COUNT(__VA_ARGS__)] = { sizeof(#__VA_ARGS__) }; \
xRequestT Request_##name = { .Content = { 0 }, .Contents = RequestContent_##name }
//=================================================================================================================================
xRequestT debug_request;
uint8_t* debug_action;
uint16_t debug_action_size;
//=================================================================================================================================
void Response_REQUEST_GET(xTxT *tx, xRequestT *request, xObject request_obj, uint16_t request_obj_size, int16_t error){
  ResponseInfoT info = { .Key = request->Id, .Size = request->Content.size };  
  xTxAdd(tx, &info, sizeof(info));  
  xTxAdd(tx, request->Content.obj, request->Content.size);
}
//=================================================================================================================================
void Response_REQUEST_DEFAULT(xTxT *tx, xRequestT *request, xObject request_obj, uint16_t request_obj_size, int16_t error){
  ResponseInfoT info = { .Key = request->Id, .Size = request->Content.size + sizeof(error) };  
  xTxAdd(tx, &info, sizeof(info));
  xTxAdd(tx, &error, sizeof(error));
  xTxAdd(tx, request->Content.obj, request->Content.size);
}
//=================================================================================================================================
int rx_endline(xObject context, uint8_t *obj, uint16_t size){ 
  if(size >= sizeof(RequestHeaderT) && obj[0] == REQUEST_START_CHARACTER && obj[5] == RESPONSE_END_CHARACTER)
  {
    //if(size < sizeof(RequestT)) { return RX_STORAGE; }
    RequestT *request = (RequestT*)obj;
    uint16_t action_size = size - sizeof(RequestT);

    if(action_size < request->Info.Size) { return RX_STORAGE; }
    if(action_size > request->Info.Size) { return RX_RESET; }    
    
    uint8_t* action = obj + sizeof(RequestT);
    uint8_t action_error = ACCEPT;
    
    uint8_t i = 0;
    while(Requests[i].Id != (uint16_t)-1)
    {
      if(request->Info.Key == Requests[i].Id)
      {
        memcpy(&debug_request, &Requests[i], sizeof(debug_request));
        debug_action = action;
        debug_action_size = action_size;
        if(Requests[i].Control) { action_error = Requests[i].Control(&UsartX.Tx, action, action_size); }
        
        if(Requests[i].Response)
        {
          xTxAdd(&UsartX.Tx, RESPONSE_HEADER, sizeof_str(RESPONSE_HEADER));
          Requests[i].Response(&UsartX.Tx, &Requests[i], action, action_size, action_error);
          xTxAdd(&UsartX.Tx, RESPONSE_END, sizeof_str(RESPONSE_END));
        }
        break;
      }
      i++;
    }
  }
  return RX_RESET;
}
//=================================================================================================================================
//=================================================================================================================================
xRequestT Requests[] = {
  NEW_REQUEST(GET_INFO, Response_REQUEST_GET, 0, Bootloader.Info),
  NEW_REQUEST(GET_APP_INFO, Response_REQUEST_GET, 0, Bootloader.AppInfo),
  NEW_REQUEST(GET_FIRMWARE, Response_REQUEST_GET, 0, FIRMWARE_VERSION),
  NEW_REQUEST(GET_STATUS, Response_REQUEST_GET, 0, xFlash.Status),
  
  NEW_REQUEST(SET_FLASH_LOCK_STATE, Response_REQUEST_DEFAULT, ActionSetLockState, xFlash.Status),
  
  NEW_REQUEST(TRY_WRITE, Response_REQUEST_DEFAULT, ActionTryWrite, xFlash.Status),
  NEW_REQUEST(TRY_ERASE, Response_REQUEST_DEFAULT, ActionTryErase, xFlash.Status),
  NEW_REQUEST(TRY_JUMP_TO_MAIN, Response_REQUEST_DEFAULT, ActionTryJumpToMain, xFlash.Status),
  NEW_REQUEST(TRY_UPDATE_INFO, Response_REQUEST_DEFAULT, ActionTryUpdateInfo, Bootloader.Info),
  //NEW_REQUEST(TRY_READ_CRC, Response_REQUEST_DEFAULT, ActionReadInfo, Bootloader.Info),
  { .Id = -1 }
};
//=================================================================================================================================
