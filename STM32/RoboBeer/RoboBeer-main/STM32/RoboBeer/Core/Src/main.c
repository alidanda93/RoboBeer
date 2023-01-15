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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <FreeRTOS.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver_ToF.h"
#include "led.h"
#include "Servo.h"
#include "MCC.h"
#include "PI.h"
#include "shell.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern uint8_t uartRxReceived;
extern uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

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

int it_userButton = 0;
int it_tim3 = 0;
int it_tim7 = 0;

uint16_t servo_angle=0;
int tickD = 0;
int tickG = 0;
int speedD = 0;
int speedG = 0;
int enableUserButton = 0;

BaseType_t xReturned_RASP;
TaskHandle_t xHandle_RASP = NULL;
SemaphoreHandle_t semaphoreRASP;


BaseType_t xReturned_ControlServo;
TaskHandle_t xHandle_ControlServo = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/*Normal functions*/


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
 * Task that would go get the TOF value.
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





void RaspRead (void * pvParameters) {
	uint8_t MSG_Buffer[200] = {'\0'};
	int duree = (int) pvParameters;
	while (1) {
		xSemaphoreTake(semaphoreRASP,RASP_UART_TO);
		HAL_UART_Receive(&huart2, MSG_Buffer, sizeof(MSG_Buffer),RASP_UART_TO);
		printf("%s\r\n", MSG_Buffer);
		xSemaphoreGive(semaphoreRASP);
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
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_ADC1_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, uartRxBuffer, UART_RX_BUFFER_SIZE);
  HAL_Delay(1);
  shellInit();

  TurnOffLed(1);
  TurnOffLed(2);
  TurnOffLed(3);
  TurnOffLed(4);

  SwitchLed(4);

  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);

  InitMCC();

  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

  HAL_TIM_Base_Start_IT(&htim7); //interrupt chaque second pour print les données dans le shell
  //HAL_Delay(3000);

  /*if(ControlServo(10000) != 0)
  {
	  Error_Handler();
  }*/

  /*
  SwitchLed(3);
  HAL_Delay(3000);
  SwitchLed(3);

  if(ControlServo(SERVO_CLOSED) != 0)
  {
	  Error_Handler();
  }
  */

  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  //HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  //initTof();




  /* Create the task, storing the handle. */
  /*
  printf("1 \r\n");
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

*/

  /*
  printf("2 \r\n");
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
*/


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

/*
  semaphoreRASP = xSemaphoreCreateBinary();
  xSemaphoreGive(semaphoreRASP);

  printf("3 \r\n");
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
*/

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  //MX_FREERTOS_Init();

  /* Start scheduler */
  //osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(uartRxReceived){
		  if(shellGetChar())
		  {
			  shellExec();
			  shellPrompt();
		  }
		  uartRxReceived = 0;
	  }
	  /*for(int i = SERVO_MIN_VALUE; i < SERVO_MAX_VALUE; i+=100)
	  {
		  if(ControlServo(i) != 0)
		    {
		  	  Error_Handler();
		    }
		  HAL_Delay(500);
	  }*/

	  if(it_userButton)
	  {
		  enableUserButton = (enableUserButton+1) % 2; //passe 0 à 1 et 1 à 0



		  //sprintf((char *)MSG, "Speed = %f tick/periode\n\r", -TICK2SPEED_TIM4 * speed);
		  //HAL_UART_Transmit(&huart2, MSG, sizeof(MSG), 100);

		  it_userButton = 0;
	  }

	  if(it_tim7)
	  {
		  ReadEncodeur();
		  //ReadSpeed();
		  if(enableUserButton)
		  {
			  uint8_t MSG[CMD_BUFFER_SIZE] = {'\0'};

			  sprintf((char *)MSG, "Encoder Ticks D = %d\n\r", tickD);
			  HAL_UART_Transmit(&huart1, MSG, sizeof(MSG), 100);
			  sprintf((char *)MSG, "Encoder Ticks G = %d\n\r", tickG);
			  HAL_UART_Transmit(&huart1, MSG, sizeof(MSG), 100);
		  }
		  it_tim7 = 0;
	  }

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3) it_tim3=1; // Tim 3 avce servo donc pas d'interrupt...inutile
	else if (htim->Instance == TIM7) it_tim7=1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)  // <----- The ISR Function We're Looking For!
{
	it_userButton = 1;
}

/**
  * @brief  Function called at each new character received
  * @retval None
  */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart)
{
	if(huart->Instance == USART1)
	{
		uartRxReceived = 1;
		HAL_UART_Receive_IT(&huart1, uartRxBuffer, UART_RX_BUFFER_SIZE);
	}
	else if(huart->Instance == USART2)
	{
		//interruption Raspberry
	}
}
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
  LedError();
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
