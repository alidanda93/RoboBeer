/*
 * MCC.c
 *
 *  Created on: Jan 14, 2023
 *      Author: quent
 */


#include "MCC.h"

extern int tickD;
extern int tickG;
extern int speedD;
extern int speedG;

void InitMCC()
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	Stop();
}

void Avancer(int vitesse)
{
	if(vitesse > MAX_ARR || vitesse < 0) Error_Handler();
	TIM1->CCR1=vitesse;
	TIM1->CCR2=0;
	TIM1->CCR3=vitesse;
	TIM1->CCR4=0;

}

void Reculer(int vitesse)
{
	if(vitesse > MAX_ARR || vitesse < 0) Error_Handler();
	TIM1->CCR1=0;
	TIM1->CCR2=vitesse;
	TIM1->CCR3=0;
	TIM1->CCR4=vitesse;
}

void Stop(void)
{
	TIM1->CCR1=MAX_ARR;
	TIM1->CCR2=MAX_ARR;
	TIM1->CCR3=MAX_ARR;
	TIM1->CCR4=MAX_ARR;
}

//sens = 1 tourner a droite
//sens = 0 tourner a gauche
void Tourner(int sens, int vitesse)
{
	if(vitesse > MAX_ARR || vitesse < 0) Error_Handler();
	if(sens)
	{
		TIM1->CCR1=vitesse;
		TIM1->CCR2=0;
		TIM1->CCR3=0;
		TIM1->CCR4=vitesse;
	}

	else if(!sens)
	{
		    TIM1->CCR1=0;
			TIM1->CCR2=vitesse;
			TIM1->CCR3=vitesse;
			TIM1->CCR4=0;
	}
	else Error_Handler();

}

void ReadEncodeur()
{
	tickD = (TIM2->CNT);
	tickG = (TIM5->CNT);
}

void ReadSpeed()
{
	speedD = ((TIM2->CNT))- Mid_Period_TIM2;
	TIM2->CNT = Mid_Period_TIM2;

	speedG = ((TIM5->CNT))- Mid_Period_TIM5;
	TIM5->CNT = Mid_Period_TIM5;
}



