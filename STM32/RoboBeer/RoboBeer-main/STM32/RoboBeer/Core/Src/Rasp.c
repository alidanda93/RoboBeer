/*
 * Rasp.c
 *
 *  Created on: Jan 17, 2023
 *      Author: quent
 */

#include "Rasp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"


extern int enable;
extern int actionRasp; 		//mouvement à realiser (avancer tourner reculer stop)
extern int sensRasp;
extern int couleur;



uint8_t errlect[]="erreur lecture tram donnee\r\n";
uint8_t newlineRasp[]="\r\n";

char cmdBufferRasp[CMD_BUFFER_SIZE];
extern uint8_t 	uartRxBufferRasp[UART_RX_BUFFER_SIZE];
char* argvRasp[MAX_ARGS];
uint8_t	argcRasp;
extern uint8_t uartTxBufferRasp[UART_TX_BUFFER_SIZE];
extern uint8_t stringSizeRasp;
int bits[8] = {0};

void raspGetChar(void)
{

	HAL_UART_Transmit(&huart1, uartRxBufferRasp, NB_CARACT, HAL_MAX_DELAY); //ecrit à l'ordi
	HAL_UART_Transmit(&huart1, newlineRasp, sizeof(newlineRasp), HAL_MAX_DELAY);

	char* token;

	argcRasp = 0;
	for(int i = 0; i < UART_RX_BUFFER_SIZE_RASP; i++)
	{
		cmdBufferRasp[i] = uartRxBufferRasp[i];
	}

	token = (char*)strtok(cmdBufferRasp, " ");
	while(token!=NULL)
	{
		argvRasp[argcRasp++] = token;
		token = (char*)strtok(NULL, " ");
	}

}



void raspErrorReceive(void)
{
	HAL_UART_Transmit(&huart1, errlect, sizeof(errlect), HAL_MAX_DELAY);
}


/**
  * @brief  Call function depends of the value of argcRasp and argvRasp
  * @retval None
  */
void raspExec(void)
{
	if(strcmp(argvRasp[0],"FM")==0 && strcmp(argvRasp[5],"RT")==0)
	{
		enable = atoi(argvRasp[1]);
		actionRasp = atoi(argvRasp[2]);
		sensRasp = atoi(argvRasp[3]);
		couleur = atoi(argvRasp[4]);
		printf("%d %d %d %d\r\n", enable, actionRasp, sensRasp, couleur);
	}

	else{
		raspErrorReceive();
	}

}


