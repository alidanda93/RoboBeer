/**
  ******************************************************************************
  * @file    shell.c
  * @brief   This file provides code for shell interface
  ******************************************************************************
  *  Created on: Nov 7, 2022
  *      Author: nicolas
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "usart.h"


uint8_t prompt[]="robobeer>";
uint8_t started[]=
		"\r\n*-------------------------------*"
		"\r\n| Welcome on the robobeer shell |"
		"\r\n*-------------------------------*"
		"\r\n";
uint8_t newline[]="\r\n";
uint8_t help[]=
		"\r\n*-------------------------------*"
		"\r\n| Functions available           |"
		"\r\n*-------------------------------*\r\n"
		"help : print this message\r\n"
		"RTFM : do it!\r\n"
		"led : turn on/off la led i ou i va de 1 a 4\r\n";


uint8_t RTFM[]=
		"RTFM est, en anglais, le sigle de la phrase Read the fucking manual"
		"(littéralement « lis le putain de manuel »), injonction signifiant que"
		"la réponse à une question sur le fonctionnement d'un appareil est à chercher "
		"dans son mode d emploi. Elle est surtout utilisée dans le jargon informatique,"
		"par exemple dans les forums de discussion.\n\r" ;

uint8_t led[]="turn on/off la led selectionnee\n\r";

uint8_t cmdNotFound[]="Command not found\r\n";


char cmdBuffer[CMD_BUFFER_SIZE];
extern uint8_t 	uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t	idxCmd;
char* argv[MAX_ARGS];
uint8_t	argc;
extern uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];
extern uint8_t stringSize;



/**
  * @brief  Send a stating message
  * @retval None
  */
void shellInit(void){
	HAL_UART_Transmit(&huart1, started, sizeof(started), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, prompt, sizeof(prompt), HAL_MAX_DELAY);
}

/**
  * @brief  Send the prompt
  * @retval None
  */
void shellPrompt(void){
	HAL_UART_Transmit(&huart1, prompt, sizeof(prompt), HAL_MAX_DELAY);
}

/**
  * @brief  Send the default message if the command is not found
  * @retval None
  */
void shellCmdNotFound(void){
	HAL_UART_Transmit(&huart1, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
}

/**
  * @brief  Function called for saving the new character and call and setup argc and argv variable if ENTER is pressed
  * @retval 1 if a new command is available, 0 if not.
  */
uint8_t shellGetChar(void){
	uint8_t newCmdReady = 0;
	char* token;

	switch(uartRxBuffer[0]){
		// If Enter, update argc and argv
	case ASCII_CR:
		HAL_UART_Transmit(&huart1, newline, sizeof(newline), HAL_MAX_DELAY);
		cmdBuffer[idxCmd] = '\0';
		argc = 0;
		token = (char*)strtok(cmdBuffer, " ");
		while(token!=NULL){
			argv[argc++] = token;
			token = (char*)strtok(NULL, " ");
		}

		idxCmd = 0;
		newCmdReady = 1;
		break;
		// Delete last character if "return" is pressed
	case ASCII_BS:
		cmdBuffer[idxCmd--] = '\0';
		HAL_UART_Transmit(&huart1, uartRxBuffer, 1, HAL_MAX_DELAY);
		break;
		// Default state : add new character to the command buffer
	default:
		cmdBuffer[idxCmd++] = uartRxBuffer[0];
		HAL_UART_Transmit(&huart1, uartRxBuffer, 1, HAL_MAX_DELAY);
	}

	return newCmdReady;
}

/**
  * @brief  Call function depends of the value of argc and argv
  * @retval None
  */
void shellExec(void){
	if(strcmp(argv[0],"help")==0)
	{
		HAL_UART_Transmit(&huart1, help, sizeof(help), HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],"RTFM")==0)
	{
		HAL_UART_Transmit(&huart1, RTFM, sizeof(RTFM), HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],"led")==0)
	{
		HAL_UART_Transmit(&huart1, led, sizeof(led), HAL_MAX_DELAY);
		SwitchLed(atoi(argv[1]));
	}
	else{
		shellCmdNotFound();
	}
}

