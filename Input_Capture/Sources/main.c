/* ###################################################################
**     Filename    : main.c
**     Project     : Freedom_LED
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-01-26, 07:12, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "LEDR.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "LEDG.h"
#include "LEDpin2.h"
#include "BitIoLdd2.h"
#include "LEDB.h"
#include "LEDpin3.h"
#include "BitIoLdd3.h"
#include "WAIT1.h"
#include "TU1.h"
#include "Speed.h"
#include "PwmLdd1.h"
#include "Direction.h"
#include "PwmLdd2.h"
#include "RxBuf.h"
#include "AS1.h"
#include "Speed_Capture.h"
#include "Direction_Capture.h"
#include "Tacometer_Capture.h"
#include "TU2.h"
#include "CheckPoint.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "General_Definitions.h"
#include "DebugSerial.h"
#include "Application.h"

extern movementType movement;
LDD_TDeviceData *SpeedCapture;
LDD_TError ErrorSpeedCapture, FlagSpeedCapture;
volatile uint32_t DataSpeedCapture;
uint32_t DataSpeedCaptureOld;

LDD_TDeviceData *DirectionCapture;
LDD_TError ErrorDirectionCapture, FlagDirectionCapture;
volatile uint32_t DataDirectionCapture;
uint32_t DataDirectionCaptureOld;

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  uint32_t dutySpeed_old=1000;
  uint32_t dutyDirection_old=1000;
  movement.direction=dutyDirection_old;
  movement.speed=dutySpeed_old;  
  
  char buffer[10];
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  SpeedCapture = Speed_Capture_Init((LDD_TUserData *)NULL);  
  startSerial();
  
  ErrorSpeedCapture  = Speed_Capture_Reset(SpeedCapture);                       /* Reset the counter */
  FlagSpeedCapture  = 1U;

  while (1)
  {
	  #ifdef CAPTURE_AND_GENERATE_DEVICE
		  if(FlagSpeedCapture == ERR_OK)
		  {			  
			  movement.speed=Set_New_Speed();
			  FlagSpeedCapture=1;
		  }
		  if(FlagDirectionCapture == ERR_OK)
		  {
			  movement.speed=Set_New_Speed();
			  FlagDirectionCapture=1;
		  }
//		  		  {
//		  			  FlagDirectionCapture=1;
//		  			  Direction_Capture_GetCaptureValue(DirectionCapture, &DataDirectionCapture);		  
//		  			  if(DataDirectionCaptureOld>DataDirectionCapture)
//		  			  {
//		  				  counterDirectionCapture=(65535-DataDirectionCaptureOld)+DataDirectionCapture;
//		  			  }
//		  			  else
//		  			  {
//		  				  counterDirectionCapture=DataDirectionCapture-DataDirectionCaptureOld;
//		  			  }
//		  			  DataDirectionCaptureOld=DataDirectionCapture;
//		  			  /*TODO!: coger el nivel del pin para saber si estamos midiendo
//		  			   * el duty o la frecuencia
//		  			   */
//		  			  if(counterDirectionCapture < 25000) //duty never will be less that 50%
//		  			  {
//		  				  timeHighDirection=counterDirectionCapture*1.53;			  
//		  				  LEDB_Neg(); 
//		  			  }
//		  			  else
//		  			  {
//		  				  timeLowDirection=counterDirectionCapture*1.53;			  
//		  			  }
//		  			  if(counterPrint++==10)
//		  			  {
//		  				counterPrint=0;
//		  				debugString((unsigned char *)"TimeHigh ");			  
//		  				itoaDebug(timeHighDirection,&buffer[0]);
//		  				debugStringRed((unsigned char *)buffer);
//		  				debugString((unsigned char *)"us.TimeLow ");			  
//		  				itoaDebug(timeLowDirection,&buffer[0]);
//		  				debugStringRed((unsigned char *)buffer);
//		  				debugString((unsigned char *)"us.Total ");			  
//		  				itoaDebug(timeLowDirection+timeHighDirection,&buffer[0]);
//		  				debugStringRed((unsigned char *)buffer);
//		  				debugString((unsigned char *)"us\n\r");
//		  			  }
//		  			  movement.Direction=timeHighDirection;
//		  		  }		  	  
//		  		  if(movement.Direction!=dutyDirection_old)
//		  		  {
//		  			  dutyDirection_old=movement.Direction;
//		  			  Direction_SetDutyUS(50000-dutyDirection_old);
//		  			  
//		  			  debugString((unsigned char *)"New duty Direction:");
//		  			  itoaDebug(movement.Direction,&buffer[0]);
//		  			  debugStringGreen((unsigned char *)buffer);
//		  			  debugString((unsigned char *)"\n\r");
//		  		  }
//		  		  if(movement.direction!=dutyDirection_old)
//		  		  {
//		  			  dutyDirection_old=movement.direction;
//		  			  Direction_SetDutyUS(50000-dutyDirection_old);
//		  			  
//		  			  debugString((unsigned char *)"New duty direction:");
//		  			  itoaDebug(movement.direction,buffer);
//		  			  debugStringRed((unsigned char *)buffer);
//		  			  debugString((unsigned char *)"\n\r");
//		  		  }
//  	  	  }
		  if(movement.speed!=dutySpeed_old)
		  {
			  dutySpeed_old=movement.speed;
			  Speed_SetDutyUS(50000-dutySpeed_old);
			  
			  debugString((unsigned char *)"New duty speed:");
			  itoaDebug(movement.speed,&buffer[0]);
			  debugStringGreen((unsigned char *)buffer);
			  debugString((unsigned char *)"\n\r");
		  }
		  if(movement.direction!=dutyDirection_old)
		  {
			  dutyDirection_old=movement.direction;
			  Direction_SetDutyUS(50000-dutyDirection_old);
			  
			  debugString((unsigned char *)"New duty direction:");
			  itoaDebug(movement.direction,buffer);
			  debugStringRed((unsigned char *)buffer);
			  debugString((unsigned char *)"\n\r");
		  }
	  #else //GENERATE PWM from CONSOLE
		  getCommandReceived();
		  
		  if(movement.speed!=dutySpeed_old)
		  {
			  dutySpeed_old=movement.speed;
			  Speed_SetDutyUS(50000-dutySpeed_old);
			  
			  debugString((unsigned char *)"New duty speed:");
			  itoaDebug(movement.speed,&buffer[0]);
			  debugStringGreen((unsigned char *)buffer);
			  debugString((unsigned char *)"\n\r");
		  }
		  if(movement.direction!=dutyDirection_old)
		  {
			  dutyDirection_old=movement.direction;
			  Direction_SetDutyUS(50000-dutyDirection_old);
			  
			  debugString((unsigned char *)"New duty direction:");
			  itoaDebug(movement.direction,buffer);
			  debugStringRed((unsigned char *)buffer);
			  debugString((unsigned char *)"\n\r");
		  }
		  WAIT1_Waitms(50);
		  LEDG_Neg();
	 #endif
	  	   
	  
  }
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
