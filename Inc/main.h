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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define BT_GPIO_0_Pin GPIO_PIN_2
#define BT_GPIO_0_GPIO_Port GPIOC
#define BT_GPIO_1_Pin GPIO_PIN_3
#define BT_GPIO_1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_5
#define LD4_GPIO_Port GPIOA
#define BATT_ADC_IN_Pin GPIO_PIN_6
#define BATT_ADC_IN_GPIO_Port GPIOA
#define BT_GPIO_2_Pin GPIO_PIN_4
#define BT_GPIO_2_GPIO_Port GPIOC
#define BT_GPIO_3_Pin GPIO_PIN_5
#define BT_GPIO_3_GPIO_Port GPIOC
#define BATT_ADC_ENABLE_Pin GPIO_PIN_6
#define BATT_ADC_ENABLE_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define ACCEL_EXTI_0_Pin GPIO_PIN_6
#define ACCEL_EXTI_0_GPIO_Port GPIOB
#define ACCEL_EXTI_1_Pin GPIO_PIN_7
#define ACCEL_EXTI_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define CRITICAL_SECTION_BEGIN uint32_t p = __get_PRIMASK(); \
                               __disable_irq();  

#define CRITICAL_SECTION_END   __set_PRIMASK(p);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
