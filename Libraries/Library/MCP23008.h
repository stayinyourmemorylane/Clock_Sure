/*
 * MCP23008.h
 *
 * Created: 3/13/2013 3:40:27 PM
 *  Author: mfolz
 */ 


#ifndef MCP23008_H_INCLUDED_
#define MCP23008_H_INCLUDED_

#define MCPAddress 0x40

void MCP_Initalize(void);
void SetMCPReg(char reg, char val);
void MCP23008_Serial_2_Port(void);

#endif /* MCP23008_H_ */