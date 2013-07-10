/*
 * _74HC164.c
 *
 * Created: 4/23/2013 12:17:24 AM
 *  Author: Michael
 */
#include "../Atmeg_Libs/AtmegaPins.h"
#include "_74HC164.h"

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

void _74HC164_Initialize(void){
	_74HC164_DDR = (1 << _74HC164_Data) | (1 << _74HC164_Clock);  	
}

// Pull CS LOW to address the HT1632 chip  only good for the Sure display only goes as high as 4 
void _74HC164_ChipSelect(char chip){
	int total_clockcount= 0; 
	if (chip <= 0){
		_74HC164_CS_Data_High();   // force low for all 4 clock cycles
		for(int x= 0; x < CHIP_MAX; x++){
			_74HC164_Clock_High();
			_74HC164_Clock_Low();
		}
	}		
		
	else {
		_74HC164_CS_Data_High(); 
		// Send the initial clock pulses 
		for(int clockloop=4; clockloop >= 0; clockloop--){   /// pro tip change it to 8 and you can use more
			_74HC164_Clock_High();
			_74HC164_Clock_Low();
			_74HC164_CS_Data_High();
			
			if ( clockloop == (chip))_74HC164_CS_Data_Low();
			
			}			
		}
}


