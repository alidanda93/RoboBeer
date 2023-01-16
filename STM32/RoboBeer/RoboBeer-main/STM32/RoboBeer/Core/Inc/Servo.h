/*
 * Servo.h
 *
 *  Created on: Jan 12, 2023
 *      Author: quent
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "main.h"

#define SERVO_MIN_VALUE 800		//should be between 1000 and 2000
#define SERVO_MAX_VALUE 2200

#define SERVO_OPEN 1400
#define SERVO_CLOSED 840

int ControlServo(int angle);

#endif /* INC_SERVO_H_ */
