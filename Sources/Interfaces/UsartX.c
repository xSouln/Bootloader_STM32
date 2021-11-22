//=================================================================================================================================
#include <string.h>
#include "xPort.h"
#include "xRequest.h"
#include "main.h"
#include "usart.h"
#include "xRx.h"
#include "xTx.h"
#include "UsartX.h"
#include "Responses.h"
//=================================================================================================================================
#define USARTX_RX_CIRCLE_BUF_SIZE_MASK 0x1ff
#define USARTX_RX_OBJECT_BUF_SIZE 0x1ff
#define USARTX_TX_CIRCLE_BUF_SIZE_MASK 0x3ff
//=================================================================================================================================
TX_BUF_INIT(USARTX);
RX_BUF_INIT(USARTX);
//=================================================================================================================================
//=================================================================================================================================
inline void usart1_handler()
{
  xRxUpdate(&UsartX.Rx);
  
  if(UsartX.Tx.State.HandlerIndex != UsartX.Tx.State.TotalIndex && !UsartX.Reg->CR1.TxEmptyInterruptEnable){ UsartX.Reg->CR1.TxEmptyInterruptEnable = true; }
}
//=================================================================================================================================
inline void usart1_init()
{
  UsartX.Reg->CR1.ReceiverEnable = true;
  UsartX.Reg->CR1.RxNotEmptyInterruptEnable = true;

  //Ports.B.Out->USART1_DE = false;
  UsartX.Reg->CR1.TransmitterEnable = true;
  UsartX.Reg->CR1.TxCompleteInterruptEnable = false;
  UsartX.Reg->CR1.TxEmptyInterruptEnable = false;
}
//=================================================================================================================================
bool usart1_transmit_action(xObject context, uint8_t* ptr, uint16_t size)
{
  //Ports.B.Out->USART1_DE = true;
  //UsartX.Tx.Packet = *packet;
  UsartX.Reg->CR1.TxEmptyInterruptEnable = true;
  return true;
}
//=================================================================================================================================
UsartX_T UsartX = {
  .Rx = //&USARTX_RX,  
  {
    RX_OBJECT_RECEIVER_INIT(USARTX, rx_endline),
    RX_CIRCLE_RECEIVER_INIT(USARTX)
  },
  
  .Tx =
  {
    TX_BINDING(USARTX, usart1_transmit_action)
  },

  .Reg = (UsartReg_T*)USART1
};
//=================================================================================================================================
