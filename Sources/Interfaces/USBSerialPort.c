//=================================================================================================================================
#include <string.h>
#include "USBSerialPort.h"
#include "xPort.h"
#include "xRequest.h"
#include "Responses.h"
#include "usbd_conf.h"
#include "stm32f1xx_hal_pcd.h"
//=================================================================================================================================
#define USBX_RX_CIRCLE_BUF_SIZE_MASK CDC_DATA_HS_OUT_PACKET_SIZE
#define USBX_RX_OBJECT_BUF_SIZE 0x1ff
#define USBX_TX_CIRCLE_BUF_SIZE_MASK 0x3ff

TX_BUF_INIT(USBX);
RX_BUF_INIT(USBX);
//=================================================================================================================================
extern USBD_HandleTypeDef hUsbDeviceFS;
USBD_CDC_HandleTypeDef *USBSerialPortHandle;
//=================================================================================================================================
static int8_t USBSerialPortInit(){
  USBSerialPortHandle = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  USBSerialPortHandle->RxBuffer = USBX_RX_CIRCLE_BUF;
  return ACCEPT;
}
//=================================================================================================================================
static int8_t USBSerialPortDeInit(){
  
  return ACCEPT;
}
//=================================================================================================================================
static int8_t USBSerialPortHandler(uint8_t cmd, uint8_t* pbuf, uint16_t length){
  
  return ACCEPT;
}
//=================================================================================================================================
static int8_t USBSerialPortReceive(uint8_t* buf, uint32_t *len){
  xRxAdd(&USBSerialPort.Rx, buf, (uint16_t)*len);  
  HAL_PCD_EP_Receive(hUsbDeviceFS.pData, CDC_OUT_EP, USBX_RX_CIRCLE_BUF, USBX_RX_CIRCLE_BUF_SIZE_MASK);
  return ACCEPT;
}
//=================================================================================================================================
int8_t USBSerialPortTransmit(uint8_t* buf, uint16_t len){
  if(USBSerialPortHandle->TxState == 0){ USBSerialPortHandle->TxState = BUSY;
    HAL_PCD_EP_Transmit(hUsbDeviceFS.pData, CDC_IN_EP, buf, len);
    return ACCEPT;
  }
  return BUSY;
}
//=================================================================================================================================
inline void USBSerialPortThread(){
  if(USBSerialPortHandle->TxState == 0 && USBSerialPort.Tx.State.HandlerIndex != USBSerialPort.Tx.State.TotalIndex){
    xPacketT packet = xTxGetPacket(&USBSerialPort.Tx);
    USBSerialPortHandle->TxState = BUSY;
    HAL_PCD_EP_Transmit(hUsbDeviceFS.pData, CDC_IN_EP, packet.ptr, packet.size);
  }
}
//=================================================================================================================================
/*
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd){
  switch(hpcd->State){
  case HAL_PCD_STATE_RESET: break;
  case HAL_PCD_STATE_READY: break;
  case HAL_PCD_STATE_ERROR: break;
  case HAL_PCD_STATE_BUSY: break;
  case HAL_PCD_STATE_TIMEOUT: break;
  }
}
*/
//=================================================================================================================================
USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  USBSerialPortInit,
  USBSerialPortDeInit,
  USBSerialPortHandler,
  USBSerialPortReceive
};
//=================================================================================================================================
USBSerialPortT USBSerialPort =
{
  .Rx =
  {
    RX_OBJECT_RECEIVER_INIT(USBX, rx_endline),
    RX_CIRCLE_RECEIVER_INIT(USBX)
  },
  
  .Tx =
  {
    TX_BINDING(USBX, 0)
  }
};
//=================================================================================================================================
