#include "TU2.h"
#include "DebugSerial.h"
#include "LEDR.h"
#include "LEDB.h"
#include "LEDG.h"
#include "Application.h"
#include "Tacometer_Simulator.h"

extern movementType movement;
extern LDD_TDeviceData *SpeedCapture;
extern LDD_TError FlagSpeedCapture;

extern movementType movement;
extern LDD_TDeviceData *DirectionCapture;
extern LDD_TError FlagDirectionCapture;

extern LDD_TDeviceData *TachometerCapture;
extern LDD_TError ErrorTachometerCapture, FlagTachometerCapture;
extern volatile uint32_t DataTachometerCapture;

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
	  if(debuggingActivated)
	  {	  
		  if(counterPrint++==20)
	  	  {
			counterPrint=0;
			Debug_String((unsigned char *)"Speed TimeHigh ");			  
			Itoa_Debug(timeHighSpeed,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String((unsigned char *)"us.TimeLow ");			  
			Itoa_Debug(timeLowSpeed,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String((unsigned char *)"us.Total ");			  
			Itoa_Debug(timeLowSpeed+timeHighSpeed,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String((unsigned char *)"us\n\r");
		  }
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
	  if(debuggingActivated)
	  {	  
	 	  if(counterPrint++==20)
		  {
			counterPrint=0;
			Debug_String((unsigned char *)"Direction TimeHigh ");			  
			Itoa_Debug(timeHighDirection,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String((unsigned char *)"us.TimeLow ");			  
			Itoa_Debug(timeLowDirection,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String((unsigned char *)"us.Total ");			  
			Itoa_Debug(timeLowDirection+timeHighDirection,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String((unsigned char *)"us\n\r");
		  }
	  }
	  return timeHighDirection;
 }	

void Apply_Correction_Duty(int32 *valueToCorrect, int32 correction)
{
	if ((*valueToCorrect+correction)>0)
	{
		*valueToCorrect+=correction;
	}
	else
	{
		*valueToCorrect=0;
	}
}

#ifndef CAPTURE_AND_GENERATE_DEVICE
void Init_Tacometer_Simulator()
{
	tacometer=Tacometer_Simulator_Init(NULL);
}
void Set_Tacometer_Pulses()
{
	char buffer[10];
	static uint16_t periodTacometerOld=0;
	
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
		  
		if (movement.tacho!=periodTacometerOld)
		{
			if (movement.tacho>Tacometer_Simulator_SPMS_MAX)
			{
				Debug_String((unsigned char *)"Wrong period selected!\n\r");
				movement.tacho=Tacometer_Simulator_SPMS_MAX;		
			}
			if (movement.tacho<Tacometer_Simulator_SPMS_MIN)
			{
				Debug_String((unsigned char *)"Wrong period selected!\n\r");
				movement.tacho=Tacometer_Simulator_SPMS_MIN;		
			}
			periodTacometerOld=movement.tacho;
			Debug_String((unsigned char *)"Tachometer period changed to:");	
			Itoa_Debug((int)movement.tacho,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String_Red((unsigned char *)"\n\r");
			
			Tacometer_Simulator_SetPeriodMS(tacometer, movement.tacho);
			Tacometer_Simulator_SetRatio16(tacometer, 0xFFFF/2);
		}
}
#else
uint32_t Get_Tacometer_Count()
{
	  char buffer[10];
	  uint32_t DataTachometerCapture;	  
	  uint32_t counterTachometerCapture;
	  static uint32_t counterPrint=0;
	  static uint32_t DataTachometerCaptureOld;
		
	  Tacometer_Capture_GetCaptureValue(TachometerCapture, &DataTachometerCapture);		  
	  if(DataTachometerCaptureOld>DataTachometerCapture)
	  {
		  counterTachometerCapture=(65535-DataTachometerCaptureOld)+DataTachometerCapture;
	  }
	  else
	  {
		  counterTachometerCapture=DataTachometerCapture-DataTachometerCaptureOld;
	  }
	  
		  
	  //if(debuggingActivated)
	  {	  
		  if(counterPrint++==10)
		  {
			counterPrint=0;
			//Debug_String((unsigned char *)"Tachometer counts: ");
			counterTachometerCapture/=327;
			Debug_String((unsigned char *)"Tachometer: ");
			Itoa_Debug(counterTachometerCapture,&buffer[0]);
			Debug_String_Red((unsigned char *)buffer);
			Debug_String((unsigned char *)" ms\n\r");
		  }
	  }
	  DataTachometerCaptureOld=DataTachometerCapture;
	  return counterTachometerCapture;
}
#endif
