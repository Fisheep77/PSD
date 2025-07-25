/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define BM50_PWM_Pin GPIO_PIN_0
#define BM50_PWM_GPIO_Port GPIOA
#define BM50_DIR_Pin GPIO_PIN_1
#define BM50_DIR_GPIO_Port GPIOA
#define BM50_BTAK_Pin GPIO_PIN_4
#define BM50_BTAK_GPIO_Port GPIOA
#define Servo1_PWM_Pin GPIO_PIN_6
#define Servo1_PWM_GPIO_Port GPIOA
#define Servo2_PWM_Pin GPIO_PIN_7
#define Servo2_PWM_GPIO_Port GPIOA
#define BM50_EncoderA_Pin GPIO_PIN_11
#define BM50_EncoderA_GPIO_Port GPIOA
#define BM50_EncoderB_Pin GPIO_PIN_12
#define BM50_EncoderB_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#include "bsp.h"
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
