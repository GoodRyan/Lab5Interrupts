//
// lcd.h
// Authors: Todd Branchflower and Ryan Good
// Date: 17 October 2013
// Description: Library to interact with the Geek Box LCD.
//
#ifndef _lcd_h
#define _lcd_h
#include <msp430.h>


void initSPI();

void LCDinit();

void LCDclear();

void secondLine();

void firstLine();

void print(char * string);

void scrollString(char * string1, char * string2);

void writeCommandByte(char commandByte);

void writeDataByte(char dataByte);

void writeCommandNibble(char commandNibble);

//void selectMessage(char buttonPressed);

#endif
