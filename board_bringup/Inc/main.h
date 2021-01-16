/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
  GENERIC_STATUS_OK,
  GENERIC_STATUS_ERROR,
  GENERIC_STATUS_BUFFER_OVERFLOW,
  GENERIC_STATUS_BAD_COMMAND,
} GENERIC_STATUS_e;
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
#define GREEN_LED_Pin GPIO_PIN_5
#define GREEN_LED_GPIO_Port GPIOA
#define BATT_ADC_Pin GPIO_PIN_6
#define BATT_ADC_GPIO_Port GPIOA
#define MIC_IN_ADC_Pin GPIO_PIN_0
#define MIC_IN_ADC_GPIO_Port GPIOB
#define MIC_EN_Pin GPIO_PIN_2
#define MIC_EN_GPIO_Port GPIOB
#define CHARGER_STAT1_Pin GPIO_PIN_14
#define CHARGER_STAT1_GPIO_Port GPIOB
#define CHARGER_STAT2_Pin GPIO_PIN_15
#define CHARGER_STAT2_GPIO_Port GPIOB
#define BATT_ADC_EN_Pin GPIO_PIN_6
#define BATT_ADC_EN_GPIO_Port GPIOC
#define BOOST_EN_Pin GPIO_PIN_7
#define BOOST_EN_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

#define CRITICAL_SECTION_BEGIN uint32_t p = __get_PRIMASK(); __disable_irq();  
#define CRITICAL_SECTION_END   __set_PRIMASK(p);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
