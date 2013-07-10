/*
 * Memory_Logic.c
 *
 * Created: 4/29/2013 10:18:49 AM
 *  Author: mfolz
 */ 



#include "Memory_Logic.h"
#include "../Libraries/32x16 Libs/Font.h"
#include "../Libraries/32x16 Libs/HT1632C.h"
#include "../Libraries/ClockMemLibs/DS1307.h"
#include "../Libraries/ClockMemLibs/_AA2432A.h"
#define  F_CPU    16000000UL
#include <avr/io.h>
#include <util/delay.h>

/// Display Library // Display both Hours and minutes // should make this variable.
char *Douglass[] = {
"I'm not mad, I'm proud of you. You took your first pinch like a man and you learn two great things in your life. Look at me, never rat on your friends and always keep your mouth shut. - Goodfellas",
"If real is what you can feel, smell, taste and see, then 'real' is simply electrical signals interpreted by your brain  - Matrix ",
"Throughout human history, we have been dependent on machines to survive. Fate, it seems, is not without a sense of irony. - Matrix",
"Sooner or later you're going to realize just as I did that there's a difference between knowing the path and walking the path. - Matrix",
};
/*
"Please! This is supposed to be a happy occasion. Let's not bicker and argue over who killed who.  - Monty Python",
"Well, now, uh, Launcelot, Galahad, and I, wait until nightfall, and then leap out of the rabbit, taking the French by surprise - not only by surprise, but totally unarmed!  - Monty Python",
"The only thing worse than not having a job is looking for one. - Kids in the hall",
"All right now, son, I want you to get a good night's rest. And remember, I could murder you while you sleep. - Kids in the hall",
"I can't have a breakthrough without taking a break - Tiger & Bunny",

"Hey mister, I am mad scientist. It's SO COOOOOL!!! Sonovabitch. - Stines;Gate",
"The world is not beautiful: And that, in a way, lends it a sort of beauty. - Kino no Tabi",
" One of the things I have learned through my travels is to retain my thoughts until I have solid facts. - Kino No Tabi",
"Fear gave rise to fear, and soon they became a shadow, dark beyond human understanding. - Mononoke",
"To face one's own feelings is akin to throwing oneself into a bottomless pit, or setting sail on an endless sea. - Mononoke",

"Listen up, maggots. You are not special. You are not a beautiful or unique snowflake. You're the same decaying organic matter as everything else.  - Fight Club",
"Without pain, without sacrifice, we would have nothing. Like the first monkey shot into space. - Fight Club",
"I'll bring us through this. As always. I'll carry you - kicking and screaming - and in the end you'll thank me.  - Fight Club",
" Why don't you boys go down to Wall Street and find some real crooks? Whoever sold you those suits had a wonderful sense of humor. - Goodfellas",

"I'm not mad, I'm proud of you. You took your first pinch like a man and you learn two great things in your life. Look at me, never rat on your friends and always keep your mouth shut. - Goodfellas",
"If real is what you can feel, smell, taste and see, then 'real' is simply electrical signals interpreted by your brain  - Matrix ",
"Throughout human history, we have been dependent on machines to survive. Fate, it seems, is not without a sense of irony. - Matrix",
"Sooner or later you're going to realize just as I did that there's a difference between knowing the path and walking the path. - Matrix",
// MAX 

"Sooner or later you're going to realize just as I did that there's a difference between knowing the path and walking the path. - Matrix",
"Have you ever had a dream, Neo, that you were so sure was real? What if you were unable to wake from that dream? How would you know the difference between the dream world and the real world? - Matrix",
"I seldom end up where I wanted to go, but almost always end up where I need to be.",// length 83
"A common mistake that people make when trying to design something completely foolproof is to underestimate the ingenuity of complete fools."	, // 140 length
"I'm sorry Dave, I'm afraid I can't do that.",
"I've been talking to the main computer.... It hates me :( - ",
"I've calculated your chance of survival, but I don't think you'll like it.",

*/ 

char *Clock_happy[] =
{
"Thi255125s is a test",
"Th12512561is is also a test",
"TEST!!!!                 were u surprised? ", // blank....
"TEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEST", // blank....
"I have no idea what i am doing",
"TESTING TEST TESTING TESTERS TESTS",
 "You can go away now"
};  


void Memtest(void){
	struct Memory_Application_Info *Mem_Var = &StringInfo;
/*
	// used for strings
	_24AA32A_SendByte(Memory_String_Number,0x01);
	_24AA32A_SendByte(0x0002,0x01);  // write 256
	_24AA32A_SendByte(0x0003,0x00); */

	/// Used for com
	//WipeMemory(0x0A00, 0x0E00);
	_24AA32A_SendByte(Memory_Com_String_Number,0x01);
	_24AA32A_SendByte(0x0A02,0x0B);  // write 0x0B00
	_24AA32A_SendByte(0x0A03,0x00);
		
	
	HT1632_WriteString(0,2,2,0,"Writing Strings");
	WriteString2Memory("Don't have negative thoughts. Remember your mantra.",1);
	WriteString2Memory("Where there is fear and doubt, let us bring hope.",1);
	WriteString2Memory("It's much easier to become interested in others than it is to convince them to be interested in you",1);

	
	for (int i=0; i < 3; i++){
	//	WriteString2Memory(Douglass[i],0); /// should be in 256 chars or less
		
	}
  
}


void WipeMemory(int startmem, int endmem){
	for (;startmem < endmem; startmem++){
		_24AA32A_SendByte(startmem,0x00);
		DataInDelay();
	}		

}

/*
// First 10k reserved for strings and string information
// first 2 bytes holds the number of strings stored, next 4 bytes there after hold Location& length for 400 bytes- 9600 bytes for strings
// Font, and serial store
/// String_Type   4096 bytes apparently
	1 = Quotes -> 0x0000 - 0x09FF-  2559 chars 
	2 =  Com Strings -> 0x0A00- 0x1000 - 1536 chars  
	
	
	//// Skim through the code remove the bloat 
*/
void WriteString2Memory(char *stringin, char String_Type){
	struct Memory_Application_Info *Mem_Var = &StringInfo;
		
	Mem_Var->String_Location =0;
	Mem_Var->String_Length= 0; 
	Mem_Var->String_Lenght_Count =0;
	 
	if (String_Type == 1){
		Mem_Var->Memory_Message_Number = Memory_Com_String_Number; 
		Mem_Var->Memory_Message_Section = Memory_Com_String_Section; 
		Mem_Var->Memory_Message_Location_Begin = Memory_Com_Location_Begin;
		Mem_Var->Memory_Message_Section_End = Memory_Com_String_End;
	}
	else{
		Mem_Var->Memory_Message_Number = Memory_String_Number;
		Mem_Var->Memory_Message_Section = Memory_String_Section;
		Mem_Var->Memory_Message_Location_Begin = Memory_String_Location_Begin;
		Mem_Var->Memory_Message_Section_End = Memory_String_End;
	}
	
	Mem_Var->String_Number = _24AA32A_ReadByte(Mem_Var->Memory_Message_Number);    // returns the number of strings in the memory module
	

	while(*stringin){
		Mem_Var->String_Lenght_Count++;
		stringin++;
	}
	
	stringin -= Mem_Var->String_Lenght_Count;
	/// read string location, this takes up to 2bytes of memory
	Mem_Var->String_Location = _24AA32A_ReadByte((((Mem_Var->String_Number)-1)*4)+Mem_Var->Memory_Message_Location_Begin); // read last 8 bits
	Mem_Var->String_Location <<= 8;
	Mem_Var->String_Location += _24AA32A_ReadByte(((((Mem_Var->String_Number)-1)*4) + Mem_Var->Memory_Message_Location_Begin)+1);  // read first 8 bits
	
	// If the string + string location is beyond the string boundries return and do nothing. 
	if ((Mem_Var->String_Lenght_Count + Mem_Var->String_Location) > Mem_Var->Memory_Message_Section_End) return; 



	for(int Send_Char=0; Send_Char < Mem_Var->String_Lenght_Count; Send_Char++){
		_24AA32A_SendByte(Mem_Var->String_Location, stringin[Send_Char]);
		Mem_Var->String_Location++;
		Mem_Var->String_Length++;
	}
	
	Mem_Var->String_Location+=1;
	// Send String Location 2 bytes worth
	_24AA32A_SendByte( ((((Mem_Var->String_Number)*4)+2) + Mem_Var->Memory_Message_Number), ((Mem_Var->String_Location >> 8) &0x00FF)  ); // send last 8 bits 0x0000----
	_24AA32A_SendByte( ((((Mem_Var->String_Number)*4)+3)+ Mem_Var->Memory_Message_Number) , (Mem_Var->String_Location & 0x00FF));  // send first 8 bits  0x----0000
	// Send String Length
	_24AA32A_SendByte( (((Mem_Var->String_Number*4)+0)+ Mem_Var->Memory_Message_Number) , ((Mem_Var->String_Lenght_Count >> 8) & 0x00FF) ); // send last 8 bits
	_24AA32A_SendByte( (((Mem_Var->String_Number*4)+1)+ Mem_Var->Memory_Message_Number) , Mem_Var->String_Lenght_Count);
	
	//Send String Number increase
	Mem_Var->String_Number++;
	_24AA32A_SendByte(Mem_Var->Memory_Message_Number,Mem_Var->String_Number);
	
}


////////////// NEEDS WORK!!!
// reads up to 256 chars 
char *ReadStringMemory(char stringnumber, char String_Type){
	struct Memory_Application_Info *Mem_Var = &StringInfo;
	/// wipe before using every time
	
	for (int x =0; x < 256; x++){ Mem_Var->Read_String[x]= 0x00;}
	
	if (String_Type == 1){
		Mem_Var->Memory_Message_Location_Begin= Memory_Com_String_Number;
	}
	else{
		Mem_Var->Memory_Message_Location_Begin= Memory_String_Number;
	}
	
	// Reading Location
	Mem_Var->String_Location = (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+((stringnumber*4)-2));
	Mem_Var->String_Location <<= 8;
	Mem_Var->String_Location += (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+((stringnumber*4)-1));

	// Reading String Length
	Mem_Var->String_Length = (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+ (stringnumber*4));
	Mem_Var->String_Length <<= 8;
	Mem_Var->String_Length += (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+ ((stringnumber*4)+1));

	//Store String in a central location for reading.
	for (Mem_Var->String_Inc= 0; Mem_Var->String_Inc < Mem_Var->String_Length ; Mem_Var->String_Inc++){
		Mem_Var->Read_String[Mem_Var->String_Inc] = _24AA32A_ReadByte((Mem_Var->String_Inc)+(Mem_Var->String_Location));
	}
	Mem_Var->Read_String[((Mem_Var->String_Location)+1)]= '\0';

	return Mem_Var->Read_String;
}


/*
void Memtest(void){
	struct Memory_Application_Info *Mem_Var = &StringInfo;
	HT1632_WriteString(0,2,2,0,"Done Wiping Mem");
	_24AA32A_SendByte(Memory_String_Number,0x01);
	_24AA32A_SendByte(Memory_String_Number,0x01);
	_24AA32A_SendByte(0x0002,0x01);  // write 256
	_24AA32A_SendByte(0x0003,0x00);
	
	HT1632_WriteString(0,2,2,0,"Writing Strings");
	for (int i=0; i < 6; i++){
		WriteString2Memory(Douglass[i]); /// should be in 256 + 15
		
	}

	HT1632_WriteString(0,2,2,0,"Reading Strings");
	HT1632_WriteString(0,2,2,0,ReadStringMemory(3));  //
	HT1632_WriteString(0,2,2,0,ReadStringMemory(2));  //
	HT1632_WriteString(0,2,2,0,ReadStringMemory(1));  // you need to wipe string_Return everytime
	// you use it

}



void StoreFont(void){
	char storebyte= 0x00;
	USART_putstring("reading ");
	for (int storeall=0; storeall < 94; storeall++)
	{
		for (char x=0; x< 5; x++)
		{
			storebyte = pgm_read_byte(font+(((storeall)*5)+x)); //
			_24AA32A_SendByte((Memory_Font_Start+x+(5*storeall)),storebyte);
			
		}
	}
	USART_putstring("reading ");
}

char *ReadFont(int font_value){
	char byte;
	_24AA32A_ReadByte(Memory_Font_Start+font_value);
	return byte;
}


*/ 