/*
 * Atmega16/Atmega32 SPI Driver for "MASTER" device 
 * Author : Mohamed Hassan Fola
 */ 

/*
		   -----
1		 -|		|-		40
2		 -|		|-		39
3		 -|		|-		38
4		 -|		|-		37
5: ~SS	 -|		|-		36
6: MOSI	 -|		|-		35
7: MISO	 -|		|-		34
8: SCK	 -|		|-		33
9		 -|		|-		32
10		 -|		|-		31
11		 -|		|-		30
12		 -|		|-		29
13		 -|		|-		28
14		 -|		|-		27
15		 -|		|-		26
16		 -|		|-		25
17		 -|		|-		24
18		 -|		|-		23
19		 -|		|-		22
20		 -|		|-      21
		   -----	       
*/

#ifndef _SPI_DRIVER_MASTER_H_
#define _SPI_DRIVER_MASTER_H_

#include <stdint.h>
#include "_Bit_Manipulation.h"

#define Convert8BitsToDecimal(Bit7, Bit6, Bit5, Bit4, Bit3, Bit2, Bit1, Bit0) \
(((Bit7)<<7)|((Bit6)<<6)|((Bit5)<<5)|((Bit4)<<4)|((Bit3)<<3)|((Bit2)<<2)|((Bit1)<<1)|((Bit0)<<0))


// registers used
#define SPI_Contorl_Register  SPCR
#define SPI_Data_Register     SPDR
#define SPI_Status_Register   SPSR
#define SPI_Direction         DDRB
#define SPI_Port              PORTB
#define SPI_Port_Name         B

// SPI pins
#define SS     4
#define MOSI   5
#define MISO   6
#define SCK    7

// SPCR register pins
#define SPIE   7
#define SPE    6
#define DORD   5
#define MSTR   4
#define CPOL   3
#define CPHA   2
#define SPR1   1
#define SPR0   0

// SPFR register pins
#define SPIF   7
#define WCOL   6
#define SPI2X  0

// the available data order
#define Data_Order_MSB   0
#define Data_Order_LSB   1

// the available clock rate
#define Clk_Rate_FOSC_divisable_by_4       0
#define Clk_Rate_FOSC_divisable_by_16      1
#define Clk_Rate_FOSC_divisable_by_64      2
#define Clk_Rate_FOSC_divisable_by_128     3
#define Clk_Rate_FOSC_divisable_by_2       4
#define Clk_Rate_FOSC_divisable_by_8       5
#define Clk_Rate_FOSC_divisable_by_32      6

// the available clock polarity
#define Clk_Polarity_IDLE_LOW    0
#define Clk_Polarity_IDLE_HIGH   1

// the available clock phase
#define Clk_Phase_LEADING        0
#define Clk_Phase_TRAILING       1


// Function to initialize the Master device of SPI communication (Without interrupt)
void SPI_Init_Master(uint8_t Clk_Rate, uint8_t Data_Order, uint8_t Clk_Polarity, uint8_t Clk_Phase)
{
	SPI_Direction |= MASK(MOSI) | MASK(SCK) | MASK(SS);    // Set MOSI and SCK pins OUTPUT 
	SET_BIT_INPUT(SPI_Port_Name, MISO);                    // Set MISO pin INPUT                                                 
	SET_BIT_HIGH(SPI_Port_Name, SS);
	
	// SPI Configurations 
	SPI_Status_Register  = Convert8BitsToDecimal(1, 1, 0, 0, 0, 0, 0, READ_BIT(Clk_Rate, 2));
	SPI_Contorl_Register = Convert8BitsToDecimal(0, 1, READ_BIT(Data_Order, 0), 1, READ_BIT(Clk_Polarity, 0), READ_BIT(Clk_Phase, 0), READ_BIT(Clk_Rate, 1), READ_BIT(Clk_Rate, 0));
}


// Function to Write and Read 
unsigned char SPI_Transceive(unsigned char Data)
{
	SPI_Data_Register = Data;                              // Load the Data to SPDR register
	while(READ_BIT(SPI_Contorl_Register, SPIF) == 0);      // wait until the transmission is over
	
	return SPI_Data_Register;                              // Return the received Data
}


// Function to Transmit Data from Master
void SPI_Master_Send(unsigned char Data)
{
	SPI_Data_Register = Data;                             // Load the Data to SPDR register
	while(READ_BIT(SPI_Status_Register, SPIF) == 0);      // wait until the transmission is over
}


// Function to receive Data for Master
unsigned char SPI_Master_receive()
{
	while(READ_BIT(SPI_Status_Register, SPIF) == 0);           // wait to receive all Data
	return SPI_Data_Register;                             // return the received Data
}


#endif  /* _SPI_DRIVER_MASTER_H_ */
