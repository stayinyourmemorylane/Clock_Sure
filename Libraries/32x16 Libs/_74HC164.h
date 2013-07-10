/*
 * _74HC164.h
 *
 * Created: 3/4/2013 11:01:21 AM
 *  Author: mfolz
 */ 
#ifndef _74HC164_H_INCLUDED_
#define _74HC164_H_INCLUDED_

#define  _74HC164_DDR  DDRB
#define  _74HC164_PORT PORTB
#define  _74HC164_Clock PINB1 //  digital pin 9
#define  _74HC164_Data PINB0 // digital pin 8
#define  _74HC164_Clock_Low()	CLEARBIT(_74HC164_PORT, _74HC164_Clock) // pull clock low
#define  _74HC164_Clock_High()	SETBIT(_74HC164_PORT,_74HC164_Clock) // pull clock high
#define  _74HC164_CS_Data_Low() CLEARBIT(_74HC164_PORT, _74HC164_Data) // pull data low
#define  _74HC164_CS_Data_High() SETBIT(_74HC164_PORT, _74HC164_Data) // pull data low
#define CHIP_MAX 4 // Number of HT1632C Chips

void _74HC164_ChipSelect(char chip);
void _74HC164_Initialize(void);


#endif /* 74HC164_H_ */
