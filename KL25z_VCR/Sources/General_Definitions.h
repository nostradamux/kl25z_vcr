/*
 * General_Definitions.h
 *
 *  Created on: Mar 28, 2015
 *      Author: pvarela
 */

#ifndef GENERAL_DEFINITIONS_H_
#define GENERAL_DEFINITIONS_H_

#include "PE_Types.h"

typedef struct
{
	int32 speed;
	int32 direction;
	int32 correctionSpeed;
	int32 correctionDirection;
	uint32 tacho;
}movementType;

#define NRF24L01_IS_SENDER    1  /* 1 if we are the sender, 0 if we are the receiver */
/*
 * With this MACRO PWM generation will be done by capturing signals
 * If is not defined, they will be generated depending on value introduced by 
 * console. 
 */
//#define CAPTURE_AND_GENERATE_DEVICE 1
#ifdef CAPTURE_AND_GENERATE_DEVICE
	#define NRF24L01_IS_SENDER    1  /* 1 if we are the sender, 0 if we are the receiver */
#else
#endif
#define VERSION	"VCR_V1_0"

extern bool debuggingSpeedDirection;
extern bool debuggingTachometer;

#endif /* GENERAL_DEFINITIONS_H_ */
