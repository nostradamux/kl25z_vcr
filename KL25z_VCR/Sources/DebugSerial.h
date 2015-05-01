/*
 * DebugSerial.h
 *
 *  Created on: Feb 22, 2015
 *      Author: pvarela
 */

#ifndef DEBUGSERIAL_H_
#define DEBUGSERIAL_H_

#include "PE_Types.h"
#include "PE_LDD.h"
#include "General_Definitions.h"

#define SPEED_COMMAND_START 		'S'
#define SPEED_COMMAND_STOP 			's'
#define DIRECTION_COMMAND_START 	'D'
#define DIRECTION_COMMAND_STOP 		'd'
#define TACHOMETER_COMMAND_START	'T'
#define TACHOMETER_COMMAND_STOP		't'

#define CORRECTION_SPEED_COMMAND_START 		'S'
#define CORRECTION_SPEED_COMMAND_STOP 		's'
#define CORRECTION_DIRECTION_COMMAND_START 	'D'
#define CORRECTION_DIRECTION_COMMAND_STOP 	'd'

#define SIZE_BUFFER_COMMAND		20

#define ANSI_COLOR_NO  		"\033[0m"
#define ANSI_COLOR_RED 	 	"\033[31m"
#define ANSI_COLOR_GREEN 	"\033[32m"
#define ANSI_COLOR_YELLOW	"\033[33m"

enum
{
	WAIT_COMMAND,
	START_SPEED,
	STOP_SPEED,
	START_DIRECTION,
	STOP_DIRECTION,
	START_TACHOMETER,
	STOP_TACHOMETER,
	CORRECTION_START_SPEED,
	CORRECTION_STOP_SPEED,
	CORRECTION_START_DIRECTION,
	CORRECTION_STOP_DIRECTION
};

typedef struct {
  LDD_TDeviceData *handle; /* LDD device handle */
  volatile uint8_t isSent; /* this will be set to 1 once the block has been sent */
  uint8_t rxChar; /* single character buffer for receiving chars */
  uint8_t (*rxPutFct)(uint8_t); /* callback to put received character into buffer */
} UART_Desc;
 
void Print_Hex(unsigned char data);
void Send_Char(unsigned char ch, UART_Desc *desc); 
void Send_String(const unsigned char *str,  UART_Desc *desc);
void Init_Serial(void);
void Start_Serial(void) ;
void Debug_String(const unsigned char *str);
char* Itoa_Debug(int i, char b[]);
void Debug_Char(const unsigned char data);
void Get_Debug_String(void);
uint8 Is_Number(uint8 ch);
void Init_Buffer_Commnad(void);
int My_Atoi(char *str);
void Get_Command_Received();
void Debug_String_Green(const unsigned char *str);
void Debug_String_Red(const unsigned char *str);
void Debug_String_Yellow(const unsigned char *str);
void Interpret_Command();
void Init_Buffer_Commnad();

#endif /* DEBUGSERIAL_H_ */
