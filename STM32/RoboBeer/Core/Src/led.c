/**
  ******************************************************************************
  * @file           : led.c
  * @brief          : lib uses to command robobeer user's leds
  * @author 		: quenphil42
  ******************************************************************************
  */

/*
 * led.c
 *
 *  Created on: Jan 12, 2023
 *      Author: quent
 */


#include "led.h"

/**
 * @brief Fonction qui permet d'activer la led i de la carte mere
 *
 * @param entier allant de 1 à 4 indiquant le numero de la led qu'on souhaite allumer
 *
 * @retval None
 *
 */
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


/**
 * @brief Fonction qui permet d'eteindre la led i de la carte mere
 *
 * @param entier allant de 1 à 4 indiquant le numero de la led qu'on souhaite allumer
 *
 * @retval None
 *
 */
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


/**
 * @brief Fonction qui permet de changer l'etat de la led i de la carte mere
 *
 * @param entier allant de 1 à 4 indiquant le numero de la led qu'on souhaite allumer
 *
 * @retval None
 *
 */
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


/**
 * @brief Fonction qui allume toutes les leds
 *
 * @param None
 *
 * @retval None
 *
 */
void LedError()
{
	HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_SET);
}




