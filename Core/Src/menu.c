/*
 * @Author: Tomood
 * @Date: 2024-02-18 22:53:58
 * @LastEditTime: 2024-02-22 01:22:25
 * @FilePath: \MDK-ARMd:\STM32Prj\IAP_Bootloader_demo\Core\Src\menu.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Tomood, All Rights Reserved. 
 */

/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include "stdio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @description: Display the Main Menu on HyperTerminal
 * @return {*}
 */
void Main_Menu(void)
{
  printf("\r\n******************************************************");
  printf("\r\n***           STM32 IAP Bootloader                 ***");
  printf("\r\n***                                                ***");
  printf("\r\n***                                                ***");
  printf("\r\n***                         Version 1.0  By Tomood ***");
  printf("\r\n******************************************************");
  printf("\r\n");
}
