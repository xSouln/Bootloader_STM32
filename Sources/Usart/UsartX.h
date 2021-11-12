//=================================================================================================================================
#ifndef _USARTX_CONTROL_H_
#define _USARTX_CONTROL_H_
//=================================================================================================================================
#include <stdint.h>
#include <stdbool.h>
#include <xTx.h>
#include "xType.h"
#include "xRx.h"
//=================================================================================================================================
typedef struct{  
  xRxT Rx;	
  xTxT Tx;
  volatile UsartReg_T *Reg;
}UsartX_T;
//=================================================================================================================================
extern UsartX_T UsartX;
//=================================================================================================================================
extern inline void usart1_handler();
extern inline void usart1_init();
//=================================================================================================================================
#endif
