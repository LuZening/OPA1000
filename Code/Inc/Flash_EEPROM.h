/*
 * Flash_EEPROM.h
 *
 *  Created on: 2019年9月28日
 *      Author: Zening
 */

#ifndef FLASH_EEPROM_H_
#define FLASH_EEPROM_H_

#include "main.h"
#include "string.h"

#define EEPROM_VALID_BYTE 0xAA
#if defined(STM32F4)
#define FLASH_LAST_SECTOR_BASE_ADDR 0x080E0000
#define FLASH_LAST_SECTOR_SIZE (128U * 1024U) // BYTES
#else
#define EEPROM_BASE_ADDR 0x0800BC00 // 1KB before FS
#endif

typedef struct
{
	uint8_t* base_addr; // 4-bit alignment!!!!!!! base_addr must be divisible by 4
	uint32_t size;
} EEPROM_typedef;
extern EEPROM_typedef FlashEEPROM;
/*------------------------------------------------------------
 Func: EEPROM数据按字节写入
 Note:
-------------------------------------------------------------*/
void Flash_EEPROM_WriteBytes(EEPROM_typedef* pEEPROM, uint8_t *Buffer,uint16_t Length);
void Flash_EEPROM_ReadBytes(EEPROM_typedef* pEEPROM, uint8_t *Buffer,uint16_t Length);

#endif /* FLASH_EEPROM_H_ */
