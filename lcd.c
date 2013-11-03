//
// lcd.h
// AuthorL Ryan Good
// Date: 17 October 2013
// Description: Library to interact with the Geek Box LCD.
//

#include "lcd.h"
//#include "button.h"

#define RS_MASK 0x40

char LCDCON = 0;// LCDSEND = 0;

void initSPI(){
	UCB0CTL1 |= UCSWRST;

	UCB0CTL0 |= UCCKPH|UCMSB|UCMST|UCSYNC;
	UCB0CTL1 |= UCSSEL1;
	//UCB0STAT |= UCLISTEN;
	P1DIR |= BIT0;

	P1SEL |= BIT5;
	P1SEL2 |= BIT5;

	P1SEL |= BIT7;
	P1SEL2 |= BIT7;

	P1SEL |= BIT6;
	P1SEL2 |= BIT6;

	UCB0CTL1 &= ~UCSWRST;
}

void setSlaveSelectLo(){
	P1OUT &= ~BIT0;
}

void setSlaveSelectHi(){
	P1OUT |= BIT0;
}

void SPIsend(char byteToSend){
	volatile char readByte;

	setSlaveSelectLo();

	UCB0TXBUF = byteToSend;

	while(!(UCB0RXIFG & IFG2)){
		//waiting for byte
	}

	readByte = UCB0RXBUF;

	setSlaveSelectHi();

}

//formerly LCDDELAY1 in assembly
void LCDdelayShort(){
	_delay_cycles(100);
}

//formerly LCDDELAY2 in assembly
void LCDdelayLong(){
	_delay_cycles(1900);
}

void LCDwriteFour(char LCDdata){
	char halfCharacter = LCDdata;
	halfCharacter &= 0x0f;
	halfCharacter |= LCDCON;
	halfCharacter &= 0x7f;
	SPIsend(halfCharacter);
	LCDdelayShort();

	halfCharacter |= 0x80;
	SPIsend(halfCharacter);
	LCDdelayShort();

	halfCharacter &= 0x7f;
	SPIsend(halfCharacter);
	LCDdelayShort();

}

void LCDwriteEight(char byteToSend){
	unsigned char sendByte = byteToSend;

	sendByte &= 0xF0;

	sendByte = sendByte >> 4;

	LCDwriteFour(sendByte);

	sendByte = byteToSend;

	sendByte &= 0x0F;

	LCDwriteFour(sendByte);


}

void writeCommandNibble(char commandNibble){
	LCDCON &= ~RS_MASK;
	LCDwriteFour(commandNibble);
	LCDdelayLong();
}

void writeCommandByte(char commandByte){
	LCDCON &= ~RS_MASK;
	LCDwriteEight(commandByte);
	LCDdelayLong();
}

void writeDataByte(char dataByte){
	LCDCON |= RS_MASK;
	LCDwriteEight(dataByte);
	LCDdelayLong();
}

void LCDinit(){
	writeCommandNibble(0x03);

	writeCommandNibble(0x03);

	writeCommandNibble(0x03);

    writeCommandNibble(0x02);

    writeCommandByte(0x28);

    writeCommandByte(0x0C);

    writeCommandByte(0x01);

    writeCommandByte(0x06);

    writeCommandByte(0x01);

    writeCommandByte(0x02);

    SPIsend(0);
    LCDdelayShort();
}

void LCDclear(){
	writeCommandByte(1);
}

//might not need
void printCharacter(char asciiChar){
	writeDataByte(asciiChar);
}

void print(char * string){
	int stringTracker = 0;

	while (stringTracker < 8 && *string != 0x00){
		printCharacter(*string);
		string++;
		stringTracker++;
	}

}

char * scrollHelper(char * start, char * current){
	if(*current == 0){
		current = start;
	}
	char * display = current;
	int positionTracker = 0;
	for (positionTracker = 0; positionTracker < 8; positionTracker++){
		writeDataByte(*display);
		display++;
		if(*display == 0){
			display = start;
		}
	}
	return current + 1;
}


void scrollString(char * string1, char * string2){
	char * startStringOne = string1;
	char * currentStringOne = string1;
	char * startStringTwo = string2;
	char * currentStringTwo = string2;
	while(1){
		firstLine();
		currentStringOne = scrollHelper(startStringOne, currentStringOne);
		secondLine();
		currentStringTwo = scrollHelper(startStringTwo, currentStringTwo);
		_delay_cycles(300000);
	}

}


/**char getButton(){
	char buttonPressed = 0;
	configureP1PinAsButton(BIT1|BIT2|BIT3);
	while (buttonPressed != 1 && buttonPressed != 2 && buttonPressed != 3){
	if(isP1ButtonPressed(BIT1)){
		waitForP1ButtonRelease(BIT1);
		buttonPressed = 1;
	}
	if(isP1ButtonPressed(BIT2)){
		waitForP1ButtonRelease(BIT2);
		buttonPressed = 2;
	}
	if(isP1ButtonPressed(BIT3)){
		waitForP1ButtonRelease(BIT3);
		buttonPressed = 3;
	}
	}
	return buttonPressed;
}

//moves cursor to the second line
void secondLine(){
	writeCommandByte(0xa8);
}

//moves cursor to the first line
void firstLine(){
	writeCommandByte(0x80);
}
**/

/**void selectMessage(char buttonPressed){
	if(buttonPressed == 1){
		scrollString("ECE382 is my favorite class!", "You pressed the first button");
	}
	else if(buttonPressed == 2){
		scrollString("ECE382 is my favorite class!", "You pressed the second button");
	}
	else if(buttonPressed == 3){
		scrollString("ECE382 is my favorite class!", "You pressed the third button");
	}
}
**/
