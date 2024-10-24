/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FS.h"
#include "cmsis_os.h"
#include "Sensor_task.h"
#include "RadioTypes.h"
#include "persistent.h"
#include "fan.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern  osMutexId_t mtxBandDataHandle;
extern osMessageQueueId_t qAlerts;
extern osEventFlagsId_t SensorDataReadyEvents_handle;
//extern osSemaphoreId_t GUIDataUpdatedSemaphore_handle;
extern osMessageQueueId_t COMBytesToSendQHandle;
extern osMessageQueueId_t COMBytesRecvedQHandle;
extern osThreadId_t defaultTaskHandle;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define OS_FLAG_POWEROFF 0x02

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BAND_SEL6X_Pin GPIO_PIN_2
#define BAND_SEL6X_GPIO_Port GPIOE
#define BAND_SEL5X_Pin GPIO_PIN_3
#define BAND_SEL5X_GPIO_Port GPIOE
#define BAND_SEL4X_Pin GPIO_PIN_4
#define BAND_SEL4X_GPIO_Port GPIOE
#define BAND_SEL3X_Pin GPIO_PIN_5
#define BAND_SEL3X_GPIO_Port GPIOE
#define BAND_SEL2X_Pin GPIO_PIN_6
#define BAND_SEL2X_GPIO_Port GPIOE
#define BAND_SEL1X_Pin GPIO_PIN_13
#define BAND_SEL1X_GPIO_Port GPIOC
#define PrtctSig_Pin GPIO_PIN_15
#define PrtctSig_GPIO_Port GPIOC
#define MCU_STATE_LEDX_Pin GPIO_PIN_0
#define MCU_STATE_LEDX_GPIO_Port GPIOA
#define EEP_SDA_Pin GPIO_PIN_1
#define EEP_SDA_GPIO_Port GPIOA
#define EEP_SCL_Pin GPIO_PIN_2
#define EEP_SCL_GPIO_Port GPIOA
#define EEP_WP_Pin GPIO_PIN_5
#define EEP_WP_GPIO_Port GPIOC
#define RF_12V_ON_Pin GPIO_PIN_2
#define RF_12V_ON_GPIO_Port GPIOB
#define BAND_TX_Pin GPIO_PIN_10
#define BAND_TX_GPIO_Port GPIOB
#define BAND_RX_Pin GPIO_PIN_11
#define BAND_RX_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_14
#define LCD_BL_GPIO_Port GPIOB
#define T_INTX_Pin GPIO_PIN_15
#define T_INTX_GPIO_Port GPIOB
#define T_INTX_EXTI_IRQn EXTI15_10_IRQn
#define ENC_AX_Pin GPIO_PIN_11
#define ENC_AX_GPIO_Port GPIOD
#define ENC_BX_Pin GPIO_PIN_12
#define ENC_BX_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_13
#define LCD_RS_GPIO_Port GPIOD
#define LCD_RESX_Pin GPIO_PIN_6
#define LCD_RESX_GPIO_Port GPIOC
#define ENC_SWX_Pin GPIO_PIN_7
#define ENC_SWX_GPIO_Port GPIOC
#define SpareSigX_Pin GPIO_PIN_8
#define SpareSigX_GPIO_Port GPIOC
#define PTTX_Pin GPIO_PIN_2
#define PTTX_GPIO_Port GPIOD
#define PTTX_EXTI_IRQn EXTI2_IRQn
#define POT_SCL_Pin GPIO_PIN_8
#define POT_SCL_GPIO_Port GPIOA
#define POT_SDA_Pin GPIO_PIN_11
#define POT_SDA_GPIO_Port GPIOA
#define Transmit_Pin GPIO_PIN_12
#define Transmit_GPIO_Port GPIOA
#define TEMP2_HIGH_Pin GPIO_PIN_15
#define TEMP2_HIGH_GPIO_Port GPIOA
#define TEMP1_HIGH_Pin GPIO_PIN_10
#define TEMP1_HIGH_GPIO_Port GPIOC
#define IMAIN_HIGHX_Pin GPIO_PIN_11
#define IMAIN_HIGHX_GPIO_Port GPIOC
#define VMAIN_HIGH_Pin GPIO_PIN_12
#define VMAIN_HIGH_GPIO_Port GPIOC
#define FAN1_Pin GPIO_PIN_9
#define FAN1_GPIO_Port GPIOC
#define LCD_RD_Pin GPIO_PIN_4
#define LCD_RD_GPIO_Port GPIOD
#define LCD_WR_Pin GPIO_PIN_5
#define LCD_WR_GPIO_Port GPIOD
#define PTT_SUPPRESSX_Pin GPIO_PIN_6
#define PTT_SUPPRESSX_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_7
#define LCD_CS_GPIO_Port GPIOD
#define REV2_HIGH_Pin GPIO_PIN_6
#define REV2_HIGH_GPIO_Port GPIOB
#define REV1_HIGHX_Pin GPIO_PIN_7
#define REV1_HIGHX_GPIO_Port GPIOB
#define T_BUSY_Pin GPIO_PIN_8
#define T_BUSY_GPIO_Port GPIOB
#define T_CSX_Pin GPIO_PIN_9
#define T_CSX_GPIO_Port GPIOB
#define PWR_on_sig_Pin GPIO_PIN_0
#define PWR_on_sig_GPIO_Port GPIOE
#define OverdriveX_Pin GPIO_PIN_1
#define OverdriveX_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

#if defined(OPA1000)
/*commented by "//" means exists in OPA2000 only */
#define AUTO_BAND_Pin GPIO_PIN_14
#define AUTO_BAND_Port GPIOC
//#define PrtctSig_Pin GPIO_PIN_15
//#define PrtctSig_GPIO_Port GPIOC
#define MCU_STATE_LEDX_Pin GPIO_PIN_0
#define MCU_STATE_LEDX_GPIO_Port GPIOA
//#define EEP_SDA_Pin GPIO_PIN_1
//#define EEP_SDA_GPIO_Port GPIOA
//#define EEP_SCL_Pin GPIO_PIN_2
//#define EEP_SCL_GPIO_Port GPIOA
//#define EEP_WP_Pin GPIO_PIN_5
//#define EEP_WP_GPIO_Port GPIOC
#define RF_12V_ON_Pin GPIO_PIN_2
#define RF_12V_ON_GPIO_Port GPIOB
//#define BAND_TX_Pin GPIO_PIN_10
//#define BAND_TX_GPIO_Port GPIOB
#define BAND_RX_Pin GPIO_PIN_1
#define BAND_RX_GPIO_Port GPIOA
#define LCD_BL_Pin GPIO_PIN_10
#define LCD_BL_GPIO_Port GPIOB
#define T_INTX_Pin GPIO_PIN_11
#define T_INTX_GPIO_Port GPIOB
#define T_INTX_EXTI_IRQn EXTI15_10_IRQn
#define FAN1_Pin GPIO_PIN_2
#define FAN1_GPIO_Port GPIOD
#define FAN2_Pin GPIO_PIN_3
#define FAN2_GPIO_Port GPIOD
//#define ENC_AX_Pin GPIO_PIN_11
//#define ENC_AX_GPIO_Port GPIOD
//#define ENC_BX_Pin GPIO_PIN_12
//#define ENC_BX_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_13
#define LCD_RS_GPIO_Port GPIOD
#define LCD_RESX_Pin GPIO_PIN_6
#define LCD_RESX_GPIO_Port GPIOC
//#define ENC_SWX_Pin GPIO_PIN_7
//#define ENC_SWX_GPIO_Port GPIOC
//#define SpareSigX_Pin GPIO_PIN_8
//#define SpareSigX_GPIO_Port GPIOC
//#define PTTX_Pin GPIO_PIN_9
//#define PTTX_GPIO_Port GPIOC
//#define PTTX_EXTI_IRQn EXTI9_5_IRQn
//#define POT_SCL_Pin GPIO_PIN_8
//#define POT_SCL_GPIO_Port GPIOA
//#define POT_SDA_Pin GPIO_PIN_11
//#define POT_SDA_GPIO_Port GPIOA
/* the same allocation as OPA2000, but as input on OPA1000
#define Transmit_Pin GPIO_PIN_12
#define Transmit_GPIO_Port GPIOA
*/

//#define PTT_SUPPRESSX_Pin GPIO_PIN_6
//#define PTT_SUPPRESSX_GPIO_Port GPIOD
#define T_BUSY_Pin GPIO_PIN_8
#define T_BUSY_GPIO_Port GPIOB
#define T_CSX_Pin GPIO_PIN_9
#define T_CSX_GPIO_Port GPIOB
#define PWR_on_sig_Pin GPIO_PIN_0
#define PWR_on_sig_GPIO_Port GPIOE
#define OverdriveX_Pin GPIO_PIN_1
#define OverdriveX_GPIO_Port GPIOE
#endif

// Band selector
extern uint8_t BAND_source; // select the control source of auto band
extern band_t BAND_switch; // the position of Band Rotary Switch
extern band_t BAND_soft; // Band set by software
// Current status
extern Transmission_State_t  trans_state; // transmission_state: STANDBY, RECEIVING, TRANSMITTING

// Menu configurations (to be save in EEPROM)
extern bool Enable_band_remote;

// Fan1, Fan2
extern Fan_t fan1, fan2;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
