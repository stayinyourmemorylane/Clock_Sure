/*
 * Sure_32x16_BiLed.c
 *
 * Created: 3/4/2013 11:00:21 AM
 *  Author: mfolz
 */ 
/*
Serial Shift()
CLK = Digital Pin 9  =  Clock input for the serial shift  = Blue Wire 
CS  = DIgital pin 8  =  8 bit serial shift data input   = Yellow Wire

HT1632C()
WR = Digital pin 11 = Clock input for write data  = Orange Wire
DATA = Digital pin 10 = Data input for 32x16 LED   = Green Wire 


/// DIgital pin 6 == Dalas DS18B20 18B20 
/// http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
*/ 
#define  F_CPU    16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 

// Standard ATMEG Libs
#include "Libraries/Atmeg_Libs/I2C.h"  // i2c and twi libs
#include "Libraries/Atmeg_Libs/Serial_Atmel328p.h" // serial libs
#include "Libraries/Atmeg_Libs/AtmegaPins.h" // useful macros for clear bits

//Sure electronics display libs
#include "Libraries/32x16 Libs/HT1632C.h"// 32x16 Display Libs 
#include "Libraries/32x16 Libs/_74HC164.h" // 3 -> 8 Pseudo SPI to parallel shift register

//Clock Libs
#include "Libraries/ClockMemLibs/_AA2432A.h"  // 32k memory module
#include "Libraries/ClockMemLibs/DS1307.h"  // Clock 

#include "Libraries/Library/MCP23008.h" // Another shift register. 
#include "Libraries/Library/PIR_Sensor.h" // motion detection 
#include "Libraries/ClockMemLibs/DS18B20.h"

// Applications 
#include "Applications/Memory_Logic.h" // 
#include "Applications/Display_Logic.h" // 
#include "Macros_Functions.h"  ///

#include "Libraries/32x16 Libs/Font.h"


char Com_String_Count=1;
char Com_String_Store=1;
char Mem_String_Count=1;
char Mem_String_Store=1;

void Init(void);
void ClockFunction(void);

int main(void)
{
	Init();
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	UCSR0B |= (1 << RXCIE0); // Enable the USART Recieve Complete interrupt (USART_RXC)
	fdevopen(USART_send, NULL); // use printf yyay   

	//Memtest();
	
	//Com_String_Store= _24AA32A_ReadByte(Memory_Com_String_Number);
	//Mem_String_Store= _24AA32A_ReadByte(Memory_String_Number);
	//for (char count=1; count <  Mem_String_Store; count++)USART_putstring(ReadStringMemory(count,0));
	//for (char count=1; count <  Com_String_Store; count++)USART_putstring(ReadStringMemory(count,1));

	//USART_putstring(IntToArray(Mem_String_Store,3));
	//USART_putstring(IntToArray(Com_String_Store,3));
	//DS1307_SetTime(0,36,22,2,24,6, 2013);
	while (1){
	 ClockFunction();
	}		
}	 

void ClockFunction(void){
	struct DS1307_Properties *point_time = &Time;
	struct Memory_Application_Info *Mem_Var = &StringInfo;
	struct Serial_Properties *point_com= &Status;
	char Strings[10] = "ST:";
	char ComStrings[10] = "CM:";
	
		/*if (PIR_Check_Delay()){
			HT1632_WriteString(0, 2, 2, 0, Clock_happy[4]);
			H1632C_Draw_Face(2,2,2, 24,(2*24), Asian_Ascii);			
			HT1632_ClearScreen(); 
		*/
	
		//Display_Time(0,2);
		//HT1632_Write_StaticLine(1,1,2, stringcopy(FloatToArray(DS18B20_Convert(2), 0,0), "C"));
		
		Com_String_Store= _24AA32A_ReadByte(Memory_Com_String_Number);
		if (Com_String_Count >= Com_String_Store)Com_String_Count = 1;
		
		
		Mem_String_Store= _24AA32A_ReadByte(Memory_String_Number);
		if (Mem_String_Count > Mem_String_Store)Mem_String_Count = 1;
		
		
		point_time->CurrentTime = TWI_Read(DS_Seconds); // read seconds (constantly)
			
		// display temperature
		if (point_time->CurrentTime == 0x10){
			Temperature_Display();
		}
			
		// Display memory string
		else if (point_time->CurrentTime == 0x20)
		{
			HT1632_ClearLine(2);
			HT1632_Write_StaticLine(1,1,2, stringcopy(Strings ,IntToArray(Mem_String_Count,1)));
			HT1632_WriteString(0,2,0,0,ReadStringMemory(Mem_String_Count,0));
			Mem_String_Count++; 
			Display_Time(0,2);
		}
		
		// display com string 
		else if (point_time->CurrentTime == 0x40)
		{
			HT1632_Write_StaticLine(1,1,2, stringcopy(ComStrings ,IntToArray(Com_String_Count,1)));
			HT1632_WriteString(0, 2, 1, 0, ReadStringMemory(Com_String_Count,1));
			Com_String_Count++;
			Display_Time(0,2);
		}
	
		// display Date and time moving 
		else if (point_time->CurrentTime == 0x50)
		{	
			Display_Time(0,2);
			Display_Date();					
		} 
		else
		{
			 Display_Time(0,2); 
			 _delay_ms(100);
		}
}

ISR(USART_RX_vect)
{ 
	struct Serial_Properties *point_com= &Status;
	//// Clear buffer when a new string is sent this isnt the best method (there are a few bugs -while a message is being displayed if you try to send a new message the old message 
	//  will become corrupted. you must also disconnect the serial port manually if you dont the string will be wiped. 
	point_com->ComReadBuffer[point_com->buffer_increment] = USART_receive();
	USART_send(point_com->ComReadBuffer[point_com->buffer_increment]);
	if (point_com->ComReadBuffer[point_com->buffer_increment-1] == 0x5C && point_com->ComReadBuffer[point_com->buffer_increment] == 0x30)
		{
			point_com->ComReadBuffer[point_com->buffer_increment-1] = 0x00;
			point_com->ComReadBuffer[point_com->buffer_increment] = 0x00;
			
			WriteString2Memory(point_com->ComReadBuffer, 1);
			HT1632_WriteString(0,2,2,0,point_com->ComReadBuffer ); 
			for (int clearbuff=0; clearbuff < 100; clearbuff++)point_com->ComReadBuffer[clearbuff]=0x00;
			point_com->buffer_increment =0; 
		}	
	/// repeat outgoing what is incoming. 
	point_com->buffer_increment++;
}



void Init(void){
	USART_init();
	_74HC164_Initialize();
	HT1632_Initialize();
	HT1632_Brightness(2);
	HT1632_ClearScreen();
	I2C_Initalize();
	MCP_Initalize();
	//PIR_Initialize_Sensor();
}


