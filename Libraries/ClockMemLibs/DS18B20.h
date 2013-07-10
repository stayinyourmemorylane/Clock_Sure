/*
 * DS18B20.h
 *
 * Created: 4/24/2013 3:38:35 PM
 *  Author: mfolz
 */ 
//// Standard : 15.4kbps 
/// 60us 

/// REset -> rom command -> function command 

#ifndef DS18B20_H_INCLUDED
#define DS18B20_H_INCLUDED

#include <avr/io.h>


#define DS18B20_PORT PORTD
#define DS18B20_DDR	 DDRD
#define DS18B20_PIN	 PIND6
#define DS18B20		 PORTD6
// page 10-12 has discriptions of each command
#define OneWire_SearchRom		0xF0 // during boot master must identify the rom codes for all slave devices on the bus. 
#define OneWire_ReadRom			0x33 //  This command can only be used when there is one slave on the bus, 
#define OneWire_MatchRom		0x55 // Allows the master to address a specific device on single bus
#define OneWire_SkipRom			0xCC // Master can use the command to address all devices on the bus.
#define OneWire_AlarmSearch		0xEC // Search all devices to see if an alarm state is met

#define DS18B20_Convert_Temp	 0x44 // converts internal stored data, transmits 0 while converting 1 when complete
#define DS18B20_Write_Scratchpad 0x4E // Ability to write three bytes to the scratch pad 
#define DS18B20_Read_Scratchpad	 0xBE // Read the content of the scratch pad 
#define DS18B20_Copy_ScratchPad  0x48 // 
#define DS18B20_Recal_E2		 0xB8
#define DS18B20_ReadPSU			 0xB4


/* constants */
#define THERM_DECIMAL_STEPS_12BIT 625 //.0625

float DS18B20_Convert(char ambient_correction);
char OneWire_Reset();
void Onewire_WriteBit(char bit);
char Onewire_Readbit(void);
void Onewire_WriteByte(char data);
char Onewire_Readbyte(void);


struct DS18B20_Status{
	int Temperature;
}Temperature_Sensor; 


#endif /* DS18B20_H_ */