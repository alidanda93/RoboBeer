/*
 * led.c
 *
 *  Created on: Jan 12, 2023
 *      Author: quent
 */


#include "led.h"


void TurnOnLed(int i)
{
	switch (i)
	{
	case 1:
		HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
		break;
	case 4:
		HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_SET);
		break;
	}

}



void TurnOffLed(int i)
{
	switch (i)
	{
	case 1:
		    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
		    break;
	case 2:
			HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
			break;
	case 3:
			HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
			break;
	case 4:
			HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_RESET);
			break;
	}

}



void SwitchLed(int i)
{
	switch (i)
	{
	case 1:
			HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
			break;
	case 2:
			HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
			break;
	case 3:
			HAL_GPIO_TogglePin(LED_3_GPIO_Port, LED_3_Pin);
			break;
	case 4:
			HAL_GPIO_TogglePin(LED_4_GPIO_Port, LED_4_Pin);
			break;
	}

}



void LedError()
{
	HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_SET);
}




