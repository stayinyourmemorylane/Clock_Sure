/*
 * Display_Logic.c
 *
 * Created: 4/23/2013 3:11:17 PM
 *  Author: mfolz
 */ 
#include "Display_Logic.h"
#include "../Libraries/32x16 Libs/HT1632C.h"
#include "../Libraries/ClockMemLibs/DS1307.h"
#include "../Libraries/ClockMemLibs/_AA2432A.h"
#include "../Libraries/32x16 Libs/Font.h"
#include "../Libraries/ClockMemLibs/DS18B20.h"

/*
/// make new detections ever 5 minutes 
void Pir_Check_Display(void){
	char time_initial;
	char time_final; 
	char time_compare; 
	
	time_compare=  time_final - time_initial; 
	
	if (PIR_Check() && time_compare){
		H1632C_Draw_Face(2,1,2, 24,(2*24), Asian_Ascii);
		HT1632_WriteString(0, 2, 2, 0, Clock_happy[4]);
		HT1632_Write_StaticLine(0, 1, 1, stringcopy(FloatToArray(DS18B20_Convert(readbytes,2),0,0), "C" ));
		HT1632_ClearScreen();
		time_final = point_time->CurrentTime = TWI_Read(DS_Minutes); // Compare minutes
	}		
}


 HT1632_Write_StaticLine(1,1,2, stringcopy(FloatToArray(DS18B20_Convert2), 0,0), "C"));
 HT1632_WriteString(0,2,1,0,"Its Nice and Warm!");
 H1632C_Draw_Face(2,2,2, 24,0, Asian_Ascii);

*/ 


void Temperature_Display(void){
	struct DS18B20_Status *Temperature_Values = &Temperature_Sensor;
	char Current_Temperature=0; 
	
	DS18B20_Convert(2);
	Current_Temperature = Temperature_Values->Temperature;	

	
	if (Current_Temperature >= 23) {
		HT1632_Write_StaticLine(1,1,2, stringcopy(FloatToArray(DS18B20_Convert(2), 0,0), "C"));
		HT1632_WriteString(0,2,1,0,"Its Pretty Hot in here!");
		H1632C_Draw_Face(2,2,2, (24),(3*24) , Asian_Ascii);		
	}
	
	else if (Current_Temperature > 17 && Current_Temperature < 23) {
		HT1632_Write_StaticLine(1,1,2, stringcopy(FloatToArray(DS18B20_Convert(2), 0,0), "C"));
		HT1632_WriteString(0,2,2,0,"Its Nice and Warm!");
		H1632C_Draw_Face(2,2,2, 24,(0), Asian_Ascii);

	}
	
	else if (Current_Temperature < 17) {
		HT1632_Write_StaticLine(1,1,2, stringcopy(FloatToArray(DS18B20_Convert(2), 0,0), "C"));
		HT1632_WriteString(0,2,0,0,"Its Really Cold!");
		H1632C_Draw_Face(2,2,2, (24),(5*24), Asian_Ascii);
	}

}
	

void Display_Time(int y, int color){
	struct DS1307_Properties *point_time = &Time;
	struct HT1632C_Properties *point_properties = &Current;
	point_properties->color= color;
	point_properties->yPosition= y;
	DS1307_ReadTime(0,color, DS_Hours);
	HT1632_Write_Char(12,y, color,':');
	DS1307_ReadTime(18,color, DS_Minutes);
}



void Display_Date(void){
	struct DS1307_Properties *point_time = &Time;
	DS1307_Copy_Date(); 	
	HT1632_WriteString(0,2,2,0,point_time->Display_Date);
	for (int clearbuff=0; clearbuff < 100; clearbuff++)point_time->Display_Date[clearbuff]=0x00;
}


