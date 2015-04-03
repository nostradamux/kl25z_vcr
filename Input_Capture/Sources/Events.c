/* ###################################################################
**     Filename    : Events.c
**     Project     : Freedom_LED
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-01-26, 07:12, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "DebugSerial.h"

extern LDD_TDeviceData *SpeedCapture;
extern LDD_TError ErrorSpeedCapture, FlagSpeedCapture;
extern volatile uint32_t DataSpeedCapture;

extern LDD_TDeviceData *DirectionCapture;
extern LDD_TError ErrorDirectionCapture, FlagDirectionCapture;
extern volatile uint32_t DataDirectionCapture;

extern LDD_TDeviceData *TachometerCapture;
extern LDD_TError ErrorTachometerCapture, FlagTachometerCapture;
extern volatile uint32_t DataTachometerCapture;

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnBlockReceived (module Events)
**
**     Component   :  AS1 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void AS1_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	
	UART_Desc *ptr = (UART_Desc*)UserDataPtr;
	
	(void)ptr->rxPutFct(ptr->rxChar); /* but received character into buffer */
	(void)AS1_ReceiveBlock(ptr->handle, (LDD_TData *)&ptr->rxChar, sizeof(ptr->rxChar));
}

/*
** ===================================================================
**     Event       :  AS1_OnBlockSent (module Events)
**
**     Component   :  AS1 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void AS1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	UART_Desc *ptr = (UART_Desc*)UserDataPtr;	
	ptr->isSent = TRUE; /* set flag so sender knows we have finished */
}

/*
** ===================================================================
**     Event       :  Speed_Capture_OnCapture (module Events)
**
**     Component   :  Speed_Capture [Capture_LDD]
*/
/*!
**     @brief
**         This event is called on capturing of counter actual value.
**         Component and OnCapture event must be enabled. See
**         [SetEventMask] and [GetEventMask] methods. This event is
**         available only if a [Interrupt service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void Speed_Capture_OnCapture(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	FlagSpeedCapture = Speed_Capture_GetCaptureValue(SpeedCapture, &DataSpeedCapture);	
}
/*
** ===================================================================
**     Event       :  CheckPoint_OnBlockReceived (module Events)
**
**     Component   :  CheckPoint [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void CheckPoint_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  CheckPoint_OnBlockSent (module Events)
**
**     Component   :  CheckPoint [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void CheckPoint_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  Tacometer_Capture_OnCapture (module Events)
**
**     Component   :  Tacometer_Capture [Capture_LDD]
*/
/*!
**     @brief
**         This event is called on capturing of counter actual value.
**         Component and OnCapture event must be enabled. See
**         [SetEventMask] and [GetEventMask] methods. This event is
**         available only if a [Interrupt service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void Tacometer_Capture_OnCapture(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
  FlagTachometerCapture = Tacometer_Capture_GetCaptureValue(TachometerCapture, &DataTachometerCapture);	
}

/*
** ===================================================================
**     Event       :  Direction_Capture_OnCapture (module Events)
**
**     Component   :  Direction_Capture [Capture_LDD]
*/
/*!
**     @brief
**         This event is called on capturing of counter actual value.
**         Component and OnCapture event must be enabled. See
**         [SetEventMask] and [GetEventMask] methods. This event is
**         available only if a [Interrupt service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void Direction_Capture_OnCapture(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	FlagDirectionCapture = Direction_Capture_GetCaptureValue(DirectionCapture, &DataDirectionCapture);	
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
