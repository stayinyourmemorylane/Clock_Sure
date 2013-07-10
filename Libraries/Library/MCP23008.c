/*
 * MCP23008.c
 *
 * Created: 4/23/2013 12:38:02 AM
 *  Author: Michael
 */ 

#include "MCP23008.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>


void MCP_Initalize(void)
{
	///// Set MCP Regestries////
	SetMCPReg(0x00,0x00); // set IODIR  Set all pins to output  Direction port
	SetMCPReg(0x05,0x0C); // set CONFREG (0x05) to 0
	_delay_us(1000);
}

void SetMCPReg(char reg, char val ) {
	I2C_StartWrite(); // Send The Write Command
	I2C_SendByte(MCPAddress);
	I2C_SendByte(reg);
	I2C_SendByte(val);
	I2C_TerminateTransmission();
}

void MCP23008_Serial_2_Port(void)
{
	char byteread= 0x00;
	byteread= ASCII_2_HEX(USART_receive());
	USART_send(byteread);
	byteread <<= 4;
	USART_send(byteread);
	byteread |=  ASCII_2_HEX(USART_receive());
	USART_send(byteread);
	SetMCPReg(0x09,(byteread ^ 0xFF));
	USART_send(byteread ^0xFF);
}

