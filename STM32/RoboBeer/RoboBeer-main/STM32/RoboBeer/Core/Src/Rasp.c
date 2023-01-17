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


extern int action; 		//mouvement à realiser (avancer tourner reculer stop)
extern int sens;
extern int couleur;
extern int TOF_dist;



uint8_t errlect[]="erreur lecture tram donnee\r\n";
uint8_t newlineRasp[]="\r\n";

char cmdBufferRasp[CMD_BUFFER_SIZE];
extern uint8_t 	uartRxBufferRasp[UART_RX_BUFFER_SIZE];
uint8_t	idxCmdRasp = 0;
char* argvRasp[MAX_ARGS];
uint8_t	argcRasp;
extern uint8_t uartTxBufferRasp[UART_TX_BUFFER_SIZE];
extern uint8_t stringSizeRasp;


uint8_t raspGetChar(void)
{
	uint8_t newCmdReady = 0;
	char* token;

	cmdBufferRasp[idxCmdRasp++] = uartRxBufferRasp[0];
	HAL_UART_Transmit(&huart1, uartRxBufferRasp, 1, HAL_MAX_DELAY); //ecrit à l'ordi

	if(idxCmdRasp == NB_CARACT)
		{
			HAL_UART_Transmit(&huart1, newlineRasp, sizeof(newlineRasp), HAL_MAX_DELAY);
			cmdBufferRasp[idxCmdRasp] = '\0';
			argcRasp = 0;
			token = (char*)strtok(cmdBufferRasp, " ");
			while(token!=NULL){
				argvRasp[argcRasp++] = token;
				token = (char*)strtok(NULL, " ");
			}

			idxCmdRasp = 0;
			newCmdReady = 1;
		}

	return newCmdReady;
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
		action = atoi(argvRasp[1]);
		sens = atoi(argvRasp[2]);
		couleur = atoi(argvRasp[3]);
		TOF_dist = atoi(argvRasp[4]);
		//printf("%d %d %d %d\r\n", action, sens, couleur, TOF_dist);
	}

	else{
		raspErrorReceive();
	}

}


