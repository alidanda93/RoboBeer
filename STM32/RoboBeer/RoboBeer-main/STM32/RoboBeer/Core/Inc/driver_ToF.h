#ifndef INC_DRIVER_TOF_H_
#define INC_DRIVER_TOF_H_





//
// Copyright (c) 2017 Larry Bank
// email: bitbank@pobox.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stm32f1xx_hal.h"
#include "main.h"
#include "i2c.h"

#define ToF_Device_Address 0x52
extern I2C_HandleTypeDef hi2c2;

//
// Read the model and revision of the
// tof sensor
//
int tofGetModel(int *model, int *revision);

//
// Read the current distance in mm
//
int tofReadDistance(void);

//
// Opens a file system handle to the I2C device
// sets the device continous capture mode
//
int tofInit(int bLongRange);


/*
 * driver_ToF.h
 *
 *  Created on: Nov 23, 2022
 *      Author: ludov
 */


int initTof(void);
int TofShutdown(void);
void RB_Delay(int);

#endif /* INC_DRIVER_TOF_H_ */
