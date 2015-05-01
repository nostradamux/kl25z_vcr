/*
 * nRF24L01.c
 *
 *  Created on: Jan 27, 2015
 *      Author: Administrador
 */

#include "nRF24L01.h"
#include "SM1.h"
#include "CE.h"
#include "CSN.h"
#include "WAIT1.h"

const char* regRF24_names[24]=
{
"RF24_CONFIG2",     
"RF24_EN_A",       
"RF24_EN_RXADDR",   
"RF24_SETUP_AW",    
"RF24_SETUP_RETR",  
"RF24_RF_CH",       
"RF24_RF_SETUP",    
"RF24_STATUS",      
"RF24_OBSERVE_TX",  
"RF24_RPD",     
"RF24_RX_ADDR_P0",  
"RF24_RX_ADDR_P1",  
"RF24_RX_ADDR_P2",  
"RF24_RX_ADDR_P3",  
"RF24_RX_ADDR_P4",  
"RF24_RX_ADDR_P5",  
"RF24_TX_ADDR",     
"RF24_RX_PW_P0",    
"RF24_RX_PW_P1",    
"RF24_RX_PW_P2",    
"RF24_RX_PW_P3",    
"RF24_RX_PW_P4",    
"RF24_RX_PW_P5",    
"RF24_FIFO_STAT"
//(unsigned char*)"RF24_CONFIG2",     
//(unsigned char*)"RF24_EN_A",       
//(unsigned char*)"RF24_EN_RXADDR",   
//(unsigned char*)"RF24_SETUP_AW",    
//(unsigned char*)"RF24_SETUP_RETR",  
//(unsigned char*)"RF24_RF_CH",       
//(unsigned char*)"RF24_RF_SETUP",    
//(unsigned char*)"RF24_STATUS",      
//(unsigned char*)"RF24_OBSERVE_TX",  
//(unsigned char*)"RF24_RPD",     
//(unsigned char*)"RF24_RX_ADDR_P0",  
//(unsigned char*)"RF24_RX_ADDR_P1",  
//(unsigned char*)"RF24_RX_ADDR_P2",  
//(unsigned char*)"RF24_RX_ADDR_P3",  
//(unsigned char*)"RF24_RX_ADDR_P4",  
//(unsigned char*)"RF24_RX_ADDR_P5",  
//(unsigned char*)"RF24_TX_ADDR",     
//(unsigned char*)"RF24_RX_PW_P0",    
//(unsigned char*)"RF24_RX_PW_P1",    
//(unsigned char*)"RF24_RX_PW_P2",    
//(unsigned char*)"RF24_RX_PW_P3",    
//(unsigned char*)"RF24_RX_PW_P4",    
//(unsigned char*)"RF24_RX_PW_P5",    
//(unsigned char*)"RF24_FIFO_STATUS"
};

/* Macros to hide low level functionality */
#define RF_WAIT_US(x) WAIT1_Waitus(x) /* wait for the given number of micro-seconds */
#define RF_WAIT_MS(x) WAIT1_Waitms(x) /* wait for the given number of milli-seconds */
#define RF_CE_LOW() CE_ClrVal() /* put CE LOW */
#define RF_CE_HIGH() CE_SetVal() /* put CE HIGH */
#define RF_CSN_LOW() CSN_ClrVal() /* put CSN LOW */
#define RF_CSN_HIGH() CSN_SetVal() /* put CSN HIGH */
/*!
* \brief Writes a byte and reads the value
* \param val Value to write. This value will be shifted out
* \return The value shifted in
*/
static uint8_t SPI_WriteRead(uint8_t val) 
{
	uint8_t ch;
	while (SM1_GetCharsInTxBuf()!=0) {} /* wait until tx is empty */
	while (SM1_SendChar(val)!=ERR_OK) {} /* send character */
	while (SM1_GetCharsInTxBuf()!=0) {} /* wait until data has been sent */
	while (SM1_GetCharsInRxBuf()==0) {} /* wait until we receive data */
	while (SM1_RecvChar(&ch)!=ERR_OK) {} /* get data */
	return ch;
}
/*!
* \brief Writes a buffer to the SPI bus and the same time reads in the data
* \param bufOut Output buffer
* \param bufIn Input buffer
* \param bufSize Size of input and output buffer
*/
static void SPI_WriteReadBuffer(uint8_t *bufOut, uint8_t *bufIn, uint8_t bufSize) 
{
	uint8_t i;
	for(i=0;i<bufSize;i++) 
	{
		bufIn[i] = SPI_WriteRead(bufOut[i]);
	}
}
/*!
* \brief Writes multiple bytes to the SPI bus
* \param bufOut Buffer to write
* \param bufSize Size of buffer
*/
static void SPI_WriteBuffer(uint8_t *bufOut, uint8_t bufSize) 
{
	uint8_t i;
	for(i=0;i<bufSize;i++) 
	{
		(void)SPI_WriteRead(bufOut[i]);
	}
}
/*!
* \brief Write a register value to the transceiver
* \param reg Register to write
* \param val Value of the register to write
*/
void RF_WriteRegister(uint8_t reg, uint8_t val) 
{
	RF_CSN_LOW(); /* initiate command sequence */
	(void)SPI_WriteRead(RF24_W_REGISTER|reg); /* write register command */
	(void)SPI_WriteRead(val); /* write value */
	RF_CSN_HIGH(); /* end command sequence */
	RF_WAIT_US(10); /* insert a delay until next command */
}
/*!
* \brief Reads a byte value from a register
* \param reg Register to read
* \return Register value read
*/
uint8_t RF_ReadRegister(uint8_t reg) 
{
	uint8_t val;
	RF_CSN_LOW();
	(void)SPI_WriteRead(reg);
	val = SPI_WriteRead(0); /* write dummy */
	RF_CSN_HIGH();
	RF_WAIT_US(10);
	return val;
}
/*!
* \brief Read multiple bytes from the bus.
* \param reg Register address
* \param buf Buffer where to write the data
* \param bufSize Buffer size in bytes
*/
void RF_ReadRegisterData(uint8_t reg, uint8_t *buf, uint8_t bufSize) 
{
	RF_CSN_LOW();
	(void)SPI_WriteRead(RF24_R_REGISTER|reg);
	SPI_WriteReadBuffer(buf, buf, bufSize);
	RF_CSN_HIGH();
	RF_WAIT_US(10);
}
/*!
* \brief Write multiple bytes to the bus.
* \param reg Register address
* \param buf Buffer of what to write
* \param bufSize Buffer size in bytes
*/
void RF_WriteRegisterData(uint8_t reg, uint8_t *buf, uint8_t bufSize) 
{
	RF_CSN_LOW();
	(void)SPI_WriteRead(RF24_W_REGISTER|reg); /* not masking registers as it would conflict with RF24_W_TX_PAYLOAD */
	SPI_WriteBuffer(buf, bufSize);
	RF_CSN_HIGH();
	RF_WAIT_US(10);
}
/*!
* \brief Writes a byte and reads back one byte
* \param val Byte to write to the SPI shift register
* \return Byte read from the SPI shift register
*/
uint8_t RF_WriteRead(uint8_t val) 
{
	RF_CSN_LOW();
	val = SPI_WriteRead(val);
	RF_CSN_HIGH();
	RF_WAIT_US(10);
	return val;
}
/*!
* \brief Writes a byte to the bus, without returning the byte read.
* \param val Byte to write.
*/
void RF_Write(uint8_t val) 
{
	RF_CSN_LOW();
	(void)SPI_WriteRead(val);
	RF_CSN_HIGH();
	RF_WAIT_US(10);
}
/*!
* \brief Read and return the STATUS
* \return Status
*/
uint8_t RF_GetStatus(void) 
{
	return RF_WriteRead(RF24_NOP);
}
/*!
* \brief Reset the given mask of status bits
* \param flags Flags, one or more of RF24_STATUS_RX_DR, RF24_STATUS_TX_DS, RF24_STATUS_MAX_RT
*/
void RF_ResetStatusIRQ(uint8_t flags) 
{
	RF_WAIT_US(10);
	RF_CSN_LOW();
	RF_WAIT_US(10);
	RF_WriteRegister(RF24_STATUS, flags); /* reset all IRQ in status register */
	RF_WAIT_US(10);
	RF_CSN_HIGH();
RF_WAIT_US(10);
}
/*!
* \brief Send the payload to the Tx FIFO and send it
* \param payload Buffer with payload to send
* \param payloadSize Size of payload buffer
*/
void RF_TxPayload(uint8_t *payload, uint8_t payloadSize) 
{
	RF_Write(RF24_FLUSH_TX); /* flush old data */
	RF_WriteRegisterData(RF24_W_TX_PAYLOAD, payload, payloadSize); /* write payload */
	RF_CE_HIGH(); /* start transmission */
	RF_WAIT_US(15); /* keep signal high for 15 micro-seconds */
	RF_CE_LOW(); /* back to normal */
}
/*!
* \brief Receive the Rx payload from the FIFO and stores it in a buffer.
* \param payload Pointer to the payload buffer
* \param payloadSize Size of the payload buffer
*/
void RF_RxPayload(uint8_t *payload, uint8_t payloadSize) 
{
	RF_CE_LOW(); /* need to disable rx mode during reading RX data */
	RF_ReadRegisterData(RF24_R_RX_PAYLOAD, payload, payloadSize); /* rx payload */
	RF_CE_HIGH(); /* re-enable rx mode */
}
/*!
* \brief Initializes the transceiver.
*/
void RF_Init(void) 
{
	RF_CE_LOW(); /* CE LOW: do not send or receive data */
	RF_CSN_HIGH(); /* CSN HIGH: not sending commands to the device */
}
