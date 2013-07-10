/*
 * HT1632C.h
 *
 * Created: 3/4/2013 11:01:51 AM
 *  Author: mfolz
 */ 
#include "HT1632C.h"
#include "../32x16 Libs/_74HC164.h"  // this is the library for the CS functions.
#include "../Atmeg_Libs/AtmegaPins.h"
#include "../../Applications/Memory_Logic.h"
#include "Font.h"

#define  F_CPU    16000000UL
#include <avr/io.h>
#include <util/delay.h>

void HT1632_SendHalfByte(char halfbyte){
	struct HT1632C_Properties *point_properties = &Current;
	_74HC164_ChipSelect(point_properties->chip_select);
	HT1632_WriteBits(HT1632_ID_WR, 3);  		   // send ID: WRITE to RAM
	HT1632_WriteBits(point_properties->Address, 7);
	HT1632_WriteBits(halfbyte, 4);   // no need to send address, ram write or chipselect. for now ...
}

int pattern=0;  /// EEEWWWW GLOBAL VARIABLE 

// Write String
// String will scroll from right to left (1) or from left to right (anything else).
// X is for the starting position of the string, also note that y will only respond to 0,1 and 2
// Green = 0 , Red = 1, Orange = 2

void HT1632_WriteString(int x, int y, int color, int direction, char *string){
	int count =0, variable =0, decriment=0;
	while(*string){ string++; count++;}

	if (direction == 0){
			x= 32;
			variable= -(count*6+1);
			decriment = -1;
		}
	else {
			x= -(count*6);
			variable = 32;
			decriment = 1;
		}

	for(; x != variable;)
		{
			for(int one_char=0; one_char < count; one_char++)
			{
				HT1632_Write_Char(x+(6*one_char), y, color, (string[one_char-count]));
			}
			_delay_ms(12);
			x+= decriment;
		}
}
// up to 5 chars safely 
void HT1632_Write_StaticLine(int xcorridnate, int ycorridnate, int color, char *string){
	char count =0;
	while(*string){ string++; count++;}
	string -=count;
	
	for (int x= 0; x< count ; x++)
	{
		HT1632_Write_Char(xcorridnate,ycorridnate,color, string[x]);
		xcorridnate+=6;
	}
}



void HT1632_Write_Char(int xcorridnate, int ycorridnate, int color, char c){
	int halfbyte=0,  shiftbit=0, address=0, chipselect=1 ; 
	struct HT1632C_Properties *point_properties = &Current;
	
	if (color == 1)point_properties->color= 0x20;
	else point_properties->color= 0x00; 
	
	if (ycorridnate == 2){point_properties->chip_select = 3;}
		else point_properties->chip_select =1 ;
		
	address= (xcorridnate << 1) ; 
	
	for (int charlength= 0; charlength < 7; charlength++)
		{		
			    if (address >= 32 &&  point_properties->chip_select == 1 || address >= 32 && point_properties->chip_select ==3){
				    point_properties->chip_select+=1;
				    address-=32;
			    }
				
				if (address >=0  && address <=30){
					// Used for blanking leds, without this the lights would leave a trail and a appear to drag. 
					if (charlength ==0){
						HT1632_DrawHorizontal_Line(address+ point_properties->color, point_properties->chip_select);
						if (color == 2)	HT1632_DrawHorizontal_Line(address+ point_properties->color +0x20, point_properties->chip_select);
						address+=2;
					}
					else if (charlength == 6) {
						HT1632_DrawHorizontal_Line(address+  point_properties->color, point_properties->chip_select); 
						if (color == 2)	HT1632_DrawHorizontal_Line(address+ point_properties->color +0x20, point_properties->chip_select);
						address+=2;
					}					

					else {
						halfbyte = pgm_read_byte(font+( ((c-32)*5)+charlength-1)); // pull char from array  
						point_properties->Address = address+ point_properties->color;
						HT1632_SendHalfByte(halfbyte >> 4);   // no need to send address, ram write or chipselect. for now ...
						point_properties->Address = address + point_properties->color +1;
						HT1632_SendHalfByte(halfbyte);
						
						if (color == 2){
							point_properties->Address = address+ 0x20;
							HT1632_SendHalfByte(halfbyte >> 4);   // no need to send address, ram write or chipselect. for now ...
							point_properties->Address = address + 0x20 + 1;
							HT1632_SendHalfByte(halfbyte);
							
						}
						
						address+=2;
					}
				}	
			else 	address+=2; 								
		}
	
}


void H1632C_Draw_Face(int xcorridnate, int ycorridnate, int color, int length, int start_array,  char *char_array ){
	struct HT1632C_Properties *point_properties = &Current;
	char halfbyte=0;
	char address=0; 

	if (ycorridnate == 2){point_properties->chip_select = 3;}
	else point_properties->chip_select =1 ;
	address= (xcorridnate << 1);
		
	for (int x=0; x < length; x++)
	{
		 if (address >= 32 &&  point_properties->chip_select == 1 || address >= 32 && point_properties->chip_select ==3){
			 point_properties->chip_select+=1;
			 address-=32;
		}
				
		halfbyte = char_array[x+start_array];
		point_properties->Address = address+ point_properties->color;
		HT1632_SendHalfByte(halfbyte >> 4);   // no need to send address, ram write or chipselect. for now ...
		point_properties->Address = address + point_properties->color +1;
		HT1632_SendHalfByte(halfbyte);		
			
		if (color == 2){
			point_properties->Address = address+ 0x20;
			HT1632_SendHalfByte(halfbyte >> 4);   // no need to send address, ram write or chipselect. for now ...
			point_properties->Address = address + 0x20 + 1;
			HT1632_SendHalfByte(halfbyte);
				
		}
		address+=2; 
	}		
}

void HT1632_DrawHorizontal_Line(int address, int chipselect)
{
	struct HT1632C_Properties *point_properties = &Current;
	_74HC164_ChipSelect(chipselect);
	HT1632_WriteBits(HT1632_ID_WR, 3);  		   // send ID: WRITE to RAM
	HT1632_WriteBits(address, 7);
	HT1632_WriteBits(0x00, 4);   // no need to send address, ram write or chipselect. for now ...
	HT1632_WriteBits(0x00, 4);   // no need to send address, ram write or chipselect. for now ...
}

/// Write to all 4 chips brightness is from 0 - 16 
void HT1632_Brightness(int brightness){
	for (int x=1; x <= CHIPCOUNT; x++) HT1632_SendCommand(x, (HT1632_CMD_PWM+brightness));
}

// clears all colors for all 4 chips
void HT1632_ClearScreen(){
	for (int x=1; x <= CHIPCOUNT; x++ ){
		_74HC164_ChipSelect(x);
		HT1632_WriteBits(HT1632_ID_WR,3);
		HT1632_WriteBits(0,7); // send address 00
		
		for (int y= 0; y<= 64; y++)	HT1632_WriteBits(0x00,4); // send 8 bits of 0 data
		_74HC164_ChipSelect(0);
	}
}
void HT1632_ClearLine(char line){
		char clearline=0; 
		
		if (line == 0 || line == 1){
			clearline = 1;
			
		}
		else  clearline =3;
	
		for (char x=0; x < 2; x++)
		{
			_74HC164_ChipSelect(clearline+x);
			HT1632_WriteBits(HT1632_ID_WR,3);
			HT1632_WriteBits(0,7); // send address 00
			for (int y= 0; y<= 64; y++)	HT1632_WriteBits(0x00,4); // send 8 bits of 0 data	
		}
		_74HC164_ChipSelect(0);		

}


/// plot pixel 
void HT1632_Plot(int x, int y, int color){
	int address=0, chipselect=1;
	int pixel = 0x01;
	
	if (x >= 8 && x <=15){
		chipselect+=2;
		x-=8;
	}
	
	if (y >=16 && y <= 31)
	{
		chipselect+=1;
		y-=16;
	}
	
	if (x >= 4 && x <= 7){
		address+=1;
		x-=4;
	}
	
	
	y <<= 1;
	address+= y;
	pixel <<= (3-x);
	pattern = pattern | pixel;
	
	if (color == 2) address += 0x20;
	
	_74HC164_ChipSelect(chipselect);
	HT1632_WriteBits(HT1632_ID_WR, 3);  		   // send ID: WRITE to RAM
	HT1632_WriteBits(address, 7);       		   // Send address
	HT1632_WriteBits(pattern, 4);   // send 4 bits of data
}

// Initialize the display! 
void HT1632_Initialize(void)
{
	HT1632_DDR |= (1 << HT1632_WR_CLK) | (1 << HT1632_Data);
	
	for (int i=1; i <= CHIPCOUNT; i++) {
	HT1632_SendCommand(i, HT1632_CMD_SYSDIS); // Disable system
	HT1632_SendCommand(i, HT1632_CMD_COMS00); // 16*32, PMOS drivers
	HT1632_SendCommand(i, HT1632_CMD_MSTMD);  // MASTER MODE - Use on-chip clock
	HT1632_SendCommand(i, HT1632_CMD_SYSON);  // System on - Enable system oscil
	HT1632_SendCommand(i, HT1632_CMD_LEDON);  // LEDs on
	}
}

// Send command to H1632 
void HT1632_SendCommand(int chip, int command)
{
	_74HC164_ChipSelect(chip);//pull desired chip low
	HT1632_WriteBits(HT1632_ID_CMD,3);
	HT1632_WriteBits(command,8);
	HT1632_WriteBits(0x00,4);
}

/// Write to the HT1632 8 bits max, length must be input. 
void HT1632_WriteBits(char data, char datalength){

		for (int datacount=datalength-1; datacount >=0 ; datacount--){
			HT1632_WR_CLK_Low();
			if (data & _BV(datacount)){
				HT1632_Data_High();
			}
			
			else {
				HT1632_Data_Low();
			}
			HT1632_WR_CLK_High();	
		}
}

//// LIbraries to sort through and remove. 

/*

void H1632C_Get_Properties(void){
	struct HT1632C_Properties *point_properties = &Current;
	int x=0 , y=0, address=0, chipselect=0;
	point_properties->chip_select = 1;
	
	x =point_properties->xPosition;
	y =point_properties->yPosition;
	
	if (x >= 17 && x <=32){
		point_properties->chip_select+=1;
		point_properties->xPosition-=16;
	}
	
	if (y >=8 && y <= 15)
	{
		point_properties->chip_select+=2;
		point_properties->yPosition-=8;
	}
	
	if (point_properties->color == 2)point_properties->color = 0x20;
	else point_properties->color = 0x00;
	
} 

// While this may be faster it is harder to control, much harder
void HT1632_Write_FastChar(int xcorridnate, int ycorridnate, int color, char c){
	struct HT1632C_Properties *point_properties = &Current;
	int temp= 0x00, led_4bit= 0, shiftbit=0;
	
	point_properties->xPosition = xcorridnate;
	point_properties->yPosition = ycorridnate;

	H1632C_Get_Properties();
	_74HC164_ChipSelect(point_properties->chip_select);
	HT1632_WriteBits(HT1632_ID_WR, 3);  		   // send ID: WRITE to RAM
	HT1632_WriteBits(((point_properties->xPosition*2) + point_properties->color), 7);

	for (int x=5; x>= 1; x--){
		for (int seprate_bit=0; seprate_bit < 2; seprate_bit++)
		{
			shiftbit = ( seprate_bit == 1) ? 4: 0;
			
			led_4bit = array[(((c-32)*5)+5)-x];
			led_4bit >>= shiftbit;
			led_4bit &= 0x0F; // zero out other bits(not necessary, good code practice)
			
			HT1632_WriteBits(led_4bit, 4);   // no need to send address, ram write or chipselect. for now ...
		}
	}
}


void HT1632C_WriteString(int x, int y, int color, int direction, char *string){
	int count =0;
while(*string){ string++; count++;}
//	for( int x=-(count*6); x < 32; x++)
//for( int x=-(count*6); x < 32; x++)

for( int x=32; x > -(count*6+1); x--)
{
	for(int one_char=0; one_char < count; one_char++)
	{
		HT1632_Write_Char(x+(6*one_char), y, color, (string[one_char-count]));
	}
	_delay_ms(75);
}
}


*/ 