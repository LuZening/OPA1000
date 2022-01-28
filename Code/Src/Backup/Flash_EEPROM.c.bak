/*
 * Flash_EEPROM.c
 *
 *  Created on: 2019年9月28日
 *      Author: Zening
 */


#include "Flash_EEPROM.h"
#if defined(STM32F4)
EEPROM_typedef EEPROM = {.base_addr=(uint8_t*)(FLASH_LAST_SECTOR_BASE_ADDR), .size=FLASH_LAST_SECTOR_SIZE};
#else
EEPROM_typedef EEPROM = {.base_addr=(uint8_t*)EEPROM_BASE_ADDR, .size=0x400};
#endif

typedef union
{
	uint8_t bytes[4];
	uint32_t word;
} Quadbyte;

// Notice: 4-byte alignment
void EEPROM_WriteBytes(EEPROM_typedef *pEEPROM,uint8_t *Buffer,uint16_t Length)
{
	uint16_t i;
	uint32_t* addr = (uint32_t*)pEEPROM->base_addr;
	Quadbyte quadbyte;
#if defined(STM32F4)
	FLASH_EraseInitTypeDef fe = {.TypeErase=FLASH_TYPEERASE_SECTORS,
			.Sector = FLASH_SECTOR_11,
			.NbSectors=1,
			.VoltageRange=FLASH_VOLTAGE_RANGE_3};
#else
	FLASH_EraseInitTypeDef fe = {.TypeErase=FLASH_TYPEERASE_PAGES, .PageAddress=(uint32_t)addr, .NbPages=1};
#endif
	uint32_t PageError = 0;
    __disable_irq();
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
            FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    HAL_FLASHEx_Erase(&fe, &PageError);
    while(Length)
    {
    	memset(quadbyte.bytes, 0, 4);
    	for(i=0; i<4 && Length; ++i)
    	{
    		quadbyte.bytes[i] = *Buffer;
    		Length--;
    		Buffer++;
    	}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)addr++, quadbyte.word);
    }
    HAL_FLASH_Lock();
    __enable_irq();
}

void EEPROM_ReadBytes(EEPROM_typedef* pEEPROM, uint8_t *Buffer,uint16_t Length)
{
	uint8_t* addr = pEEPROM->base_addr;
	while(Length)
	{
		*Buffer = *addr;
		Buffer++;
		addr++;
		Length--;
	}
}
