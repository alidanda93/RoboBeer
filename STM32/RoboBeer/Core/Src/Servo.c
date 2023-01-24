/**
  ******************************************************************************
  * @file           : Servo.c
  * @brief          : lib uses to init and command robobeer's servomotor
  * @author 		: quenphil42
  ******************************************************************************
  */

/*
 * Servo.c
 *
 *  Created on: Jan 12, 2023
 *      Author: quent
 */


#include "Servo.h"


/*
 * @ brief Function to control the servo motor
 *
 * @param angle : must be between 0 and 1024-1
 *
 * @output : 0 if all is OK, 1 if angle is not an allowed value
 */
int ControlServo(int angle){
	if ((angle <= SERVO_MAX_VALUE) && (angle >= SERVO_MIN_VALUE))
	{
		TIM3 -> CCR1 = angle;
		return 0;
	}
	else
	{
		return 1;
	}
}
