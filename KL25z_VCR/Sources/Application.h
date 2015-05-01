/*
 * Application.h
 *
 *  Created on: Mar 28, 2015
 *      Author: pvarela
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "General_Definitions.h"
uint32_t Set_New_Speed();
uint32_t Set_New_Direction();
void Apply_Correction_Duty(int32 *valueToCorrect, int32 correction);

#ifndef CAPTURE_AND_GENERATE_DEVICE
	void Init_Tacometer_Simulator();
	void Set_Tacometer_Pulses();
#else
	uint32_t Get_Tacometer_Count();
#endif

#endif /* APPLICATION_H_ */
