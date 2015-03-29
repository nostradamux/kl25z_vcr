/*
 * General_Definitions.h
 *
 *  Created on: Mar 28, 2015
 *      Author: Administrador
 */

#ifndef GENERAL_DEFINITIONS_H_
#define GENERAL_DEFINITIONS_H_

typedef struct
{
	int32 speed;
	int32 direction;
	int32 correctionSpeed;
	int32 correctionDirection;
}movementType;

/*
 * With this MACRO PWM generation will be done by capturing signals
 * If is not defined, they will be generated depending on value introduced by 
 * console. 
 */
#define CAPTURE_AND_GENERATE_DEVICE 1
#define VERSION	"VCR_V0_1"

extern bool debuggingActivated;

#endif /* GENERAL_DEFINITIONS_H_ */
