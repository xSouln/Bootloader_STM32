
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2021 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "xTimer.h"
#include "Ports.h"
#include "USBSerialPort.h"
#include "xThread.h"
#include "xList.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//THREAD_INIT(Main, 0, 0x07, 0);
//TIMER_INIT(Main, 0, 5);
xThreadT ThreadMain;
xTimerT TimerMain;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
xListT List;
int remove_action = -1;
int add_action = -1;
int insert_action = -1;
int count;
int phase = 0;

xTimerRequestT *TimerRequest1;
xTimerRequestT *TimerRequest2;
xTimerRequestT *TimerRequest3;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void ThreadAction1(xThreadT* context, xThreadRequestT* request){
  /*
  for(int i = 0; i < List.Count; i++){
    uint8_t* str = xListGet(&List, i);
    xTxAdd(&USBSerialPort.Tx, str, strlen((char*)str));
  }
  xTxAdd(&USBSerialPort.Tx, "\r", 1);
*/
  xTxAdd(&USBSerialPort.Tx, "\rThreadAction1\r", strlen("\rThreadAction1\r"));
}

void ThreadAction2(xThreadT* context, xThreadRequestT* request){
  Ports.C.Out->LED ^= true;
}

void ThreadAction3(xThreadT* context, xThreadRequestT* request){
  count++;
  
  switch(phase){
  case 0: xTxAdd(&USBSerialPort.Tx, "phase:0 ", strlen("phase:0 ")); break;
  case 1: xTxAdd(&USBSerialPort.Tx, "phase:1 ", strlen("phase:1 ")); break;
  case 2: xTxAdd(&USBSerialPort.Tx, "phase:2 ", strlen("phase:2 ")); break;
  case 3: xTxAdd(&USBSerialPort.Tx, "phase:3 ", strlen("phase:3 ")); break;
  case 4: xTxAdd(&USBSerialPort.Tx, "phase:4 ", strlen("phase:4 ")); break;
  case 5: xTxAdd(&USBSerialPort.Tx, "phase:5 ", strlen("phase:5 ")); break;
  default: phase = 0; return;
  }
  phase++;
}

void TimerAction1(xTimerT* context, xTimerRequestT* request){
  /*
  xTxAdd(&USBSerialPort.Tx, request->Object, request->ObjectSize);
  xTxAdd(&USBSerialPort.Tx, request->Object, request->ObjectSize);
*/  
  xThreadAdd(&ThreadMain, (xThreadAction)ThreadAction1, 0, 0, 0);
}

void TimerAction2(xTimerT* context, xTimerRequestT* request){
  xThreadAdd(&ThreadMain, (xThreadAction)ThreadAction2, 0, 0, 0);
  xThreadAdd(&ThreadMain, (xThreadAction)ThreadAction3, 0, 0, 0);
}

void TimerAction3(xTimerT* context, xTimerRequestT* request){
  //xThreadAdd(&ThreadMain, (xThreadAction)ThreadAction3, 0, 0, 0);
  xThreadAdd(&ThreadMain, (xThreadAction)ThreadAction3, 0, 0, 0);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  TimerRequest1 = xTimerAdd(&TimerMain, (xTimerAction)TimerAction1, 1000, 1000);
  TimerRequest1->Object = "timer\r";
  TimerRequest1->ObjectSize = 6;
  TimerRequest1->Handler.Enable = true;
  
  TimerRequest2 = xTimerAdd(&TimerMain, (xTimerAction)TimerAction2, 1000, 500);
  TimerRequest2->Handler.Enable = true;
  
  TimerRequest3 = xTimerAdd(&TimerMain, (xTimerAction)TimerAction3, 1000, 100);  
  TimerRequest3->Handler.Enable = true;
  
  xListAdd(&List, "state:1 ");
  xListAdd(&List, "state:2 ");
  xListAdd(&List, "state:3 ");
  xListAdd(&List, "state:4 ");
  xListAdd(&List, "state:5 ");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    USBSerialPortThread();
    xThread(&ThreadMain);
    xTimer(&TimerMain);
    
    if(remove_action != -1){
      xListRemoveAt(&List, remove_action);
      remove_action = -1;
    }
    
    if(add_action != -1){
      switch(add_action){
      case 0: xListAdd(&List, "state:1 "); break;
      case 1: xListAdd(&List, "state:1 "); break;
      case 2: xListAdd(&List, "state:2 "); break;
      case 3: xListAdd(&List, "state:3 "); break;
      case 4: xListAdd(&List, "state:4 "); break;
      case 5: xListAdd(&List, "state:5 "); break;
      }
      add_action = -1;
    }
    
    if(insert_action != -1){
      xListInsert(&List, insert_action, "state:x ");
      insert_action = -1;
    }
    
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 1, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
