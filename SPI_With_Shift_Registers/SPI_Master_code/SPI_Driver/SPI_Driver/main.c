/*
 * Test for the SPI Master Driver 
 * Author : Mohamed Hassan Fola
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>
#include "_SPI_Driver_Master.h"
#define EN 3

int main()
{
	SET_BIT_OUTPUT(B, EN);
	SPI_Init_Master(Clk_Rate_FOSC_divisable_by_16, Data_Order_MSB, Clk_Polarity_IDLE_LOW, Clk_Phase_LEADING);
	unsigned char data;
	
	while (1)
	{
		SET_BIT_HIGH(SPI_Port_Name, SS);
		data = SPI_Transceive(0xff);
		SET_BIT_LOW(SPI_Port_Name, SS);
		_delay_ms(10);
		
		SET_BIT_LOW(B, EN);
		SPI_Master_Send(data);
		SET_BIT_HIGH(B, EN);
		_delay_ms(10);
	}
}
