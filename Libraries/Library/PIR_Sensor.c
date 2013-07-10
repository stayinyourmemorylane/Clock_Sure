/*
 * PIR_Sensor.c
 *
 * Created: 4/23/2013 12:36:05 AM
 *  Author: Michael
 */ 


#include "PIR_Sensor.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

void PIR_Initialize_Sensor(void){
	PIR_SENSOR_DDR &= (0 << PIR_SENSOR_INPUT);
	//USART_putstring("Calibrating Sensor");
	// this delay isnt 100% necessary you can have the processor do other work
	// and not just derp for 10 seconds
	for(int x=0; x < CALIBRATION_TIME; x++){
		_delay_ms(100);   /// this isnt 100% necessary
		//USART_putstring("....");
	}
	//USART_putstring("\n\r\n Calibration Complete");
}

// redundant code - wont do anything else while the sensor detects movement
char PIR_Check_Delay(void){
	char count=0;
	
	while(PIR_Check()){
		_delay_us(100); 
		count++;		
	}
	 if (count >= 16) return 1;
	else return 0;
}