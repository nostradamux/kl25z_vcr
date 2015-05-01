/*
 * Application.c
 *
 *      Author: Erich Styger
 */
#include "nRF24L01_application.h"

static const uint8_t TADDR[5] = {0x11, 0x22, 0x33, 0x44, 0x55}; /* device address */
static const uint8_t RADDR[5] = {0x11, 0x22, 0x33, 0x44, 0x55}; /* device address */
static uint8_t payload[PAYLOAD_SIZE]; /* buffer for payload */
static volatile bool isrFlagNRF24L01; /* flag set by ISR */
extern const unsigned char* regRF24_names[24];

void NRF24L01_OnInterrupt(void) 
{
  CE_ClrVal(); /* stop sending/listening */
  isrFlagNRF24L01 = TRUE;
}

void NRF24L01_Print_All_Registers()
{
	uint8_t i;

	Debug_String((unsigned char*)"RF24 registers:\n\r");
	for (i=0;i<24;i++)
	{
		Debug_String(regRF24_names[i]);
		Debug_String((unsigned char*)"\t\t:");
		Print_Hex(RF_ReadRegister(i));
		Debug_String((unsigned char*)"\n\r");		
	}
}

void NRF24L01_Init()
{		  
	WAIT1_Waitms(200); /* give device time to power up */
	RF_Init(); /* set CE and CSN to initialization value */
	
	RF_WriteRegister(RF24_RF_SETUP, RF24_RF_SETUP_RF_PWR_0|RF24_RF_SETUP_RF_DR_2000);
	RF_WriteRegister(RF24_RX_PW_P0, PAYLOAD_SIZE); /* number of payload bytes we want to send and receive */
	RF_WriteRegister(RF24_RF_CH, CHANNEL_NO); /* set channel */
	
	/* Set RADDR and TADDR as the transmit address since we also enable auto acknowledgment */
	RF_WriteRegisterData(RF24_RX_ADDR_P0, (uint8_t*)TADDR, sizeof(TADDR));
	RF_WriteRegisterData(RF24_TX_ADDR, (uint8_t*)TADDR, sizeof(TADDR));
	
	/* Enable RX_ADDR_P1 address matching */
	RF_WriteRegister(RF24_EN_RXADDR, RF24_EN_RXADDR_ERX_P0); /* enable data pipe 0 */
	
	/* clear interrupt flags */
	RF_ResetStatusIRQ(RF24_STATUS_RX_DR|RF24_STATUS_TX_DS|RF24_STATUS_MAX_RT);
	//RF_ResetStatusIRQ(RF24_STATUS_TX_DS);	
	
	#ifdef NRF24L01_IS_SENDER
	  RF_WriteRegister(RF24_EN_AA, RF24_EN_AA_ENAA_P0); /* enable auto acknowledge. RX_ADDR_P1 needs to be equal to TX_ADDR! */
	  RF_WriteRegister(RF24_SETUP_RETR, RF24_SETUP_RETR_ARD_750|RF24_SETUP_RETR_ARC_15); /* Important: need 750 us delay between every retry */
	  TX_POWERDOWN();
	  WAIT1_Waitms(200); /* give device time to power up */
	  TX_POWERUP();  /* Power up in transmitting mode */
	  CE_ClrVal();   /* Will pulse this later to send data */
	#else
	  RX_POWERUP();  /* Power up in receiving mode */
	  CE_SetVal();   /* Listening for packets */
	#endif
}

void NRF24L01_Set_Buffer_To_Send(uint8_t* buffer,uint8_t size)
{
	uint8_t i=0;
	uint8_t j=0;
	uint8_t* bufferTemp=buffer;
	
	for(i=0;i<PAYLOAD_SIZE;i++) 
	{
		payload[i]=i;
		/*if(j<size)
		{
			j++;
			payload[i]=*bufferTemp++;
		}
		else
		{
			payload[i] = 0; // just fill payload with some data 
		}
		 */
	}
}

void NRF24L01_Receive(void) 
{
	static int cntr = 0;
	uint8_t status=0;
	uint32_t i=0;	
	
	if (isrFlagNRF24L01) 
    { 
    	/* interrupt? */
    	isrFlagNRF24L01 = FALSE; /* reset interrupt flag */
		cntr = 0; /* reset counter */
		LEDB_Off();
		LEDG_Neg(); /* blink green LED to indicate good communication */
		status = RF_GetStatus();
		if (status&RF24_STATUS_RX_DR) 
		{ 
			/* data received interrupt */
			RF_RxPayload(payload, sizeof(payload)); /* will reset RX_DR bit */
			RF_ResetStatusIRQ(RF24_STATUS_RX_DR|RF24_STATUS_TX_DS|RF24_STATUS_MAX_RT); /* make sure we reset all flags. Need to have the pipe number too */
			Debug_String((unsigned char*)"Received data!:");
			Debug_String((unsigned char*)"\n\r");
			for (i=0;i< sizeof(payload);i++)
			{
				Print_Hex((unsigned char)payload[i]);
				Debug_String((unsigned char*)"\n\r");
			}				
		}
		if (status&RF24_STATUS_TX_DS) 
		{ 
			RF_ResetStatusIRQ(RF24_STATUS_TX_DS); /* clear bit */
		}
		if (status&RF24_STATUS_MAX_RT) 
		{ 
			/* retry timeout interrupt */
			RF_ResetStatusIRQ(RF24_STATUS_MAX_RT); /* clear bit */
		}
	} 
    else 
    {
		cntr++;
		if (cntr>500) 
		{ 
			/* blink every 500 ms if not receiving data */
			cntr = 0; /* reset counter */
			LEDG_Off();
			LEDB_Neg(); /* blink blue to indicate no communication */
		}
    }
}

void NRF24L01_Send(void) 
{ 
	static int cntr = 0;
	uint8_t status=0;
		
	cntr++;
	if (cntr==200) 
	{ 
		/* send data every 200 ms */
		cntr = 0;		
		LEDG_On();
		RF_TxPayload(payload, sizeof(payload)); /* send data */
	}   
	if (cntr==100) 
	{ 
		/* not able to send data for 50 ms? */
		LEDG_Off();
		LEDR_On();
	}
	if (isrFlagNRF24L01) 
	{ 
		/* check if we have received an interrupt */
		LEDR_Off(); /* indicate data has been sent */
		isrFlagNRF24L01 = FALSE; /* reset interrupt flag */
		status = RF_GetStatus();
		if (status&RF24_STATUS_RX_DR) 
		{ 
			/* data received interrupt */
			RF_ResetStatusIRQ(RF24_STATUS_RX_DR); /* clear bit */
		}
		if (status&RF24_STATUS_TX_DS) 
		{ 
			/* data sent interrupt */
			cntr = 0; /* reset timeout counter */
			//LEDR_Off(); /* indicate data has been sent */
			RF_ResetStatusIRQ(RF24_STATUS_TX_DS); /* clear bit */
			LEDG_Off();
		}
		if (status&RF24_STATUS_MAX_RT) 
		{ 
			/* retry timeout interrupt */
			RF_ResetStatusIRQ(RF24_STATUS_MAX_RT); /* clear bit */
		}
	}
	//WAIT1_Waitms(1);
}

