/*
 * PIR_Sensor.h
 *
 * Created: 3/14/2013 2:12:32 PM
 *  Author: mfolz
 
 Data Sheet for Passive Infra-Red Sensor
 http://www.parallax.com/Portals/0/Downloads/docs/prod/sens/910-28027-PIRsensor-v1.4.pdf
 */ 


#ifndef PIR_SENSOR_H_INCLUDED_
#define PIR_SENSOR_H_INCLUDED_

#define  PIR_SENSOR_DDR  DDRD
#define  PIR_SENSOR_PORT PORTD
#define  PIR_SENSOR_INPUT PIND7 //  digital pin 9 
#define  CALIBRATION_TIME 10  // defined in terms of seconds - datasheet recommends 10-60
#define	 PIR_JUMPER_SETTINGS 1   // 1 implies output is low when idle, 2 - output goes high then low when triggered, repeated high/low pulses
								// when motion is detected low when idle. 
#define  PIR_Check() (PIND & (1 << PIR_SENSOR_INPUT))
void PIR_Initialize_Sensor(void);
char PIR_Check_Delay(void);


#endif /* PIR_SENSOR_H_ */