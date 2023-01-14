/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define USER_BTN_Pin GPIO_PIN_13
#define USER_BTN_GPIO_Port GPIOC
#define USER_BTN_EXTI_IRQn EXTI15_10_IRQn
#define MOTOR_2_ENCODER_A_Pin GPIO_PIN_0
#define MOTOR_2_ENCODER_A_GPIO_Port GPIOA
#define MOTOR_2_ENCODER_B_Pin GPIO_PIN_1
#define MOTOR_2_ENCODER_B_GPIO_Port GPIOA
#define RASP_USART2_TX_Pin GPIO_PIN_2
#define RASP_USART2_TX_GPIO_Port GPIOA
#define RASP_USART2_RX_Pin GPIO_PIN_3
#define RASP_USART2_RX_GPIO_Port GPIOA
#define MOTOR_1_ADC1_CURRENT_SENSOR_Pin GPIO_PIN_4
#define MOTOR_1_ADC1_CURRENT_SENSOR_GPIO_Port GPIOA
#define MOTOR_2_ADC1_CURRENT_SENSOR_Pin GPIO_PIN_5
#define MOTOR_2_ADC1_CURRENT_SENSOR_GPIO_Port GPIOA
#define BDT_1_Pin GPIO_PIN_6
#define BDT_1_GPIO_Port GPIOA
#define BDT_2_Pin GPIO_PIN_7
#define BDT_2_GPIO_Port GPIOA
#define BDT_3_Pin GPIO_PIN_0
#define BDT_3_GPIO_Port GPIOB
#define BDT_4_Pin GPIO_PIN_1
#define BDT_4_GPIO_Port GPIOB
#define COLOR_I2C_SCL_Pin GPIO_PIN_10
#define COLOR_I2C_SCL_GPIO_Port GPIOB
#define COLOR_I2C_SDA_Pin GPIO_PIN_11
#define COLOR_I2C_SDA_GPIO_Port GPIOB
#define LED_1_Pin GPIO_PIN_12
#define LED_1_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_13
#define LED_2_GPIO_Port GPIOB
#define LED_3_Pin GPIO_PIN_14
#define LED_3_GPIO_Port GPIOB
#define LED_4_Pin GPIO_PIN_15
#define LED_4_GPIO_Port GPIOB
#define PINCE_PWM_Pin GPIO_PIN_6
#define PINCE_PWM_GPIO_Port GPIOC
#define MOTOR_2_PWM_IN2_Pin GPIO_PIN_8
#define MOTOR_2_PWM_IN2_GPIO_Port GPIOA
#define MOTOR_2_PWM_IN1_Pin GPIO_PIN_9
#define MOTOR_2_PWM_IN1_GPIO_Port GPIOA
#define MOTOR_1_PWM_IN2_Pin GPIO_PIN_10
#define MOTOR_1_PWM_IN2_GPIO_Port GPIOA
#define MOTOR_1_PWM_IN1_Pin GPIO_PIN_11
#define MOTOR_1_PWM_IN1_GPIO_Port GPIOA
#define MOTOR_1_ENCODER_A_Pin GPIO_PIN_15
#define MOTOR_1_ENCODER_A_GPIO_Port GPIOA
#define MOTOR_1_ENCODER_B_Pin GPIO_PIN_3
#define MOTOR_1_ENCODER_B_GPIO_Port GPIOB
#define TOF_Interrupt_Pin GPIO_PIN_4
#define TOF_Interrupt_GPIO_Port GPIOB
#define TOF_XSHUT_Pin GPIO_PIN_5
#define TOF_XSHUT_GPIO_Port GPIOB
#define STLINK_USART1_TX_Pin GPIO_PIN_6
#define STLINK_USART1_TX_GPIO_Port GPIOB
#define STLINK_USART1_RX_Pin GPIO_PIN_7
#define STLINK_USART1_RX_GPIO_Port GPIOB
#define TOF_I2C_SCL_Pin GPIO_PIN_8
#define TOF_I2C_SCL_GPIO_Port GPIOB
#define TOF_I2C_SDA_Pin GPIO_PIN_9
#define TOF_I2C_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define STACK_SIZE 1000
#define MAX_S_Time 300


#define DELAY_1 10
#define DELAY_2 10
#define DELAY_3 50
#define DELAY_4 20


#define Code_Priority 2
#define PRIORITY_ENCODER 5
#define PRIORITY_TOF 7
#define PRIORITY_RASP 3
#define PRIORITY_SERVO 1


#define RASP_UART_TO 300




/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
