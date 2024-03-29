/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "usart.h"
#include "dma.h"
#include "flash.h"
#include "string.h"
#include "iap_bootloader.h"
#include "tim.h"
#include "rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/**
 * @description: Uart IDLE interrupt Handle
 * @param {UART_HandleTypeDef} *huart
 * @param {uint16_t} Size 接收到数据的大小
 * @return {*}
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    uint32_t buf_32 = 0;
    /*接收完成*/
    printf("\r\nReceive completed! (File Size:%.2fKb=%dbyte)\r\n",(float)Size / 1024.0,Size);
    /*拼接*/
    buf_32 = ((uint32_t)uart1RxBuff[7] << 24) | ((uint32_t)uart1RxBuff[6] << 16) | ((uint16_t)uart1RxBuff[5] << 8) | uart1RxBuff[4];
    /*判断接收的是否为flash程序*/
    printf("\r\nChecking data...\r\n");
    if((buf_32 & 0xFF000000) != 0x08000000)
    {
      /*判断为不合法的app*/
      printf("Err:Invalid flash app!\r\n");
      /*重新启动串口IDLE接收*/
      HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)&uart1RxBuff, UART1RXBUFFSIZE);
      /*等待发送程序包*/
      printf("Please send again .bin file (less than %dKb) to STM32 ...\r\n",UART1RXBUFFSIZE);
      /*跳出当前程序 等待再次接收中断*/
      return;
    }
    /*将定时器配置成1ms计数一次*/
    __HAL_TIM_SetCounter(&htim6,0);
    HAL_TIM_Base_Start(&htim6);
    /*擦除APP程序区 总512页 每页128bytes*/
    printf("\r\nErasing Flash...\r\n");
    FLASH_Erase_Page(APPLICATION_ADDRESS, 512 - BOOTLOADER_FLASH_SIZE / 128);
    /*Flash 擦除完成*/
    printf("\r\nErase Completed! (took %dms)\r\n",__HAL_TIM_GetCounter(&htim6));
    /*写入Flash*/
    __HAL_TIM_SetCounter(&htim6,0);
    printf("\r\nPrograming Flash...\r\n");
    IAP_Bootloader_FlashProgram(APPLICATION_ADDRESS, uart1RxBuff, Size);
    /*Flash 写入完成*/
    printf("\r\nProgram Completed! (took %dms)\r\n",__HAL_TIM_GetCounter(&htim6));
    printf("\r\nRestarting system...\r\n");
    HAL_TIM_Base_Stop(&htim6);
    /*恢复备份寄存器*/
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x0000U);
    /*重启进入APP*/
    __NVIC_SystemReset();
}
/**
 * @description: 串口接收错误函数
 * @param {UART_HandleTypeDef} *huart
 * @return {*}
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /*打印错误信息*/
  printf("UART Error!\r\n");
  Error_Handler();
}

/* USER CODE END 1 */
