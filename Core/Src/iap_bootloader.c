/*
 * @Author: Tomood
 * @Date: 2024-02-19 13:34:54
 * @LastEditTime: 2024-02-22 11:33:10
 * @FilePath: \MDK-ARMd:\STM32Prj\IAP_Bootloader_demo\Core\Src\iap_bootloader.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Tomood, All Rights Reserved. 
 */
#include "iap_bootloader.h"
#include "stdio.h"
#include "tim.h"

/*定义一个void (*pFunction)(void)函数指针 并typedef提升为类型名*/
typedef void (*pFunction)(void);
pFunction JumpToApplication;
uint32_t JumpAddress;

/**
 * @description: IAP跳转APP应用程序
 * @param {uint32_t} appxaddr APP程序首地址
 * @return {*}
 */
void iap_load_app(uint32_t appxaddr)
{
    printf("\r\nChecking the app program in flash.\r\n");
    /* 判断FLASH里面是否有APP,有的话跳转 */
    if (((*(volatile uint32_t *)(APPLICATION_ADDRESS + 4)) & 0xFF000000) == 0x08000000)
    {
        /*检查栈顶地址是否合法 判断用户代码的堆栈地址是否落在0x20000000~0x2001ffff之间*/
        if(((*(__IO uint32_t *)appxaddr) & 0x2FFE0000) == 0x20000000)
        { 
            printf("\r\nCheck passed! \r\n");
            printf("Booting from flash at 0x%x\r\n", APPLICATION_ADDRESS);
            /*首地址是MSP，地址+4是复位中断服务程序地址*/
            JumpAddress = *(__IO uint32_t*)(appxaddr + 4);
            /*令JumpToApplication这个函数指针指向复位函数入口地址*/
            JumpToApplication = (pFunction) JumpAddress;
            /*关闭中断*/
            __set_PRIMASK(1);
            /*关闭滴答定时器*/
            SysTick->CTRL = 0;
            SysTick->LOAD = 0;
            SysTick->VAL = 0;
            /*设置所有时钟到默认状态*/
            HAL_RCC_DeInit();
            for (uint8_t i = 0; i < 8; i++)
            {   
                /*关闭所有中断，清除所有中断挂起标志*/
                NVIC->ICER[i]=0xFFFFFFFF;
                NVIC->ICPR[i]=0xFFFFFFFF;
            }
            /*重新使能中断*/
            __set_PRIMASK(0);
            /*初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)*/
            __set_MSP(*(__IO uint32_t*)appxaddr);
            /*跳转app程序*/
            JumpToApplication();
        }
    }
    else
    {
        /*打印错误信息：非法应用程序*/
        printf("\r\nError:Illegal APP program!\r\n");
        Error_Handler();
    }
}
/**
 * @description: IAP_Bootloader_FlashProgram
 * @param addr   : 目标起始Flash地址 (此地址必须为2的倍数!!,否则写入出错!)
 * @param pbuf   : 数据指针
 * @param length : 要写入的字节数
 * @return {*}
 */
void IAP_Bootloader_FlashProgram(uint32_t addr, uint8_t *pbuf, uint16_t length)
{
    uint16_t loop_times = 0;
    uint16_t i = 0;
    uint32_t buf_32 = 0;
    HAL_FLASH_Unlock();
    /*如果要写入的数据正好为整数个word*/
    if(length % 4 == 0)
        loop_times = length / 4;
    /*如果要写入的数据不是整数个word*/
    else
        loop_times = length / 4 + 1;
    /*循环处理格式转换*/
    for(i = 0; i < loop_times; i++)
    {
        /*将u8数据拼接成u32(word) 注意stm32l0为小端 需要反着写哈哈哈哈哈 2024.2.22 4:46PM 踩坑寄录 */
        buf_32 = ((uint32_t)pbuf[4 * i + 3] << 24) | ((uint32_t)pbuf[4 * i + 2] << 16) | ((uint16_t)pbuf[4 * i + 1] << 8) | pbuf[4 * i];
        /*将pbuf内的数据写入Flash*/
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, buf_32) == HAL_OK)
        {
            /*指向下一个字 1 word == 4 byte */
            addr += 4;
        }
        else
        {
            printf("Write Flash fail at 0x%x\r\n",addr);
            Error_Handler();
        }
    }
}