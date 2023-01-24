/**
  ******************************************************************************
  * @file           : MCC.c
  * @brief          : lib uses to init and command robobeer's MCC
  * @author 		: quenphil42
  ******************************************************************************
  */

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


/**
 * @brief Fonction qui active les Timers en mode PWM pour la commande des MCC
 *
 * @param None
 *
 * @retval None
 *
 */
void InitMCC()
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	Stop();
}

/**
 * @brief Fonction qui modifie la consigne de vitesse des moteurs pour avancer tout droit sans correceteur PI
 *
 * @param vitesse des roues entre 0 et MAX_ARR
 *
 * @retval None
 *
 */
void Avancer(int vitesse)
{
	if(vitesse > MAX_ARR || vitesse < 0) Error_Handler();
	TIM1->CCR1=vitesse;
	TIM1->CCR2=0;
	TIM1->CCR3=vitesse;
	TIM1->CCR4=0;

}

/**
 * @brief Fonction qui modifie la consigne de vitesse des moteurs pour avancer tout droit avec correceteur PI
 *
 * @param vitesse des roues entre 0 et MAX_ARR
 *
 * @retval None
 *
 */
void AvancerPI(int moteur, int vitesse)
{
	if(vitesse > MAX_ARR || vitesse < 0) Error_Handler();
	if (moteur)
	{
		TIM1->CCR1=0;
		TIM1->CCR2=vitesse;
	}
	else
	{
		TIM1->CCR3=0;
		TIM1->CCR4=vitesse;
	}
}

/**
 * @brief Fonction qui modifie la consigne de vitesse des moteurs pour reculer tout droit sans correceteur PI
 *
 * @param vitesse des roues entre 0 et MAX_ARR
 *
 * @retval None
 *
 */
void Reculer(int vitesse)
{
	if(vitesse > MAX_ARR || vitesse < 0) Error_Handler();
	TIM1->CCR1=0;
	TIM1->CCR2=vitesse;
	TIM1->CCR3=0;
	TIM1->CCR4=vitesse;
}


/**
 * @brief Fonction qui modifie la consigne de vitesse des moteurs pour reculer tout droit avec correceteur PI
 *
 * @param vitesse des roues entre 0 et MAX_ARR
 *
 * @retval None
 *
 */
void ReculerPI(int moteur, int vitesse)
{
	if(vitesse > MAX_ARR || vitesse < 0) Error_Handler();
	if (moteur)
	{
		TIM1->CCR1=vitesse;
		TIM1->CCR2=0;
	}
	else
	{
		TIM1->CCR3=vitesse;
		TIM1->CCR4=0;
	}
}

/**
 * @brief Fonction qui bloque les roues des MCC
 *
 * @param None
 *
 * @retval None
 *
 */
void Stop(void)
{
	TIM1->CCR1=MAX_ARR;
	TIM1->CCR2=MAX_ARR;
	TIM1->CCR3=MAX_ARR;
	TIM1->CCR4=MAX_ARR;
}


/**
 * @brief Fonction qui permet de faire tourner le robot dans le sens "sens" à la vitesse "vitesse"
 *
 * @param 	vitesse des roues entre 0 et MAX_ARR
 * @param 	sens = 1 tourner à droite
 * @param 	sens = 0 tourner à gauche
 *
 * @retval None
 *
 */
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


/**
 * @brief 	Fonction qui effectue la lecture du Timer2 et reset sa valeur.
 * 			Associe aux variables locales tickD et tickG les valeurs de la vitesse
 *
 * @param None
 *
 * @retval None
 *
 */
void ReadEncodeur()
{
	tickD = Mid_Period_TIM2 - (TIM2->CNT);
	TIM2->CNT = Mid_Period_TIM2;
	tickG = (TIM5->CNT) - Mid_Period_TIM5;
	TIM5->CNT = Mid_Period_TIM5;
}


