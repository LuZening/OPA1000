/*
 * R61408.h
 *
 *  Created on: Mar 11, 2020
 *      Author: Zening
 */


#ifndef R61408_H_
#define R61408_H_

#include "main.h"
#include "lvgl.h"


#define LCD_RS		18	// A18
#define BANK1_LCD_CMD    ((uint32_t)0x60000000)	   //Disp Reg ADDR
#define BANK1_LCD_DATA    (BANK1_LCD_CMD | ((uint32_t) 1<<(LCD_RS+1)))    //Disp Data ADDR


#define LCD_Back_Light_ON   LCD_BL_GPIO_Port->BSRR = LCD_BL_Pin
#define LCD_Back_Light_OFF  LCD_BL_GPIO_Port->BRR  = LCD_BL_Pin


#define Set_LCD_CS  LCD_CS_GPIO_Port->BSRR = (LCD_CS_Pin)
#define Clr_LCD_CS  LCD_CS_GPIO_Port->BSRR = (LCD_CS_Pin << 16)

#define Set_LCD_RS	LCD_RS_GPIO_Port->BSRR = (LCD_RS_Pin)
#define Clr_LCD_RS  LCD_RS_GPIO_Port->BSRR = (LCD_RS_Pin << 16)

#define LCD_Send_Command(cmd) (*(volatile uint16_t*)BANK1_LCD_CMD = ((uint16_t)cmd))
#define LCD_Send_Data(d) (*(volatile uint16_t*)BANK1_LCD_DATA = ((uint16_t)d))

#define LCD_SET_ADDRESS_MODE 0x36
#define LCD_SET_DISPLAY_ON 0x29
#define LCD_SET_COLUMN_ADDRESS 0x2A
#define LCD_SET_PAGE_ADDRESS 0x2B
#define LCD_WRITE_MEMORY_START 0x2C

void LCD_Init();

int LCD_test();

void LCD_Set_Write_Range(uint16_t x_begin, uint16_t y_begin, uint16_t x_end, uint16_t y_end);

void LCD_FillRect_2P(uint16_t x_begin, uint16_t y_begin, uint16_t x_end, uint16_t y_end, uint16_t background);
void LCD_PutPixels(uint16_t x_begin, uint16_t y_begin, uint16_t width, uint16_t height, const uint16_t* pixels);

// LVGL flush pixel interface
void LCD_LVGL_flush(lv_disp_drv_t* disp_drv, const lv_area_t * area, lv_color_t * color_p);


#endif /* R61408_H_ */
