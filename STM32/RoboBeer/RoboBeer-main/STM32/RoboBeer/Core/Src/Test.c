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

extern int TOF_dist;

int start = 0;

#define DIST 3000
#define DELAY 1000
#define OFFSET_OUVERTURE_PINCE 1150

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


/*Avant test Attention :
 * verifier que TOF_dist est MAJ souvent (mettre dans Tim3)
 * verifier valeur TOF_dist quand rien n'est detecte (est ce 0 ou Max Val ?)
 */
void Test_Canette()
{

	int angle = 0;
	int dist_cannette = 0;

	/* Init - ouverture fermeture des pinces */
	ControlServo(SERVO_OPEN);
	HAL_Delay(DELAY);
	ControlServo(SERVO_CLOSED);
	HAL_Delay(DELAY);


	/* Detection canette - rotation + lecture TOF + stockage angle */
	consigneD = 200;	//vitesse
	consigneG = 200;	//vitesse
	action = TOURNER;
	dist = 0;
	while( TOF_dist == 0 || (dist > TOUR))
	{
	}
	action =  STOP;
	HAL_Delay(DELAY);
	if(dist > TOUR) Error_Handler(); 	//Systeme en default cannette non vue
	angle = dist; 						//on memorise la valeur de l'angle parcouru
	dist_cannette = TOF_dist;			//on memorise la distance à parcourir


	/* Avancer tout droit + mesure distance jusqu'à TOF = 0*/
	action =  AVANCER;
	dist = 0;
	while(dist < dist_cannette - OFFSET_OUVERTURE_PINCE)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	/* Ouvrir PINCE + Avancer tout droit OFFSET MESURE TOF */

	ControlServo(SERVO_OPEN);
	HAL_Delay(DELAY);

	action =  AVANCER;
	dist = 0;
	while(dist < OFFSET_OUVERTURE_PINCE )
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);


	/* Fermer Pince + Rotation demi tour*/

	ControlServo(SERVO_CLOSED);
	HAL_Delay(DELAY);
	action = TOURNER;
	while(dist < TOUR/2)
	{
	}
	action =  STOP;
	dist = 0;

	/* Avancer valeur dist mesuree */

	action =  AVANCER;
	dist = 0;
	while(dist < dist_cannette)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);


	/* Tourner (- angle) et avancer OFFSET 10cm*/

	sens = 1;
	action = TOURNER;
	while(dist < angle)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	action =  AVANCER;
	dist = 0;
	while(dist < OFFSET_OUVERTURE_PINCE)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	/* ouvrir pince + Reculer OFFSET 10cm*/

	ControlServo(SERVO_OPEN);
	HAL_Delay(DELAY);

	action =  RECULER;
	dist = 0;
	while(dist < OFFSET_OUVERTURE_PINCE)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);

	/* Demi Tour*/

	sens = 0;
	action = TOURNER;
	while(dist < TOUR/2)
	{
	}
	action =  STOP;
	dist = 0;
	HAL_Delay(DELAY);


	/* Fin Service */

	ControlServo(SERVO_OPEN);
	HAL_Delay(DELAY);
	ControlServo(SERVO_CLOSED);
	HAL_Delay(DELAY);

	while(1){}

}