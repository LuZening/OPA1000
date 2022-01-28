
/*
 Softwer Uart For Stm32
 By Liyanboy74
 https://github.com/liyanboy74
*/

#include "SoftUART.h"

extern TIM_HandleTypeDef htim4;

SoftUart_S 						SUart				[Number_Of_SoftUarts];
SoftUartBuffer_S 			SUBuffer		[Number_Of_SoftUarts];

__IO  uint8_t 				SU_Timer=0;
uint8_t 							SU_DBaffer;

const uint16_t BaudRates[] = {1200, 2400, 4800, 9600, 19200};

GPIO_PinState SoftUartGpioReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	return HAL_GPIO_ReadPin(GPIOx,GPIO_Pin);
}

void SoftUartGpioWritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,PinState);
}

SoftUartState_E SoftUartInit(uint8_t SoftUartNumber,GPIO_TypeDef *TxPort,uint16_t TxPin,GPIO_TypeDef *RxPort,uint16_t RxPin, void (*cbFuncRecvByte)(uint8_t))
{
	if(SoftUartNumber>=Number_Of_SoftUarts)return SoftUart_Error;

	SUart[SoftUartNumber].TxNComplated=0;

	SUart[SoftUartNumber].RxBitConter=0;
	SUart[SoftUartNumber].RxBitShift=0;
	SUart[SoftUartNumber].RxIndex=0;

	SUart[SoftUartNumber].TxEnable=0;
	SUart[SoftUartNumber].RxEnable=0;

	SUart[SoftUartNumber].TxBitConter=0;
	SUart[SoftUartNumber].TxBitShift=0;
	SUart[SoftUartNumber].TxIndex=0;

	SUart[SoftUartNumber].TxSize=0;

	SUart[SoftUartNumber].Buffer=&SUBuffer[SoftUartNumber];

	SUart[SoftUartNumber].RxPort=RxPort;
	SUart[SoftUartNumber].RxPin=RxPin;
	SoftUartGpioWritePin(RxPort, RxPin, GPIO_PIN_SET);

	SUart[SoftUartNumber].TxPort=TxPort;
	SUart[SoftUartNumber].TxPin=TxPin;

	SUart[SoftUartNumber].RxTimingFlag=0;
	SUart[SoftUartNumber].RxBitOffset=0;

	SUart[SoftUartNumber].cbFuncByteRecved = cbFuncRecvByte;
	return SoftUart_OK;
}

void SoftUartTransmitBit(SoftUart_S *SU,uint8_t Bit0_1)
{
	SoftUartGpioWritePin(SU->TxPort,SU->TxPin,(GPIO_PinState)Bit0_1);
}

SoftUartState_E SoftUartEnableRx(uint8_t SoftUartNumber)
{
	if(SoftUartNumber>=Number_Of_SoftUarts)return SoftUart_Error;
	SUart[SoftUartNumber].RxEnable=1;
	return SoftUart_OK;
}

SoftUartState_E SoftUartDisableRx(uint8_t SoftUartNumber)
{
	if(SoftUartNumber>=Number_Of_SoftUarts)return SoftUart_Error;
	SUart[SoftUartNumber].RxEnable=0;
	return SoftUart_OK;
}

uint8_t SoftUartRxAlavailable(uint8_t SoftUartNumber)
{
	return SUart[SoftUartNumber].RxIndex;
}

uint8_t SoftUartReadRxBuffer(uint8_t SoftUartNumber,uint8_t *Buffer,uint8_t Len)
{
	int i;
	SoftUartStop();
	if(SoftUartNumber>=Number_Of_SoftUarts)return SoftUart_Error;
	if(Len > SUart[SoftUartNumber].RxIndex)
		Len = SUart[SoftUartNumber].RxIndex;

	for(i=0;i<Len;i++)
	{
		Buffer[i]=SUart[SoftUartNumber].Buffer->Rx[i];
	}
	for(i=Len; i < SUart[SoftUartNumber].RxIndex; ++i)
		SUart[SoftUartNumber].Buffer->Rx[i-Len] = SUart[SoftUartNumber].Buffer->Rx[i];
	SUart[SoftUartNumber].RxIndex -= Len;
	SoftUartStart();
	return Len;
}


void SoftUartTxProcess(SoftUart_S *SU)
{
	if(SU->TxEnable)
	{
		if(SU->TxBitConter==0)
		{
			SU->TxNComplated=1;
			SU->TxBitShift=0;
			SoftUartTransmitBit(SU,0);
			SU->TxBitConter++;
		}
		else if(SU->TxBitConter<9)
		{
			SoftUartTransmitBit(SU,((SU->Buffer->Tx[SU->TxIndex])>>(SU->TxBitShift))&0x01);
			SU->TxBitConter++;
			SU->TxBitShift++;
		}
		else if(SU->TxBitConter==9)
		{
			SoftUartTransmitBit(SU,1);
			SU->TxBitConter++;
		}
		else if(SU->TxBitConter==10)
		{
			//Complate
			SU->TxBitConter=0;

			SU->TxIndex++;
			if(SU->TxSize > SU->TxIndex)
			{
				SU->TxNComplated=1;
				SU->TxEnable=1;
			}
			else
			{
				SU->TxNComplated=0;
				SU->TxEnable=0;
			}
		}
	}
}

void SoftUartRxDataBitProcess(SoftUart_S *SU,uint8_t B0_1)
{
	if(SU->RxEnable)
	{
		if(SU->RxBitConter==0)//Start
		{
			if(B0_1)return;
			SU->RxBitShift=0;
			SU->RxBitConter++;
			SU->Buffer->Rx[SU->RxIndex]=0;
		}
		else if(SU->RxBitConter<9)//Data
		{
			SU->Buffer->Rx[SU->RxIndex]|=((B0_1&0x01)<<SU->RxBitShift);
			SU->RxBitConter++;
			SU->RxBitShift++;
		}
		else if(SU->RxBitConter==9)
		{
			SU->RxBitConter=0;
			SU->RxTimingFlag=0;
			if(B0_1)//Stop Bit
			{
				// Invoke One byte received callback function
				uint8_t data = SU->Buffer->Rx[SU->RxIndex];
				if(SU->cbFuncByteRecved)
					SU->cbFuncByteRecved(data);
				//OK
				if((SU->RxIndex)<(SoftUartRxBufferSize-1))(SU->RxIndex)++;
			}
		}
	}
}

void SoftUartWaitUntilTxComplate(uint8_t SoftUartNumber)
{
	while(SUart[SoftUartNumber].TxNComplated);
}

SoftUartState_E SoftUartPuts(uint8_t SoftUartNumber,uint8_t *Str,uint8_t Len)
{
	int i;
	if(SoftUartNumber>=Number_Of_SoftUarts)return SoftUart_Error;
	if(SUart[SoftUartNumber].TxNComplated) return SoftUart_Error;

	SUart[SoftUartNumber].TxIndex=0;
	SUart[SoftUartNumber].TxSize=Len;

	for(i=0;i<Len;i++)
	{
		SUart[SoftUartNumber].Buffer->Tx[i]= Str[i];
	}

	SUart[SoftUartNumber].TxNComplated=1;
	SUart[SoftUartNumber].TxEnable=1;

	return SoftUart_OK;
}

uint8_t SoftUartScanRxPorts(void)
{
	int i;
	uint8_t Bit;
	SoftUart_S* S = SUart;
	Bit=SoftUartGpioReadPin(S->RxPort,S->RxPin);
	if(!S->RxBitConter && !S->RxTimingFlag && !Bit)
	{
		S->RxBitOffset=((SU_Timer+2)%5);
		S->RxTimingFlag=1;
	}
	return Bit;
}


void SoftUartHandler(void)
{

	//RX
	SU_DBaffer=SoftUartScanRxPorts();
	SoftUart_S* S = SUart;

	if(S->RxBitOffset==SU_Timer)
	{
		SoftUartRxDataBitProcess(S,SU_DBaffer);
	}

	if(SU_Timer==0)
	{
		//TX
		SoftUartTxProcess(S);
	}

	SU_Timer++;
	if(SU_Timer>=5)SU_Timer=0;
}

int8_t baudrate2idx(uint16_t baudrate)
{
	for(int8_t i = 0; i < sizeof(BaudRates) / sizeof(uint16_t); ++i)
	{
		if(BaudRates[i] == baudrate)
			return i;
	}
	return -1;
}

void SoftUartChangeBaud(uint16_t baud)
{
	if(baud >= 19200)
		baud = 19200; // limit max baud rate
	uint32_t newARRValue = BAUD_TO_TIM_ARR_VALUE(baud);
	SoftUartStop();
	__HAL_TIM_SET_AUTORELOAD(&htim4,  newARRValue);
	SoftUartStart();
}
void SoftUartStop()
{
	HAL_TIM_Base_Stop_IT(&htim4);
}
void SoftUartStart()
{
	HAL_TIM_Base_Start_IT(&htim4);
}
