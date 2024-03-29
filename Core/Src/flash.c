/*
 * @Author: Tomood
 * @Date: 2024-02-18 22:50:38
 * @LastEditTime: 2024-02-22 02:47:47
 * @FilePath: \MDK-ARMd:\STM32Prj\IAP_Bootloader_demo\Core\Src\flash.c
 * @Description: L051的flash一共 512页，每页128bytes
 * 
 * Copyright (c) 2024 by Tomood, All Rights Reserved. 
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "flash.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @description: Unlocks Flash for write access
 * @param  start_addr: start of user flash area
 * @return {*}
 */
void FLASH_Init(void)
{
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR |
                         FLASH_FLAG_OPTVERR | FLASH_FLAG_RDERR | FLASH_FLAG_FWWERR |
                         FLASH_FLAG_NOTZEROERR);/* Clear all FLASH flags */
  HAL_FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  start_addr: start of user flash area
  * @param  NbPages:Number of pages to be erased
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
void FLASH_Erase_Page(uint32_t start_addr,uint32_t NbPages)
{
  uint32_t PageError;
 //Flash 写入之前必须先进行页擦除
 FLASH_EraseInitTypeDef EraseInitStruct = {
    .TypeErase = FLASH_TYPEERASE_PAGES,       //页擦除(只支持页擦除)
    .PageAddress = start_addr,                //擦除起始地址
    .NbPages = NbPages                        //擦除页数
  };
  HAL_FLASH_Unlock();
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    //擦除错误
    printf("FLASH Erase Fail\r\n");
    printf("Fail Code:%d\r\n",HAL_FLASH_GetError());
    printf("Fail Page:%d\r\n",PageError);
  }
  HAL_FLASH_Lock();
}	


/**
 * @description: 读取指定地址的全字(32位数据)
 * @param {uint32_t} address
 * @return {*}
 */
uint32_t FLASH_ReadWord(uint32_t address)
{
  return *(__IO uint32_t*)address;
}

/**
 * @description: Flash写入函数
 * @param {uint32_t} Address
 * @param {uint32_t} Data
 * @return {*}
 */
void FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
	HAL_FLASH_Unlock();//先解锁才能写
  if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data) == HAL_OK)
  {
    printf("write data 0x%x OK\n", Data);
  }
  else
  {
    printf("failed!!!\n");
  }
	HAL_FLASH_Lock();
} 

