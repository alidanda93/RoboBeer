/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <FreeRTOS.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver_ToF.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define STACK_SIZE 1000
#define MAX_S_Time 3000


#define DELAY_1 10
#define DELAY_2 10
#define DELAY_3 100
#define DELAY_4 20

#define Code_Priority 2
#define PRIORITY_ENCODER 5
#define PRIORITY_TOF 7
#define PRIORITY_RASP 3
#define PRIORITY_SERVO 1


#define SERVO_OPEN 500
#define SERVO_CLOSED 2500

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

BaseType_t xReturned;
TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;

/*
 * These are the private variables for the Encoder
 */
BaseType_t xReturned_Encoder;
TaskHandle_t xHandle_Encoder = NULL;

	/*
	 * These are the private variables for the TOF
	 */
BaseType_t xReturned_TofGetDistance;
TaskHandle_t xHandle_TofGetDistance = NULL;

int it_tim3=0;
uint16_t servo_angle=0;

BaseType_t xReturned_RASP;
TaskHandle_t xHandle_RASP = NULL;


BaseType_t xReturned_ControlServo;
TaskHandle_t xHandle_ControlServo = NULL;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/*Normal functions*/
void ControlServo(int angle);

/*Task Functions*/
void codeTache (void * pvParameters);
void GetEncoderValue(void * pvParameters);
void TofGetDistance(void * pvParameters);
void RaspRead (void * pvParameters);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim3.Instance) {
		it_tim3=1;
	}
}

int __io_putchar(int ch) {
	HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, MAX_S_Time);
	return ch;
}

void codeTache (void * pvParameters) {
	int compteur = 0;
	int duree = (int) pvParameters;
	char* s = pcTaskGetName(xTaskGetCurrentTaskHandle());
	while (1) {
		printf("Je suis la tache %s et je m'endors pour %d periodes\n", s, duree);
		vTaskDelay(duree);
		compteur++;
	}
}



/*
 * Task taht would go get the TOF value.
 * Send it via UART or printf
 * Task Delay of DELAY_TOF
 */

void TofGetDistance(void * pvParameters)
{
	printf("Ici init TOF\r\n");
	int distance;
	int duree = (int) pvParameters;
	while(1){
		distance = tofReadDistance();//scan
		printf("distance : %d\r\n", distance);
		if (distance <60){
			ControlServo(SERVO_OPEN);
		}else{
			ControlServo(SERVO_CLOSED);
		}
		vTaskDelay(duree);
	}
}


/*
 * Task that would go get the encoder value from the counter of the TIM3.
 * Transmit value via UART
 * Task Delay 1 OS tick = 1ms.
 * */
void GetEncoderValue(void * pvParameters){
	printf("Ici init Encodeur\r\n");
	uint16_t angle;
	int duree = (int) pvParameters;
	while(1){
		angle = (TIM3->CNT);
		printf("Encoder Ticks = %d\n\r", angle);
		vTaskDelay(duree);
	}
}

/*
 * Function to control the servo motor
 */
void ControlServo(int angle){
	TIM1 -> CCR1 = angle;
	it_tim3=0;
}



void RaspRead (void * pvParameters) {
	uint8_t MSG_Buffer[200] = {'\0'};
	int duree = (int) pvParameters;
	while (1) {
		HAL_UART_Receive(&huart3, MSG_Buffer, sizeof(MSG_Buffer), MAX_S_Time);
		printf("########## RASP says :   %s \r\n", MSG_Buffer);
		vTaskDelay(duree);
	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  initTof();

  /* Create the task, storing the handle. */

  xReturned_Encoder = xTaskCreate(
		  GetEncoderValue, 				// Function that implements the task.
		  "Encodeur", 					// Text name for the task.
		  STACK_SIZE, 					// Stack size in words, not bytes.
		  (void *) DELAY_1, 			// Parameter passed into the task.
		  PRIORITY_ENCODER,				// Priority at which the task is created.
		  &xHandle_Encoder ); 			// Used to pass out the created task's handle.

  if (xReturned_Encoder == pdPASS ){
	  printf("Task GetEncoderValue created \r\n");
  } else{
		printf("ERROR GetEncoderValue not created \r\n");
		while(1){}
  }



  xReturned_TofGetDistance = xTaskCreate(
		  TofGetDistance, 				// Function that implements the task.
		  "TOF", 						// Text name for the task.
		  STACK_SIZE, 					// Stack size in words, not bytes.
		  (void *) DELAY_2, 			// Parameter passed into the task.
		  PRIORITY_TOF,					// Priority at which the task is created.
		  &xHandle_TofGetDistance );	// Used to pass out the created task's handle.

  if(xReturned_TofGetDistance == pdPASS){
	  printf("Task TofGetDistance created \r\n");
  } else{
	printf("ERROR TofGetDistance NOT created \r\n");
	while(1){}
  }

/*
  xReturned_ControlServo = xTaskCreate(
		  ControlServo, 				// Function that implements the task.
		  "ServoMotor", 						// Text name for the task.
		  STACK_SIZE, 					// Stack size in words, not bytes.
		  (int *) servo_angle, 			// Parameter passed into the task.
		  PRIORITY_SERVO,					// Priority at which the task is created.
		  &xHandle_ControlServo );	// Used to pass out the created task's handle.

  if(xReturned_ControlServo == pdPASS){
	  printf("Task ControlServo created \r\n");
  } else{
	printf("ERROR ControlServo NOT created \r\n");
	while(1){}
  }
 */


  xReturned_RASP = xTaskCreate(
		  RaspRead, 					// Function that implements the task.
		  "Raspberry", 					// Text name for the task.
		  STACK_SIZE, 					// Stack size in words, not bytes.
		  (void *) DELAY_3, 			// Parameter passed into the task.
		  PRIORITY_RASP,				// Priority at which the task is created.
		  &xHandle_RASP ); 				// Used to pass out the created task's handle.

  if (xReturned_RASP == pdPASS ){
	  printf("Task RaspRead created \r\n");
  } else{
		printf("ERROR RaspRead not created \r\n");
		while(1){}
  }

  vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
