/*
 * DS18B20.c
 *
 * Created: 4/24/2013 3:38:24 PM
 *  Author: mfolz
// connected to  digital pin 6 port D- D6

 */ 

#define F_CPU 16000000UL
#include "DS18B20.h"
#include "../Atmeg_Libs/AtmegaPins.h"
#include <avr/delay.h>

#define DS18B20_PORT PORTD
#define DS18B20_DDR	 DDRD
#define DS18B20_PIN	 PIND6

#define DS18B20		 PORTD6

#define One_Wire_Data_Length 0x08 
#define DS18D20_Resoultion 0x0C

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))


char OneWire_Reset()
{
	char Variable =0x00; 
	CLEARBIT(DS18B20_PORT, DS18B20); // pull line low 
	SETBIT(DS18B20_DDR, DS18B20); // DDR Pull high
	_delay_us(500); // delay 480us minimum (500 to be safe)

	CLEARBIT(DS18B20_DDR, DS18B20); /// Set input
	_delay_us(60);
	Variable = CHECKBIT(PIND, PD6); // read and store
	_delay_us(470);
	return Variable;  // return variable
}

void Onewire_WriteBit(char bit)
{
	// Master pulls low for at least 1us 
	// 15us- 30us sampling time + 15us = 55us to be safe  for both master write 
	CLEARBIT(DS18B20_PORT, DS18B20); // Pull line low
	SETBIT(DS18B20_DDR,DS18B20);  // Set line to output
	_delay_us(1);
	if (bit) CLEARBIT(DS18B20_DDR, DS18B20); 	//If we want to write 1, release the line (if not will keep low)
	_delay_us(60);
	CLEARBIT(DS18B20_DDR, DS18B20);
	_delay_us(5);
}

char Onewire_Readbit(void)
{
	char bit=0x00; 
	// Master pulls low for at least 1us 
	// 45us sampling + 15us = 55us 
	CLEARBIT(DS18B20_PORT, DS18B20); // pull value low
	SETBIT(DS18B20_DDR, DS18B20);
	_delay_us(1);
	CLEARBIT(DS18B20_DDR, DS18B20); // set as input
	_delay_us(14);
	if (CHECKBIT(PIND, PD6)) bit=1; // store value in byte
	_delay_us(45); // allow DS18B20 to respond	
	return bit;
}

void Onewire_WriteByte(char data)
{
	
	for (int datacount= One_Wire_Data_Length-1; datacount >=0 ; datacount--){
		Onewire_WriteBit(data & 0x01);
		data>>=1;
	}
}


char Onewire_Readbyte(void)
{
	char ByteRead = 0x00;
	for (int readbyte = 8; readbyte > 0; readbyte--){
		ByteRead>>=1; // shift over 1
		ByteRead|= (Onewire_Readbit() << 7);
		
	}
	return ByteRead;
	
}

void Onewire_Select(char rom[8])
{
	Onewire_WriteByte(OneWire_MatchRom);
	for (int rom_write =0; rom_write < 8; rom_write++)
		{
			Onewire_WriteByte(rom[rom_write]);
		}	

}

float DS18B20_Convert(char ambient_correction){
	struct DS18B20_Status *Temperature_Values = &Temperature_Sensor;
	char temperature[2];
	char digit;
	char decimal;
	float temperature_result;
	//Reset, skip ROM and start temperature conversion
	OneWire_Reset();
	Onewire_WriteByte(OneWire_SkipRom);
	Onewire_WriteByte(DS18B20_Convert_Temp); // convert temperature
	//Wait until conversion is complete, when complete DS will transmit a 1
	while(!Onewire_Readbit());
	//Reset, skip ROM and send command to read Scratchpad
	OneWire_Reset();
	Onewire_WriteByte(OneWire_SkipRom);
	Onewire_WriteByte(DS18B20_Read_Scratchpad);
	
	//Read Scratchpad (only 2 first bytes)
	temperature[0]= Onewire_Readbyte();
	//USART_send(temperature[0]);
	temperature[1]= Onewire_Readbyte();
	//USART_send(temperature[1]);
	OneWire_Reset();
	//Store temperature integer digits and decimal digits
	digit=temperature[0]>>4;
	digit|=(temperature[1]&0x7)<<4;
	//Store decimal digits
	decimal=temperature[0]&0xf;
	// ambient correction 8C off
	digit -= ambient_correction; 
	//Format temperature into a string [+XXX.XXC]
	temperature_result = (digit) + (decimal/100.0);
	Temperature_Values->Temperature= digit; // could at the decimal values but it isnt necessary
	return temperature_result;
}