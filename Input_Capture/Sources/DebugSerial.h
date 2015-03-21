/*
 * DebugSerial.h
 *
 *  Created on: Feb 22, 2015
 *      Author: Administrador
 */

#ifndef DEBUGSERIAL_H_
#define DEBUGSERIAL_H_

#include "PE_Types.h"
#include "PE_LDD.h"

#define SPEED_COMMAND_START 	'S'
#define SPEED_COMMAND_STOP 		's'
#define DIRECTION_COMMAND_START 'D'
#define DIRECTION_COMMAND_STOP 	'd'
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
	STOP_DIRECTION
};

typedef struct
{
	int32 speed;
	int32 direction;
}movementType;

typedef struct {
  LDD_TDeviceData *handle; /* LDD device handle */
  volatile uint8_t isSent; /* this will be set to 1 once the block has been sent */
  uint8_t rxChar; /* single character buffer for receiving chars */
  uint8_t (*rxPutFct)(uint8_t); /* callback to put received character into buffer */
} UART_Desc;
 
void printHex(unsigned char data);
static void SendChar(unsigned char ch, UART_Desc *desc); 
static void SendString(const unsigned char *str,  UART_Desc *desc);
static void InitSerial(void);
void startSerial(void) ;
void debugString(const unsigned char *str);
char* itoaDebug(int i, char b[]);
void debugChar(const unsigned char data);
void getDebugString(void);
uint8 isNumber(uint8 ch);
void initBufferCommnad(void);
int myAtoi(char *str);
void getCommandReceived();
void debugStringGreen(const unsigned char *str);
void debugStringRed(const unsigned char *str);
void debugStringYellow(const unsigned char *str);

#endif /* DEBUGSERIAL_H_ */
