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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver_ToF.h"
#include "led.h"
#include "Servo.h"
#include "MCC.h"
#include "PI.h"
#include "shell.h"
#include "Test.h"
#include "Rasp.h"
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

extern uint8_t uartRxReceivedRasp;
extern uint8_t uartRxBufferRasp[UART_RX_BUFFER_SIZE];
extern uint8_t uartTxBufferRasp[UART_TX_BUFFER_SIZE];

//BaseType_t xReturned;
//TaskHandle_t xHandle1 = NULL;
//TaskHandle_t xHandle2 = NULL;
//
///*
// * These are the private variables for the Encoder
// */
//BaseType_t xReturned_Encoder;
//TaskHandle_t xHandle_Encoder = NULL;
//
///*
// * These are the private variables for the TOF
// */
//BaseType_t xReturned_TofGetDistance;
//TaskHandle_t xHandle_TofGetDistance = NULL;


uint16_t servo_angle=0;

uint16_t dist = 0;
int X_odo = 0;
int Y_odo = 0;
int tickD = 0;
int consigneD = 0;
int tickG = 0;
int consigneG = 0;
int action = 3; 		//mouvement à realiser (avancer tourner reculer stop)
int sens = 0;
int couleur = 0;
extern int start;

int TOF_dist = 0;

int enableUserButton = 0;

PIController MoteurD;
PIController MoteurG;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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


void TestSensorOnI2C(int * tab)
{
	 int u = 0;

	 printf("debut de lecture du bus I2c\r\n\n");
	  for(int i =0; i< 256; i++)
	  {
		  int result = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i),2,2);
		  if (result != HAL_OK)//HAL_ERROR or HAL_TIMEOUT
		  {
			  printf("."); //No ACKreceived at the address
		  }
		  if (result == HAL_OK)
		  {
			  tab[u] = i;
			  u++;
			  printf("0x%X",i); //Received an ACK at the address
		  }
	  }
	  printf("\r\n\nfin de lecture du bus I2C\r\n\n\n");
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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_ADC1_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();
  MX_TIM4_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */



  HAL_UART_Receive_IT(&huart1, uartRxBuffer, UART_RX_BUFFER_SIZE);
  HAL_Delay(1);
  shellInit();

  HAL_UART_Receive_IT(&huart2, uartRxBufferRasp, UART_RX_BUFFER_SIZE);
  HAL_Delay(1);

  int tabI2CAdd[8] = {0,0,0,0,0,0,0,0};
  TestSensorOnI2C(tabI2CAdd);
  initTof();
  //int tabI2CAdd[8] = {0,0,0,0,0,0,0,0};
  //TestSensorOnI2C(tabI2CAdd);

  TurnOffLed(1);
  TurnOffLed(2);
  TurnOffLed(3);
  TurnOffLed(4);

  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
  ControlServo(SERVO_CLOSED);

  HAL_TIM_Base_Start_IT(&htim6);
  InitMCC();

  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

  PIController_Init(&MoteurD);
  PIController_Init(&MoteurG);

  HAL_TIM_Base_Start_IT(&htim7); //interrupt chaque second pour print les données dans le shell
  HAL_TIM_Base_Start_IT(&htim4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(start)
	  {
		  //Test_Odometrie_Carre();
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
	if (htim->Instance == TIM3) // 50Hz freq lecture TOF
	{
		//TOF_dist = tofReadDistance();//scan
	}



	else if (htim->Instance == TIM7) //Tim 7 1sec pour du printf shell
	{
		//printf("%d\r\n", dist);
		printf("ali petit zizi ca rime...je dis ca je dis rien\r\n");
		TOF_dist = tofReadDistance();//scan
		printf("distance : %d\r\n", TOF_dist);
	}

	else if (htim->Instance == TIM4) //Delay Tof
		{
		}

	else if (htim->Instance == TIM6) //Tim 6 asserv en vitesse a 0.1sec
		{

		switch(action)
		{
		case TOURNER :
			if(sens)dist -=	(( (TIM2->CNT)-Mid_Period_TIM2 ) + ((TIM5->CNT) - Mid_Period_TIM5 ) ) / 2; //on moyenne la valeur
			else dist +=	(( (TIM2->CNT)-Mid_Period_TIM2 ) + ((TIM5->CNT) - Mid_Period_TIM5 ) ) / 2; //on moyenne la valeur
			break;
		default :
			dist +=	(( (TIM2->CNT)-Mid_Period_TIM2 ) + ( Mid_Period_TIM5 - (TIM5->CNT)) ) / 2; //on moyenne la valeur
			break;
		}

		  ReadEncodeur();

		  if(consigneD == 0)
		  {
			  Stop();
		  }

		  else
		  {
			switch(action)
			{
			case AVANCER :
				PIController_Update(&MoteurD, consigneD, tickD * MAX_ARR / OmaxD);
				PIController_Update(&MoteurG, consigneG, tickG * MAX_ARR / OmaxG);
				AvancerPI(0, MoteurD.out);
				AvancerPI(1, MoteurG.out);
				break;

			case RECULER :
				PIController_Update(&MoteurD, consigneD, tickD * MAX_ARR / OmaxD);
				PIController_Update(&MoteurG, consigneG, tickG * MAX_ARR / OmaxG);
				ReculerPI(0, MoteurD.out);
				ReculerPI(1, MoteurG.out);
				break;

			case TOURNER :
				PIController_Update(&MoteurD, consigneD, tickD * MAX_ARR / OmaxD);
				PIController_Update(&MoteurG, consigneG, tickG * MAX_ARR / OmaxG);
				if(sens)
				{
					ReculerPI(0, MoteurD.out);
					AvancerPI(1, MoteurG.out);
				}
				else
				{
					AvancerPI(0, MoteurD.out);
					ReculerPI(1, MoteurG.out);
				}
				break;

			default :
				Stop();
				break;
			}

		  }
		}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)  // <----- The ISR Function We're Looking For!
{
	//enableUserButton = (enableUserButton+1) % 2; //passe 0 à 1 et 1 à 0
	Debut_Test();
}

/**
  * @brief  Function called at each new character received
  * @retval None
  */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart)
{
	if(huart->Instance == USART1)
	{
		HAL_UART_Receive_IT(&huart1, uartRxBuffer, UART_RX_BUFFER_SIZE);
		if(shellGetChar())
		{
		  shellExec();
		  shellPrompt();
		}
	}
	else if(huart->Instance == USART2)
	{
		HAL_UART_Receive_IT(&huart2, uartRxBufferRasp, UART_RX_BUFFER_SIZE);
		if(raspGetChar())
		{
			raspExec();
		}
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
