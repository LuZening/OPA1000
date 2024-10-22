/*
 * R61408.C
 *
 *  Created on: Mar 11, 2020
 *      Author: Zening
 */

#include "R61408.h"
void LCD_Init()
{
	LCD_Back_Light_ON;
	// Reset LCD
	HAL_GPIO_WritePin(LCD_RESX_GPIO_Port, LCD_RESX_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(LCD_RESX_GPIO_Port, LCD_RESX_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
	LCD_Send_Command(0x11);
	LCD_Send_Data(0x00);
	HAL_Delay(40);

	LCD_Send_Command(0x29);
	LCD_Send_Data(0x00);

	LCD_Send_Command(0xB0);
	LCD_Send_Data(0x04);
	LCD_Send_Command(0xB3);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xB6);
	LCD_Send_Data(0x52);
	LCD_Send_Data(0x83);
	LCD_Send_Command(0xB7);
	LCD_Send_Data(0x80);
	LCD_Send_Data(0x72);
	LCD_Send_Data(0x11);
	LCD_Send_Data(0x25);
	LCD_Send_Command(0xB8);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x0F);
	LCD_Send_Data(0x0F);
	LCD_Send_Data(0xFF);
	LCD_Send_Data(0xFF);
	LCD_Send_Data(0xC8);
	LCD_Send_Data(0xC8);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x10);
	LCD_Send_Data(0x10);
	LCD_Send_Data(0x37);
	LCD_Send_Data(0x5A);
	LCD_Send_Data(0x87);
	LCD_Send_Data(0xBE);
	LCD_Send_Data(0xFF);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xB9);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xBD);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xC0);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x76);
	LCD_Send_Command(0xC1);
	LCD_Send_Data(0x63);
	LCD_Send_Data(0x31);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x27);
	LCD_Send_Data(0x27);
	LCD_Send_Data(0x32);
	LCD_Send_Data(0x12);
	LCD_Send_Data(0x28);
	LCD_Send_Data(0x4E);
	LCD_Send_Data(0x10);
	LCD_Send_Data(0xA5);
	LCD_Send_Data(0x0F);
	LCD_Send_Data(0x58);
	LCD_Send_Data(0x21);
	LCD_Send_Data(0x01);
	LCD_Send_Command(0xC2);
	LCD_Send_Data(0x28);
	LCD_Send_Data(0x06);
	LCD_Send_Data(0x06);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0x03);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xC3);
	LCD_Send_Data(0x40);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x03);
	LCD_Send_Command(0xC4);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x01);
	LCD_Send_Command(0xC6);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xC7);
	LCD_Send_Data(0x11);
	LCD_Send_Data(0x8D);
	LCD_Send_Data(0xA0);
	LCD_Send_Data(0xF5);
	LCD_Send_Data(0x27);
	LCD_Send_Command(0xC8);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x13);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x25);
	LCD_Send_Data(0x34);
	LCD_Send_Data(0x4E);
	LCD_Send_Data(0x36);
	LCD_Send_Data(0x23);
	LCD_Send_Data(0x17);
	LCD_Send_Data(0x0E);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x13);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x25);
	LCD_Send_Data(0x34);
	LCD_Send_Data(0x4E);
	LCD_Send_Data(0x36);
	LCD_Send_Data(0x23);
	LCD_Send_Data(0x17);
	LCD_Send_Data(0x0E);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x02);
	LCD_Send_Command(0xC9);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x13);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x25);
	LCD_Send_Data(0x34);
	LCD_Send_Data(0x4E);
	LCD_Send_Data(0x36);
	LCD_Send_Data(0x23);
	LCD_Send_Data(0x17);
	LCD_Send_Data(0x0E);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x13);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x25);
	LCD_Send_Data(0x34);
	LCD_Send_Data(0x4E);
	LCD_Send_Data(0x36);
	LCD_Send_Data(0x23);
	LCD_Send_Data(0x17);
	LCD_Send_Data(0x0E);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x02);
	LCD_Send_Command(0xCA);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x13);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x25);
	LCD_Send_Data(0x34);
	LCD_Send_Data(0x4E);
	LCD_Send_Data(0x36);
	LCD_Send_Data(0x23);
	LCD_Send_Data(0x17);
	LCD_Send_Data(0x0E);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x13);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x25);
	LCD_Send_Data(0x34);
	LCD_Send_Data(0x4E);
	LCD_Send_Data(0x36);
	LCD_Send_Data(0x23);
	LCD_Send_Data(0x17);
	LCD_Send_Data(0x0E);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x02);
	LCD_Send_Command(0xD0);
	LCD_Send_Data(0xA9);
	LCD_Send_Data(0x03);
	LCD_Send_Data(0xCC);
	LCD_Send_Data(0xA5);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x53);
	LCD_Send_Data(0x20);
	LCD_Send_Data(0x10);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x03);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xD1);
	LCD_Send_Data(0x18);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x23);
	LCD_Send_Data(0x03);
	LCD_Send_Data(0x75);
	LCD_Send_Data(0x02);
	LCD_Send_Data(0x50);
	LCD_Send_Command(0xD3);
	LCD_Send_Data(0x33);
	LCD_Send_Command(0xD5);
	LCD_Send_Data(0x2a);
	LCD_Send_Data(0x2a);
	LCD_Send_Command(0xD6);
	LCD_Send_Data(0x28);//a8
	LCD_Send_Command(0xD7);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0xAA);
	LCD_Send_Data(0xC0);
	LCD_Send_Data(0x2A);
	LCD_Send_Data(0x2C);
	LCD_Send_Data(0x22);
	LCD_Send_Data(0x12);
	LCD_Send_Data(0x71);
	LCD_Send_Data(0x0A);
	LCD_Send_Data(0x12);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0xA0);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x03);
	LCD_Send_Command(0xD8);
	LCD_Send_Data(0x44);
	LCD_Send_Data(0x44);
	LCD_Send_Data(0x22);
	LCD_Send_Data(0x44);
	LCD_Send_Data(0x21);
	LCD_Send_Data(0x46);
	LCD_Send_Data(0x42);
	LCD_Send_Data(0x40);
	LCD_Send_Command(0xD9);
	LCD_Send_Data(0xCF);
	LCD_Send_Data(0x2D);
	LCD_Send_Data(0x51);
	LCD_Send_Command(0xDA);
	LCD_Send_Data(0x01);
	LCD_Send_Command(0xDE);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0x51);//58
	LCD_Send_Command(0xE1);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xE6);
	LCD_Send_Data(0x55);//58
	LCD_Send_Command(0xF3);
	LCD_Send_Data(0x06);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x24);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xF8);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xFA);
	LCD_Send_Data(0x01);
	LCD_Send_Command(0xFB);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xFC);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xFD);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x70);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x72);
	LCD_Send_Data(0x31);
	LCD_Send_Data(0x37);
	LCD_Send_Data(0x70);
	LCD_Send_Data(0x32);
	LCD_Send_Data(0x31);
	LCD_Send_Data(0x07);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0xFE);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x20);
	LCD_Send_Command(0xB0);
	LCD_Send_Data(0x04); //04
	HAL_Delay(40);
	LCD_Send_Command(0x35);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0x44);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0x36);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0x3A);
	LCD_Send_Data(0x55);
	LCD_Send_Command(0x2A);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x01);
	LCD_Send_Data(0xDF);
	LCD_Send_Command(0x2B);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x03);
	LCD_Send_Data(0x1F);
	LCD_Send_Command(0x29);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0x28);
	LCD_Send_Data(0x00);
	LCD_Send_Command(0x29);
	LCD_Send_Data(0x00);
	HAL_Delay(180);
//	LCD_Send_Command(0x2C);
//	LCD_Send_Data(0x00);
//	HAL_Delay(10);
//
	LCD_Send_Command(0x36);
	LCD_Send_Data(0x60);

	// flush the screen
//	LCD_FillRect_2P(0, 0, 800, 480, 0xffff);
//	LCD_test();
	LCD_FillRect_2P(0, 0, 800, 480, 0xffff);
}

int LCD_test()
{
	int x = 0, y = 200;
	int r = 40;
	int n = 0;
	for(int i = 0; i < 50; i++)
	{
		for(x = 0; x < 800-40; x+=10)
		{
			LCD_FillRect_2P(x, y, x+r, y+r, 0xff00);
			LCD_FillRect_2P(x, y, x+r, y+r, 0);
			n++;
		}
	}
	return n;
}

void LCD_Set_Write_Range(uint16_t x_begin, uint16_t y_begin, uint16_t x_end, uint16_t y_end)
{
	// set columns address
	LCD_Send_Command(LCD_SET_COLUMN_ADDRESS);
	LCD_Send_Data((x_begin >> 8) & 0x3);
	LCD_Send_Data(x_begin & 0xff);
	LCD_Send_Data((x_end >> 8) & 0x3);
	LCD_Send_Data(x_end & 0xff);
	// set page address
	LCD_Send_Command(LCD_SET_PAGE_ADDRESS);
	LCD_Send_Data((y_begin >> 8) & 0x3);
	LCD_Send_Data(y_begin & 0xff);
	LCD_Send_Data((y_end >> 8) & 0x3);
	LCD_Send_Data(y_end & 0xff);
}

void LCD_FillRect_2P(uint16_t x_begin, uint16_t y_begin, uint16_t x_end, uint16_t y_end, uint16_t background)
{
	LCD_Set_Write_Range(x_begin, y_begin, x_end, y_end);
	uint32_t counter = (uint32_t)(x_end - x_begin) * (uint32_t)(y_end - y_begin);
	LCD_Send_Command(LCD_WRITE_MEMORY_START);
	while(counter--)
		LCD_Send_Data(background);
}

void LCD_PutPixels(uint16_t x_begin, uint16_t y_begin, uint16_t width, uint16_t height, const uint16_t* pixels)
{
	uint16_t x_end = x_begin + width-1;
	uint16_t y_end = y_begin + height-1;
	LCD_Set_Write_Range(x_begin, y_begin, x_end, y_end);
	uint32_t counter = (uint32_t)width * (uint32_t)height;
	LCD_Send_Command(LCD_WRITE_MEMORY_START);
	while(counter--)
		LCD_Send_Data(*(pixels++));
}

void LCD_LVGL_flush(lv_disp_drv_t* disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	int16_t width, height, x1, y1, x2, y2;
	x1 = area->x1; x2 = area->x2;
	y1 = area->y1; y2 = area->y2;
	width = x2 - x1+1;
	height = y2 - y1+1;
	if(width < 0)
	{
		width = -width;
		x1 = area->x2;
		x2 = area->x1;
	}
	if(height < 0)
	{
		height = -height;
		y1 = area->y2;
		y2 = area->y1;
	}
	LCD_PutPixels(x1, y1, width, height, (uint16_t*)color_p);
	lv_disp_flush_ready(disp_drv);         /* Indicate you are ready with the flushing*/
}


