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
void Response_REQUEST_GET(xRequestT *request, xTxT *tx, xObject request_obj, uint16_t request_obj_size, uint8_t error){
  ResponseInfoT info = { .Key = request->Id, .Size = request->Content.size };  
  xTxAdd(tx, &info, sizeof(info));  
  xTxAdd(tx, request->Content.obj, request->Content.size);
}
//=================================================================================================================================
void Response_REQUEST_DEFAULT(xRequestT *request, xTxT *tx, xObject request_obj, uint16_t request_obj_size, uint8_t error){
  ResponseInfoT info = { .Key = request->Id, .Size = request->Content.size + sizeof(error) };  
  xTxAdd(tx, &info, sizeof(info));
  xTxAdd(tx, &error, sizeof(error));
  xTxAdd(tx, request->Content.obj, request->Content.size);
}
//=================================================================================================================================
int rx_endline(xObject context, uint8_t *obj, uint16_t size){ 
  if(size >= sizeof(RequestHeaderT) && obj[0] == REQUEST_START_CHARACTER && obj[5] == RESPONSE_END_CHARACTER)
  {
    if(size < sizeof(RequestT)) { return RX_STORAGE; }
    RequestT *request = (RequestT*)obj;
    uint16_t action_size = size - sizeof(RequestT);

    if(action_size < request->Info.Size) { return RX_STORAGE; }
    if(action_size > request->Info.Size) { return RX_RESET; }    
    
    uint8_t *action = obj + sizeof(RequestT);
    uint8_t action_error = ACCEPT;
    
    uint8_t i = 0;
    while(Requests[i].Id != (uint16_t)-1)
    {
      if(request->Info.Key == Requests[i].Id)
      {
        if(Requests[i].Control) { action_error = Requests[i].Control(0, action, action_size); }
        
        if(Requests[i].Response)
        {
          xTxAdd(&UsartX.Tx, RESPONSE_HEADER, sizeof_str(RESPONSE_HEADER));
          Requests[i].Response(&Requests[i], &UsartX.Tx, action, action_size, action_error);
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
  { .Id = -1 }
};
//=================================================================================================================================
