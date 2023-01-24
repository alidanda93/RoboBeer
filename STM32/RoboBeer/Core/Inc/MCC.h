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

#define MAX_ARR 200
#define Mid_Period_TIM2 65535/2
#define Mid_Period_TIM5 65535/2

#define RESOLUTION 16
#define OmaxG 159
#define OmaxD 154

#define AVANCER		0
#define RECULER		1
#define TOURNER		2
#define STOP		3

#define TOUR 2220


void InitMCC();
void Avancer(int);
void AvancerPI(int moteur, int vitesse);
void Reculer(int);
void ReculerPI(int moteur, int vitesse);
void Stop(void);
void Tourner(int, int);

void ReadEncodeur();



#endif /* INC_MCC_H_ */