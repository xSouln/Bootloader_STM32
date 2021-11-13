/*
 * DEVICE_PORTS_H_.h
 *
 *  Created on: 27.09.2021
 *      Author: rekuts
 */

#ifndef DEVICE_PORTS_H_
#define DEVICE_PORTS_H_
//=================================================================================================================================
#include "xType.h"
#include "gpio.h"
//=================================================================================================================================
typedef union{
  struct{
    /* Pin0 */  uint32_t ADC_IN0: 1;
    /* Pin1 */  uint32_t ADC_IN1: 1;
    /* Pin2 */  uint32_t ADC_IN2: 1;
    /* Pin3 */  uint32_t ADC_IN3: 1;
    
    /* Pin4 */  uint32_t ADC_IN4: 1;
    /* Pin5 */  uint32_t ADC_IN5: 1;
    /* Pin6 */  uint32_t Pin6: 1;
    /* Pin7 */  uint32_t Pin7: 1;
    
    /* Pin8 */  uint32_t Pin8: 1;
    /* Pin9 */  uint32_t USART1_TX: 1;
    /* Pin10 */ uint32_t USART1_RX: 1;
    /* Pin11 */ uint32_t USB_DM: 1;
    
    /* Pin12 */ uint32_t USB_DP: 1;
    /* Pin13 */ uint32_t SWDIO: 1;
    /* Pin14 */ uint32_t SWDCLK: 1;
    /* Pin15 */ uint32_t Pin15: 1;
  };
  volatile uint32_t Value;
} PortA;
//=================================================================================================================================
typedef union{
  struct{
    /* Pin0 */  uint32_t Pin0: 1;
    /* Pin1 */  uint32_t Pin1: 1;
    /* Pin2 */  uint32_t Pin2: 1;
    /* Pin3 */  uint32_t Pin3: 1;
    
    /* Pin4 */  uint32_t Pin4: 1;
    /* Pin5 */  uint32_t Pin5: 1;
    /* Pin6 */  uint32_t Pin6: 1;
    /* Pin7 */  uint32_t Pin7: 1;
    
    /* Pin8 */  uint32_t Pin8: 1;
    /* Pin9 */  uint32_t Pin9: 1;
    /* Pin10 */ uint32_t Pin10: 1;
    /* Pin11 */ uint32_t Pin11: 1;
    
    /* Pin12 */ uint32_t Pin12: 1;
    /* Pin13 */ uint32_t Pin13: 1;
    /* Pin14 */ uint32_t Pin14: 1;
    /* Pin15 */ uint32_t Pin15: 1;
  };
  volatile uint32_t Value;
} PortB;
//=================================================================================================================================
typedef union{
  struct{
    /* Pin0 */  uint32_t Pin0: 1;
    /* Pin1 */  uint32_t Pin1: 1;
    /* Pin2 */  uint32_t Pin2: 1;
    /* Pin3 */  uint32_t Pin3: 1;
    
    /* Pin4 */  uint32_t Pin4: 1;
    /* Pin5 */  uint32_t Pin5: 1;
    /* Pin6 */  uint32_t Pin6: 1;
    /* Pin7 */  uint32_t Pin7: 1;
    
    /* Pin8 */  uint32_t Pin8: 1;
    /* Pin9 */  uint32_t Pin9: 1;
    /* Pin10 */ uint32_t Pin10: 1;
    /* Pin11 */ uint32_t Pin11: 1;
    
    /* Pin12 */ uint32_t Pin12: 1;
    /* Pin13 */ uint32_t LED: 1;
    /* Pin14 */ uint32_t Pin14: 1;
    /* Pin15 */ uint32_t Pin15: 1;
  };
  volatile uint32_t Value;
} PortC;
//=================================================================================================================================
typedef struct CK_Ports
{
  struct
  {
    volatile PortA *In;
    volatile PortA *Out;
  } A;

  struct
  {
    volatile PortB *In;
    volatile PortB *Out;
  } B;

  struct
  {
    volatile PortC *In;
    volatile PortC *Out;
  } C;
} PortsT;
//=================================================================================================================================
extern PortsT Ports;
//=================================================================================================================================
#endif /* DEVICE_PORTS_H_ */
