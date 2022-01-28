/*
 * COM.h
 *
 *  Created on: Mar 13, 2020
 *      Author: Zening
 */

#ifndef COM_H_
#define COM_H_

#include "stm32f407xx.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#define COM_ENABLE_TXE(huart) (huart.Instance->CR1 |= USART_CR1_TXEIE)
#define COM_DISABLE_TXE(huart) (huart.Instance->CR1 &= ~(USART_CR1_TXEIE))
typedef struct {
	uint8_t argc;
	uint8_t* argv[4]; // argv[0] = command, argv[1], argv[2], argv[3]
} Command_t;

extern osMessageQueueId_t COMBytesToSendQHandle;
extern osMessageQueueId_t COMBytesRecvedQHandle;
void COM_send_message(const char* s);

void StartCOMRecvTask();

extern band_t BAND_decoder;

#endif /* COM_H_ */
