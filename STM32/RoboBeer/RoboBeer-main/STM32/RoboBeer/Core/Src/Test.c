/*
 * Test.c
 *
 *  Created on: Jan 16, 2023
 *      Author: quent
 */


#include "MCC.h"

extern int X_odo;
extern int Y_odo;
extern int tickD;
extern int consigneD;
extern int tickG;
extern int consigneG;
extern int action; 		//mouvement à realiser (avancer tourner reculer stop)
extern int sens;

int start = 0;



void Debut_Test()
{
	start = (start+1)%2;
	X_odo = 0;
	Y_odo = 0;
}

void Test_Odometrie_Carre()
{
	/* avancer 1*/



	/* tourner gauche */

	/* avancer 2 */

	/* tourner gauche */

	/*	avancer 3 */

	/* tourner gauche */

	/* avancer 4 */

	/* tourner gauche */

	//fin du carre
	/* ouvrir + fermer pince */
}
