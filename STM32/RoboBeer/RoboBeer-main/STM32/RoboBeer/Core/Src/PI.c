/**
  ******************************************************************************
  * @file    PI.c
  * @brief   Ce fichier contient le code en rapport avec l'asservissement
  ******************************************************************************
  *  Created on: Dec 8, 2022
  *      Author: quent
  *
  ******************************************************************************
  */


#include "PI.h"
#include "MCC.h"

/**
 * @brief Fonction qui fixe la sortie du PI et les valeurs des mémoires à 0.0
 *
 * @param struct pi
 *
 * @return None
 */
void PIController_Init(PIController *pi)
{

	pi->integrator = 0.0;
	pi->prevError  = 0.0;

	pi->out = 0.0;

	pi->Kp = 1.0;
	pi->Ki = 0.5;

	pi->limMin_output = 0;
	pi->limMax_output = MAX_ARR;

		/* Integrator limits */
	pi->limMin_integrator = 0;
	pi->limMax_integrator = MAX_ARR;

		/* Sample time (in seconds) */
	pi->T = 0.1; // 10Hz frequence Tim1

		/* Controller "memory" */
	pi->integrator = 0.0;
	pi->prevError = 0.0;		/* Required for integrator */

		/* Controller output */
	pi->out = 0.0;

}

/**
 * @brief Fonction qui met à jour les valeurs de la structure PI et renvoi la sortie du bloc PI
 *
 * @param struct pi
 * @param consigne
 * @param measurement
 *
 * @return None
 */
float PIController_Update(PIController *pi, float consigne, float measurement)
{

    float error = consigne - measurement;

    float proportional = pi->Kp * error;	//Proportional

    pi->integrator = pi->integrator + 0.5 * pi->Ki * pi->T * (error + pi->prevError);	//Integrator

    if (pi->integrator > pi->limMax_integrator) pi->integrator = pi->limMax_integrator; //anti-wind-up par ecretage
    else if (pi->integrator < pi->limMin_integrator) pi->integrator = pi->limMin_integrator;


    pi->out = proportional + pi->integrator;	//Output

    if (pi->out > pi->limMax_output) pi->out = pi->limMax_output;
    else if (pi->out < pi->limMin_output) pi->out = pi->limMin_output;

    pi->prevError       = error;

    return pi->out;

}
