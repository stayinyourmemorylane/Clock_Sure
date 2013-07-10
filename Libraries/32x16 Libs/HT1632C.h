/*
 * HT1632C.h
 *
 * Created: 3/4/2013 11:01:51 AM
 *  Author: mfolz
 */ 


#ifndef HT1632C_H_INCLUDED_
#define HT1632C_H_INCLUDED_

//// Operation Mode 

#define HT1632_ID_CMD 0x04	/* ID = 100 - Commands */
#define HT1632_ID_RD  0x06	/* ID = 110 - Read RAM */
#define HT1632_ID_WR  0x05/* ID = 101 - Write RAM */

/* Commands
-------------
all commands are started with initially the 100 cmd followed by the 9 bit command (last bit is always DNC)
*/
#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSON  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-0xxx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x18	/* CMD= 0001-10xx-x Use on-chip clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM    0xA0	/* CMD= 101x-PPPP-x PWM duty cycle */

#define HT1632_CMD_SYSDIS  0x00 // Turn off both the oscillator and led duty cycle generator
#define HT1632_CMD_MSTMD   0x18  // MASTER MODE - Use on-chip clock
#define HT1632_CMD_SYSON   0x01  // System on - Enable system oscil
#define HT1632_CMD_COMS00  0x20 // 16*32, PMOS drivers Reflects the configuration of the display
#define HT1632_CMD_LEDON   0x03  // LEDs on

//this must be sent to all 4 Chips, this might be able to be sent separatly.

#define  HT1632_DDR  DDRB
#define  HT1632_PORT  PORTB 
#define  HT1632_WR_CLK PINB3 //digital pin 11
#define  HT1632_Data PINB2 // digital pin 10 

#define  HT1632_WR_CLK_Low()	CLEARBIT(HT1632_PORT,HT1632_WR_CLK) // pull clock low
#define  HT1632_WR_CLK_High()	SETBIT(HT1632_PORT, HT1632_WR_CLK) // pull clock high
#define  HT1632_Data_Low()		CLEARBIT(HT1632_PORT,HT1632_Data) // pull data low 
#define  HT1632_Data_High()		SETBIT(HT1632_PORT, HT1632_Data) // pull data high
#define  CHIPCOUNT 04


void HT1632_SendCommand(int chip, int command); 
void HT1632_Initialize(void);
void HT1632_ClearScreen(); 
void HT1632_Brightness(int brightness);
void HT1632_Plot(int x, int y, int color);
void HT1632_SendHalfByte(char halfbyte);
void HT1632_Write_Char(int xcorridnate, int ycorridnate, int color, char c);
int HT1632_ChipSelect(int x, int y); 
void HT1632C_WriteString(int x, int y, int color, int direction, char *string);
void H1632C_Draw_Face(int xcorridnate, int ycorridnate, int color, int length, int start_array,  char *char_array ); 
void HT1632_Write_StaticLine(int xcorridnate, int ycorridnate, int color, char *string);
void HT1632_ClearLine(char line);

void HT1632_DrawHorizontal_Line(int address, int chipselect);
void H1632C_Get_Properties(void);

struct HT1632C_Properties{
	int xPosition;
	int yPosition;
	int Address;
	int color;
	int chip_select;
}Current;


#endif
