/*
 * @Author: Tomood
 * @Date: 2024-02-18 22:54:12
 * @LastEditTime: 2024-02-19 16:34:39
 * @FilePath: \MDK-ARMd:\STM32Prj\IAP_Bootloader_demo\Core\Inc\menu.h
 * @Description: 
 * 
 * Copyright (c) 2024 by Tomood, All Rights Reserved. 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENU_H
#define __MENU_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Imported variables --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Main_Menu(void);

#endif
