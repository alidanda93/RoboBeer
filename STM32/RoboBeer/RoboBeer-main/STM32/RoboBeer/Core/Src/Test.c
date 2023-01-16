/*
 * Test.c
 *
 *  Created on: Jan 16, 2023
 *      Author: quent
 */


#include "MCC.h"
#include "servo.h"
#include "cmsis_os.h"

extern int dist;
extern int X_odo;
extern int Y_odo;
extern int tickD;
extern int consigneD;
extern int tickG;
extern int consigneG;
extern int action; 		//mouvement à realiser (avancer tourner reculer stop)
extern int sens;

int start = 0;

#define DIST 3000
#define DELAY 1000

void Debut_Test()
{
	start = (start+1)%2;
	X_odo = 0;
	Y_odo = 0;
	dist = 0;
}

void Test_Odometrie_Carre()
{
	/* avancer 1*/

	consigneD = 200;	//vitesse
	consigneG = 200;	//vitesse
	action = AVANCER;
	while(dist < DIST)
	{

	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);



	/* tourner gauche */

	action = TOURNER;
	while(dist < TOUR/4)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	/* avancer 2 */

	action = AVANCER;
	dist = 0;
	while(dist < DIST)
	{

	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	/* tourner gauche */

	action = TOURNER;
	while(dist < TOUR/4)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	/*	avancer 3 */

	action = AVANCER;
	dist = 0;
	while(dist < DIST)
	{

	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);


	/* tourner gauche */

	action = TOURNER;
	while(dist < TOUR/4)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);


	/* avancer 4 */


	action = AVANCER;
	dist = 0;
	while(dist < DIST)
	{

	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	/* tourner gauche */

	action = TOURNER;
	while(dist < TOUR/4)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);


	//fin du carre
	/* ouvrir + fermer pince */
	ControlServo(SERVO_OPEN);
	HAL_Delay(2000);
	ControlServo(SERVO_CLOSED);
	HAL_Delay(2000);
}


void Test_Tourner()
{
	consigneD = 200;	//vitesse
	consigneG = 200;	//vitesse
	action = TOURNER;
	while(dist < 2*TOUR)
	{
	}
	action =  STOP;
	dist = 0;
	ControlServo(SERVO_OPEN);
	HAL_Delay(2000);
	ControlServo(SERVO_CLOSED);
	HAL_Delay(2000);
}
