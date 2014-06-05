/*
	LSEL 2014 trainID RX module
	@date March 2014
	@author Jose Martin

	Modified by Enrique Casado

*/


#define ID1 0b00110100
#define ID1pin PB0

#define ID2 0b00101100
#define ID2pin PB2

#define I2C_ADDRESS 0x15 // Address must be different for each sensor

#include <avr/interrupt.h>
#include <util/delay.h>
#include "softuart.h"
#include "usiTwiSlave.h"


int main(void)
{
	sei();
	// Initialize software UART
	softuart_init();

	// Initialize I2C module
	usiTwiSlaveInit(I2C_ADDRESS);

	// Main loop, receive bytes all the time
	for (;;) {
		static uint8_t byte = 0;
		static uint8_t i2cbyte = 0;
		static bool readPending = false; // Flag meant to represent the state of the sensor
		
		if (softuart_kbhit() != 0){	// Checks if a new byte is received from TX and stores it
			byte = softuart_getchar();  
			if (byte == ID1 || byte == ID2) { // When a train crosses the sensor, flag is set to 1
				readPending = true;
			}
		}
		
		if (usiTwiDataInReceiveBuffer() == true){ // Checks if master has asked for data 
			i2cbyte = usiTwiReceiveByte();
			if (readPending == true){
				uint8_t bufByte = byte; // Buffers the byte about to be sent to prevent fails
				usiTwiTransmitByte(bufByte);
				readPending = false;
			}
		}		
	}
	return 0; /* never reached */
}
