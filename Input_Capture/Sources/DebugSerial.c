/*
 * DebugSerial.c
 *
 *  Created on: Feb 22, 2015
 *      Author: Administrador
 */

#include "DebugSerial.h"
#include "RxBuf.h"
#include "AS1.h"

static UART_Desc deviceData;
static uint8 bufferCommand[SIZE_BUFFER_COMMAND];
static uint8 indexCommand=0;
static uint8 commandInterpreted=WAIT_COMMAND;

movementType movement={-1,-1};

void printHex(unsigned char data)
{
	uint8_t dataHex[2];
	
	dataHex[0]=data & 0xf;
	dataHex[1]=(data>>4)& 0xf;
	
	if (dataHex[0]<10)
	{
		dataHex[0]=(unsigned char)(48+dataHex[0]);
	}
	else
	{
		dataHex[0]=(unsigned char)(55+dataHex[0]);
	}
	if (dataHex[1]<10)
	{
		dataHex[1]=(unsigned char)(48+dataHex[1]);
	}
	else
	{
		dataHex[1]=(unsigned char)(55+dataHex[1]);
	}
	debugChar('0');
	debugChar('x');
	debugChar(dataHex[1]);
	debugChar(dataHex[0]);
}

int myAtoi(char *str)
{
    int res = 0;  // Initialize result
    int sign = 1;  // Initialize sign as positive
    int i = 0;  // Initialize index of first digit
     
    // If number is negative, then update sign
    if (str[0] == '-')
    {
        sign = -1; 
        i++;  // Also update index of first digit
    }
     
    // Iterate through all digits and update the result
    for (; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
   
    // Return result with sign
    return sign*res;
}

char* itoaDebug(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
void debugStringGreen(const unsigned char *str)
{
	SendString((unsigned char*)ANSI_COLOR_GREEN, &deviceData);
	SendString(str, &deviceData);
	SendString((unsigned char*)ANSI_COLOR_NO, &deviceData);
}

void debugStringRed(const unsigned char *str)
{
	SendString((unsigned char*)ANSI_COLOR_RED, &deviceData);
	SendString(str, &deviceData);
	SendString((unsigned char*)ANSI_COLOR_NO, &deviceData);
}

void debugStringYellow(const unsigned char *str)
{
	SendString((unsigned char*)ANSI_COLOR_YELLOW, &deviceData);
	SendString(str, &deviceData);
	SendString((unsigned char*)ANSI_COLOR_NO, &deviceData);
}

void debugString(const unsigned char *str)
{
	SendString(str, &deviceData);
}

void debugChar(const unsigned char data)
{
	SendChar(data, &deviceData);
}

static void SendChar(unsigned char ch, UART_Desc *desc) {
  desc->isSent = FALSE;  /* this will be set to 1 once the block has been sent */
  while(AS1_SendBlock(desc->handle, (LDD_TData*)&ch, 1)!=ERR_OK) {} /* Send char */
  while(!desc->isSent) {} /* wait until we get the green flag from the TX interrupt */
}
 
static void SendString(const unsigned char *str,  UART_Desc *desc) {
  while(*str!='\0') {
    SendChar(*str++, desc);
  }
}
 
static void InitSerial(void) {
  /* initialize struct fields */
  deviceData.handle = AS1_Init(&deviceData);
  deviceData.isSent = FALSE;
  deviceData.rxChar = '\0';
  deviceData.rxPutFct = RxBuf_Put;
  /* set up to receive RX into input buffer */
  RxBuf_Init(); /* initialize RX buffer */
  /* Set up ReceiveBlock() with a single byte buffer. We will be called in OnBlockReceived() event. */
  while(AS1_ReceiveBlock(deviceData.handle, (LDD_TData *)&deviceData.rxChar, sizeof(deviceData.rxChar))!=ERR_OK) {} /* initial kick off for receiving data */
}
 
void startSerial()
{
  InitSerial();
  SendString((unsigned char*)"\r\n******************************\r\n", &deviceData);
  SendString((unsigned char*)"\r\nStarting FDRM KL25 PWM   \r\n", &deviceData);
  SendString((unsigned char*)"\r\n******************************\r\n", &deviceData);
  initBufferCommnad();
}

void getDebugString()
{
	if (RxBuf_NofElements()!=0) 
	{
		//SendString((unsigned char*)"echo: ", &deviceData);
		while (RxBuf_NofElements()!=0) 
		{
			unsigned char ch;			
			(void)RxBuf_Get(&ch);
			//SendChar(ch, &deviceData);
		}
		//SendString((unsigned char*)"\r\n", &deviceData);
	}
}
uint8 isNumber(uint8 ch) 
{
	return ((ch>47)&&(ch<58));
}

void initBufferCommnad()
{
	uint16 i=0;
	for(i=0;i<SIZE_BUFFER_COMMAND;i++)
	{
		bufferCommand[i]='\0';
	}
	indexCommand=0;
	commandInterpreted=WAIT_COMMAND;
}

void interpretComment()
{
	switch(commandInterpreted)
	{			
			case(STOP_DIRECTION):
					debugString((unsigned char*)"\r\nDirection received:");
					debugStringRed(bufferCommand);
					movement.direction=myAtoi((char*)bufferCommand);
					break;
			case(STOP_SPEED):
					debugString((unsigned char*)"\r\nSpeed received:");
					debugStringGreen(bufferCommand);
					movement.speed=myAtoi((char*)bufferCommand);
					break;
			//case(STOP_SPEED):
			default:
					debugString((unsigned char*)"\r\nWrong Command!");
					break;
	}
	debugString((unsigned char*)"\r\n");
	indexCommand=0;
	initBufferCommnad();
}

void getCommandReceived()
{
	if (RxBuf_NofElements()!=0) 
	{
		unsigned char ch;			
		while (RxBuf_NofElements()!=0)
		{
			(void)RxBuf_Get(&ch);			
			if(ch==13)//Retorno de carro (CR)
			{
				initBufferCommnad();				
				debugString((unsigned char*)"\r\n");
			}
			else
			{
				if(commandInterpreted==WAIT_COMMAND)
				{
					if (ch==SPEED_COMMAND_START)
					{
						commandInterpreted=START_SPEED;
						indexCommand=0;
					}
					if (ch==DIRECTION_COMMAND_START)
					{
						commandInterpreted=START_DIRECTION;
						indexCommand=0;
					}
				}
				else if((commandInterpreted==START_DIRECTION)||(commandInterpreted==START_SPEED))
				{
					if (ch==SPEED_COMMAND_START)
					{
						commandInterpreted=START_SPEED;
						initBufferCommnad();
					}
					else if (ch==DIRECTION_COMMAND_START)
					{
						commandInterpreted=START_DIRECTION;					
						initBufferCommnad();
					}
					else if (isNumber(ch))
					{
						bufferCommand[indexCommand]=ch;
						indexCommand++;
					}
					else if (ch==SPEED_COMMAND_STOP)
					{
						if(indexCommand>0)
						{
							commandInterpreted=STOP_SPEED;
						}
						else
						{
							commandInterpreted=WAIT_COMMAND;
						}
						interpretComment();
					}
					else if (ch==DIRECTION_COMMAND_STOP)
					{
						if(indexCommand>0)
						{
							commandInterpreted=STOP_DIRECTION;
						}
						else
						{
							commandInterpreted=WAIT_COMMAND;
						}						
						interpretComment();
					}
					else
					{
						commandInterpreted=WAIT_COMMAND;
						interpretComment();
					}
				}
			}//No CR received
		}		
	}
}
