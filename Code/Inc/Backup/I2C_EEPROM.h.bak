/*
 * I2C_EEPROM.h
 *
 *  Created on: Apr 4, 2022
 *      Author: cpholzn
 */

#ifndef I2C_EEPROM_H_
#define I2C_EEPROM_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#if defined(STM32F407xx)
#include "stm32f4xx.h"
#else
#include "stm32f10x.h"
#error "I2C EEPROM library only supported STM32F4"
#endif





typedef enum
{
    AT24C01=0,
    AT24C02,
    AT24C04,
    AT24C08,
    AT24C16,
    AT24C32,
    AT24C64
} AT24C_Cap_t;


extern const int page_sizes[];
extern const int n_pages[];


typedef struct
{
    AT24C_Cap_t capacity;
    GPIO_TypeDef* port_SCL;
    int pin_SCL;
    GPIO_TypeDef* port_SDA;
    int pin_SDA;
    GPIO_TypeDef* port_WP;
    int pin_WP;
    unsigned char addr_chip;
    // write address cursour
    intptr_t addrCur;
} EEPROM_AT24_t;

extern EEPROM_AT24_t EEPROM;

void I2C_EEPROM_init(EEPROM_AT24_t *pEEPROM, AT24C_Cap_t capacity,
		GPIO_TypeDef* port_SCL,	uint16_t pin_SCL,
		GPIO_TypeDef* port_SDA, uint16_t pin_SDA,
		GPIO_TypeDef* port_WP, 	uint16_t pin_WP,
		uint8_t addr_chip);

size_t I2C_EEPROM_WriteBytes(EEPROM_AT24_t* pEEPROM, uint8_t *Buffer, size_t Length);

size_t I2C_EEPROM_ReadBytes(EEPROM_AT24_t* pEEPROM, uint8_t *Buffer, size_t Length);





#endif /* I2C_EEPROM_H_ */
