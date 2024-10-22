/*
 * COM.h
 *
 *  Created on: Mar 13, 2020
 *      Author: Zening
 */

#ifndef COM_H_
#define COM_H_

#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "kfifo.h"


#define COM_ENABLE_TXE(huart) ((huart).Instance->CR1 |= USART_CR1_TXEIE)
#define COM_DISABLE_TXE(huart) ((huart).Instance->CR1 &= ~(USART_CR1_TXEIE))

#define min(x,y) ({ \
    typeof(x) _x = (x);    \
    typeof(y) _y = (y);    \
    (void) (&_x == &_y);    \
    _x < _y ? _x : _y; })



typedef struct {
	KFIFO FIFO_TX;
	KFIFO FIFO_RX;
	UART_HandleTypeDef* phuart;
} COM_t;

extern COM_t COM1;

#define COM_BUFFER_LEN 128

// lenbuf must be round power of 2
void COM_init(COM_t *p, UART_HandleTypeDef *phuart, uint8_t* bufTX, size_t lenbufTX, uint8_t* bufRX, size_t lenbufRX);

void COM_IRQHandler(COM_t*p);

size_t COM_send_message(COM_t* p ,  const uint8_t* s, size_t n);

size_t COM_read_message(COM_t* p, uint8_t* buf, size_t n);




#endif /* COM_H_ */
