/*
 * Rasp.h
 *
 *  Created on: Jan 17, 2023
 *      Author: quent
 */

#ifndef INC_RASP_H_
#define INC_RASP_H_

#include "main.h"
#include "usart.h"


#define CMD_BUFFER_SIZE 64
#define NB_CARACT 14
#define MAX_ARGS 9

void raspGetChar(void);
void raspErrorReceive(void);
void raspExec(void);

#endif /* INC_RASP_H_ */