#include "TU2.h"
#include "DebugSerial.h"
#include "LEDR.h"
#include "LEDB.h"
#include "LEDG.h"
#include "Application.h"
#include "General_Definitions.h"

extern movementType movement;
extern LDD_TDeviceData *SpeedCapture;
extern LDD_TError FlagSpeedCapture;

extern movementType movement;
extern LDD_TDeviceData *DirectionCapture;
extern LDD_TError FlagDirectionCapture;

uint32_t Set_New_Speed()
{
	  char buffer[10];
	  static uint32_t timeHighSpeed=0; //Time in High level
	  static uint32_t timeLowSpeed=0; //Time in Low level
	  uint32_t DataSpeedCapture;	  
	  uint32_t counterSpeedCapture;
	  static uint32_t counterPrint=0;
	  static uint32_t DataSpeedCaptureOld;
	  
	  Speed_Capture_GetCaptureValue(SpeedCapture, &DataSpeedCapture);		  
	  if(DataSpeedCaptureOld>DataSpeedCapture)
	  {
		  counterSpeedCapture=(65535-DataSpeedCaptureOld)+DataSpeedCapture;
	  }
	  else
	  {
		  counterSpeedCapture=DataSpeedCapture-DataSpeedCaptureOld;
	  }
	  DataSpeedCaptureOld=DataSpeedCapture;
	  /*TODO!: coger el nivel del pin para saber si estamos midiendo
	   * el duty o la frecuencia
	   */
	  if(counterSpeedCapture < 25000) //duty never will be less that 50%
	  {
		  timeHighSpeed=counterSpeedCapture*1.53;			  
		  LEDB_Neg(); 
	  }
	  else
	  {
		  timeLowSpeed=counterSpeedCapture*1.53;			  
	  }
	  if(counterPrint++==10)
	  {
		counterPrint=0;
		debugString((unsigned char *)"TimeHigh ");			  
		itoaDebug(timeHighSpeed,&buffer[0]);
		debugStringRed((unsigned char *)buffer);
		debugString((unsigned char *)"us.TimeLow ");			  
		itoaDebug(timeLowSpeed,&buffer[0]);
		debugStringRed((unsigned char *)buffer);
		debugString((unsigned char *)"us.Total ");			  
		itoaDebug(timeLowSpeed+timeHighSpeed,&buffer[0]);
		debugStringRed((unsigned char *)buffer);
		debugString((unsigned char *)"us\n\r");
	  }
	  return timeHighSpeed;
 }

uint32_t Set_New_Direction()
{
	  char buffer[10];
	  static uint32_t timeHighDirection=0; //Time in High level
	  static uint32_t timeLowDirection=0; //Time in Low level
	  uint32_t DataDirectionCapture;	  
	  uint32_t counterDirectionCapture;
	  static uint32_t counterPrint=0;
	  static uint32_t DataDirectionCaptureOld;
	  
	  Direction_Capture_GetCaptureValue(DirectionCapture, &DataDirectionCapture);		  
	  if(DataDirectionCaptureOld>DataDirectionCapture)
	  {
		  counterDirectionCapture=(65535-DataDirectionCaptureOld)+DataDirectionCapture;
	  }
	  else
	  {
		  counterDirectionCapture=DataDirectionCapture-DataDirectionCaptureOld;
	  }
	  DataDirectionCaptureOld=DataDirectionCapture;
	  /*TODO!: coger el nivel del pin para saber si estamos midiendo
	   * el duty o la frecuencia
	   */
	  if(counterDirectionCapture < 25000) //duty never will be less that 50%
	  {
		  timeHighDirection=counterDirectionCapture*1.53;			  
		  LEDB_Neg(); 
	  }
	  else
	  {
		  timeLowDirection=counterDirectionCapture*1.53;			  
	  }
	  if(counterPrint++==10)
	  {
		counterPrint=0;
		debugString((unsigned char *)"TimeHigh ");			  
		itoaDebug(timeHighDirection,&buffer[0]);
		debugStringRed((unsigned char *)buffer);
		debugString((unsigned char *)"us.TimeLow ");			  
		itoaDebug(timeLowDirection,&buffer[0]);
		debugStringRed((unsigned char *)buffer);
		debugString((unsigned char *)"us.Total ");			  
		itoaDebug(timeLowDirection+timeHighDirection,&buffer[0]);
		debugStringRed((unsigned char *)buffer);
		debugString((unsigned char *)"us\n\r");
	  }
	  return timeHighDirection;
 }		 
