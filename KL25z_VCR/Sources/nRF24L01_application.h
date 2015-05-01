/*
 * Application.h
 *
 *      Author: Erich Styger
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "General_Definitions.h"
#include "nRF24L01.h"
#include "LEDR.h"
#include "LEDG.h"
#include "LEDB.h"
#include "WAIT1.h"
#include "CE.h"
#include "DebugSerial.h"

#define PAYLOAD_SIZE 32  /* number of payload bytes, 0 to 32 bytes */
#define CHANNEL_NO   100  /* communication channel */

/* macros to configure device either for RX or TX operation */
#define TX_POWERUP()   RF_WriteRegister(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PWR_UP|RF24_PRIM_TX) /* enable 2 byte CRC, power up and set as PTX */
#define TX_POWERDOWN() RF_WriteRegister(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PRIM_TX) /* enable 2 byte CRC, power down and set as PTX */
#define RX_POWERUP()   RF_WriteRegister(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PWR_UP|RF24_PRIM_RX) /* enable 2 byte CRC, power up and set as PRX */
#define RX_POWERDOWN()   RF_WriteRegister(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PRIM_RX) /* enable 2 byte CRC, power up and set as PRX */

/* interrupt handler */
void NRF24L01_OnInterrupt(void);
void NRF24L01_Print_All_Registers();
void NRF24L01_Init();
void NRF24L01_Send(void); 
void NRF24L01_Receive(void);
void NRF24L01_Set_Buffer_To_Send(uint8_t* buffer,uint8_t size);

#endif /* APPLICATION_H_ */
