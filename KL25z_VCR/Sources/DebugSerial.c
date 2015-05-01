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

movementType movement={1000,1000,0,0,0};

void Print_Hex(unsigned char data)
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
	Debug_Char('0');
	Debug_Char('x');
	Debug_Char(dataHex[1]);
	Debug_Char(dataHex[0]);
}

int My_Atoi(char *str)
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

char* Itoa_Debug(int i, char b[]){
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
void Debug_String_Green(const unsigned char *str)
{
	Send_String((unsigned char*)ANSI_COLOR_GREEN, &deviceData);
	Send_String(str, &deviceData);
	Send_String((unsigned char*)ANSI_COLOR_NO, &deviceData);
}

void Debug_String_Red(const unsigned char *str)
{
	Send_String((unsigned char*)ANSI_COLOR_RED, &deviceData);
	Send_String(str, &deviceData);
	Send_String((unsigned char*)ANSI_COLOR_NO, &deviceData);
}

void Debug_String_Yellow(const unsigned char *str)
{
	Send_String((unsigned char*)ANSI_COLOR_YELLOW, &deviceData);
	Send_String(str, &deviceData);
	Send_String((unsigned char*)ANSI_COLOR_NO, &deviceData);
}

void Debug_String(const unsigned char *str)
{
	Send_String(str, &deviceData);
}

void Debug_Char(const unsigned char data)
{
	Send_Char(data, &deviceData);
}

void Send_Char(unsigned char ch, UART_Desc *desc) {
  desc->isSent = FALSE;  /* this will be set to 1 once the block has been sent */
  while(AS1_SendBlock(desc->handle, (LDD_TData*)&ch, 1)!=ERR_OK) {} /* Send char */
  while(!desc->isSent) {} /* wait until we get the green flag from the TX interrupt */
}
 
void Send_String(const unsigned char *str,  UART_Desc *desc) {
  while(*str!='\0') {
    Send_Char(*str++, desc);
  }
}
 
void Init_Serial(void) {
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
 
void Start_Serial()
{
	Init_Serial();
	
	Debug_String_Yellow((unsigned char*)"\r\n*******************************************************\r\n");
	Debug_String_Yellow((unsigned char*)"                     ");
	Debug_String_Yellow((unsigned char*)VERSION);
	#ifdef CAPTURE_AND_GENERATE_DEVICE
		Debug_String_Yellow((unsigned char*)"\r\n     Starting FDRM KL25 GENERATE FROM INPUT CAPTURE   \r\n");
	#else
		Debug_String_Yellow((unsigned char*)"\r\n     Starting FDRM KL25 GENERATE FROM CONSOLE COMMANDS   \r\n");
	#endif
		Debug_String_Yellow((unsigned char*)"\r\n*******************************************************\r\n");
	
	#ifdef CAPTURE_AND_GENERATE_DEVICE
		Debug_String((unsigned char*)"\r\nWrite Sxxxs (xxx=number) to correct Speed (us Duty). \r\nWrite Dxxxd (xxx=number) to correct Direction (us Duty) \r\n");
		Debug_String((unsigned char*)"Write Txxxt (xxx=number) to see Tachometer capture\r\n");
		Debug_String((unsigned char*)"Activate/Deactivate Speed and Direction Debugging info pressing 'I' or 'i'. \r\n");
		Debug_String((unsigned char*)"By default are deactivated \r\n\r\n\r\n");
	#else
		char buffer[10];
		Debug_String((unsigned char*)"\r\nWrite Sxxxs (xxx=number) to change Speed (us Duty). Output Pin PTA13 ");
		Debug_String((unsigned char*)"\r\nWrite Dxxxd (xxx=number) to change Direction (us Duty). Output Pin PTA12 ");
		Debug_String((unsigned char*)"\r\nWrite Txxxt (xxx=number) to change Tachometer (ms Period, range [");
		Itoa_Debug((int)Tacometer_Simulator_SPMS_MIN,&buffer[0]);
		Debug_String((unsigned char *)buffer);
		Debug_String((unsigned char*)",");
		Itoa_Debug((int)Tacometer_Simulator_SPMS_MAX,&buffer[0]);
		Debug_String((unsigned char *)buffer);
		Debug_String((unsigned char*)"]). Output Pin PTB3 \r\n\r\n\r\n");
	#endif
	 Init_Buffer_Commnad();
}

void Get_Debug_String()
{
	if (RxBuf_NofElements()!=0) 
	{
		//SendString((unsigned char*)"echo: ", &deviceData);
		while (RxBuf_NofElements()!=0) 
		{
			unsigned char ch;			
			(void)RxBuf_Get(&ch);
		}
	}
}
uint8 Is_Number(uint8 ch) 
{
	return ((ch>47)&&(ch<58));
}

void Init_Buffer_Commnad()
{
	uint16 i=0;
	for(i=0;i<SIZE_BUFFER_COMMAND;i++)
	{
		bufferCommand[i]='\0';
	}
	indexCommand=0;
	commandInterpreted=WAIT_COMMAND;
}

void Interpret_Command()
{
	switch(commandInterpreted)
	{			
			#ifdef CAPTURE_AND_GENERATE_DEVICE
				case(CORRECTION_STOP_DIRECTION):
					Debug_String((unsigned char*)"\r\nDirection correction:");
					Debug_String_Red(bufferCommand);
					movement.correctionDirection=My_Atoi((char*)bufferCommand);
					break;
				case(CORRECTION_STOP_SPEED):
					Debug_String((unsigned char*)"\r\nSpeed correction:");
					Debug_String_Green(bufferCommand);
					movement.correctionSpeed=My_Atoi((char*)bufferCommand);
					break;							
			#else
				case(STOP_DIRECTION):
					Debug_String((unsigned char*)"\r\nDirection received:");
					Debug_String_Red(bufferCommand);
					movement.direction=My_Atoi((char*)bufferCommand);
					break;
				case(STOP_SPEED):
					Debug_String((unsigned char*)"\r\nSpeed received:");
					Debug_String_Green(bufferCommand);
					movement.speed=My_Atoi((char*)bufferCommand);
					break;
				case(STOP_TACHOMETER):
					Debug_String((unsigned char*)"\r\nTachometer received:");
					Debug_String_Green(bufferCommand);
					movement.tacho=My_Atoi((char*)bufferCommand);
					break;
			#endif
			default:
					Debug_String((unsigned char*)"\r\nWrong Command!");
					break;
	}
	Debug_String((unsigned char*)"\r\n");
	indexCommand=0;
	Init_Buffer_Commnad();
}

#ifdef CAPTURE_AND_GENERATE_DEVICE
void Get_Command_Received()
{
	if (RxBuf_NofElements()!=0) 
	{
		unsigned char ch;			
		while (RxBuf_NofElements()!=0)
		{
			(void)RxBuf_Get(&ch);			
			if(ch==13)//Retorno de carro (CR)
			{
				Init_Buffer_Commnad();				
				Debug_String((unsigned char*)"\r\n");
			}
			else if ((ch=='I')||(ch=='i'))
			{
				if(debuggingSpeedDirection)
				{
					debuggingSpeedDirection=FALSE;
					Send_String((unsigned char*)"\n\rDebugging Speed & Direction deactivated\n\r", &deviceData);
				}
				else
				{
					debuggingSpeedDirection=TRUE;
					Send_String((unsigned char*)"\n\rDebugging Speed & Direction activated\n\r", &deviceData);
				}
			}
			else if ((ch=='T')||(ch=='t'))
			{
				if(debuggingTachometer)
				{
					debuggingTachometer=FALSE;
					Send_String((unsigned char*)"\n\rDebugging Tachometer deactivated\n\r", &deviceData);
				}
				else
				{
					debuggingTachometer=TRUE;
					Send_String((unsigned char*)"\n\rDebugging Tachometer activated\n\r", &deviceData);
				}
			}
			else
			{				
				if(commandInterpreted==WAIT_COMMAND)
				{
					if (ch==CORRECTION_SPEED_COMMAND_START)
					{
						commandInterpreted=CORRECTION_START_SPEED;
						indexCommand=0;
					}
					if (ch==CORRECTION_DIRECTION_COMMAND_START)
					{
						commandInterpreted=CORRECTION_START_DIRECTION;
						indexCommand=0;
					}
				}
				else if((commandInterpreted==CORRECTION_START_DIRECTION)||(commandInterpreted==CORRECTION_START_SPEED))
				{
					if (ch==CORRECTION_SPEED_COMMAND_START)
					{
						commandInterpreted=CORRECTION_START_SPEED;
						Init_Buffer_Commnad();
					}
					else if (ch==CORRECTION_DIRECTION_COMMAND_START)
					{
						commandInterpreted=CORRECTION_START_DIRECTION;					
						Init_Buffer_Commnad();
					}
					else if (ch=='-')
					{
						bufferCommand[indexCommand]=ch;
						indexCommand++;
					}
					else if (Is_Number(ch))
					{
						bufferCommand[indexCommand]=ch;
						indexCommand++;
					}
					else if (ch==CORRECTION_SPEED_COMMAND_STOP)
					{
						if(indexCommand>0)
						{
							commandInterpreted=CORRECTION_STOP_SPEED;
						}
						else
						{
							commandInterpreted=WAIT_COMMAND;
						}
						Interpret_Command();
					}
					else if (ch==CORRECTION_DIRECTION_COMMAND_STOP)
					{
						if(indexCommand>0)
						{
							commandInterpreted=CORRECTION_STOP_DIRECTION;
						}
						else
						{
							commandInterpreted=WAIT_COMMAND;
						}						
						Interpret_Command();
					}
					else
					{
						commandInterpreted=WAIT_COMMAND;
						Interpret_Command();
					}
				}
			}//No CR received
		}		
	}
}
#else
void Get_Command_Received()
{
	if (RxBuf_NofElements()!=0) 
	{
		unsigned char ch;			
		while (RxBuf_NofElements()!=0)
		{
			(void)RxBuf_Get(&ch);			
			if(ch==13)//Retorno de carro (CR)
			{
				Init_Buffer_Commnad();				
				Debug_String((unsigned char*)"\r\n");
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
					if (ch==TACHOMETER_COMMAND_START)
					{
						commandInterpreted=START_DIRECTION;
						indexCommand=0;
					}
				}
				else if((commandInterpreted==START_DIRECTION)||(commandInterpreted==START_SPEED)
						||(commandInterpreted==START_TACHOMETER))
				{
					if (ch==SPEED_COMMAND_START)
					{
						commandInterpreted=START_SPEED;
						Init_Buffer_Commnad();
					}
					else if (ch==DIRECTION_COMMAND_START)
					{
						commandInterpreted=START_DIRECTION;					
						Init_Buffer_Commnad();
					}
					else if (ch==TACHOMETER_COMMAND_START)
					{
						commandInterpreted=START_TACHOMETER;					
						Init_Buffer_Commnad();
					}
					else if (Is_Number(ch))
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
						Interpret_Command();
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
						Interpret_Command();
					}
					else if (ch==TACHOMETER_COMMAND_STOP)
					{
						if(indexCommand>0)
						{
							commandInterpreted=STOP_TACHOMETER;
						}
						else
						{
							commandInterpreted=WAIT_COMMAND;
						}						
						Interpret_Command();
					}
					else
					{
						commandInterpreted=WAIT_COMMAND;
						Interpret_Command();
					}
				}
			}//No CR received
		}		
	}
}
#endif

