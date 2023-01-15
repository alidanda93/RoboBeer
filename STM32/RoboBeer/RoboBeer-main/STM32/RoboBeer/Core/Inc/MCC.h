/*
 * MCC.h
 *
 *  Created on: Jan 14, 2023
 *      Author: quent
 */

#ifndef INC_MCC_H_
#define INC_MCC_H_


#include "tim.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MAX_ARR 200
#define Mid_Period_TIM2 65535/2
#define Mid_Period_TIM5 65535/2


void InitMCC();
void Avancer(int);
void Reculer(int);
void Stop(void);
void Tourner(int, int);

void ReadEncodeur();
void ReadSpeed();




#endif /* INC_MCC_H_ */
