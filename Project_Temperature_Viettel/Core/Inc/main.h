/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_TX485_Pin GPIO_PIN_13
#define LED_TX485_GPIO_Port GPIOC
#define LED_RX485_Pin GPIO_PIN_0
#define LED_RX485_GPIO_Port GPIOH
#define SW_SET_Pin GPIO_PIN_1
#define SW_SET_GPIO_Port GPIOH
#define NET485IO_Pin GPIO_PIN_0
#define NET485IO_GPIO_Port GPIOA
#define SW_DOWN_Pin GPIO_PIN_4
#define SW_DOWN_GPIO_Port GPIOA
#define SW_UP_Pin GPIO_PIN_5
#define SW_UP_GPIO_Port GPIOA
#define ON_SEN_Pin GPIO_PIN_2
#define ON_SEN_GPIO_Port GPIOB
#define SW_EXIT_Pin GPIO_PIN_11
#define SW_EXIT_GPIO_Port GPIOA
#define LED_BL_Pin GPIO_PIN_12
#define LED_BL_GPIO_Port GPIOA
#define SW_ON_BL_Pin GPIO_PIN_6
#define SW_ON_BL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
