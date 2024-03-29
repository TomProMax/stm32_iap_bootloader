/*
 * @Author: Tomood
 * @Date: 2024-02-19 14:08:02
 * @LastEditTime: 2024-02-22 06:13:09
 * @FilePath: \MDK-ARMd:\STM32Prj\IAP_Bootloader_demo\Core\Inc\flash.h
 * @Description: 
 * 
 * Copyright (c) 2024 by Tomood, All Rights Reserved. 
 */

#ifndef __FLASH_H_
#define __FLASH_H_
 
#include "main.h"

void FLASH_Init(void);
void FLASH_Erase_Page(uint32_t start_addr,uint32_t NbPages);
uint32_t FLASH_ReadWord(uint32_t address);
void FLASH_ProgramWord(uint32_t Address, uint32_t Data);


#endif
 
 