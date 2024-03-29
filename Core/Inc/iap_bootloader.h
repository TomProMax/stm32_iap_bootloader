/*
 * @Author: Tomood
 * @Date: 2024-02-19 13:48:32
 * @LastEditTime: 2024-02-19 14:08:32
 * @FilePath: \MDK-ARMd:\STM32Prj\IAP_Bootloader_demo\Core\Inc\iap_bootloader.h
 * @Description: 
 * 
 * Copyright (c) 2024 by Tomood, All Rights Reserved. 
 */
#ifndef _IAP_BOOTLOADER_H_
#define _IAP_BOOTLOADER_H_
 
#include "main.h"
 
typedef void (*APP_FUNC)(); 	//函数指针类型定义
void iap_load_app(uint32_t appxaddr);	//跳转函数
void IAP_Bootloader_FlashProgram(uint32_t addr, uint8_t *pbuf, uint16_t length);
#endif
 
 