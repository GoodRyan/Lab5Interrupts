#include <msp430g2553.h>
#include "game.h"
#include "lcd.h"
#include "button.h"

unsigned char initPlayer()
{
        return 0x80;
}

void printPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte('*');
}

void clearPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction)
{
        switch (direction) {
        case RIGHT:
        	if((player != 0x87) && (player != 0xC7)){
        	player += 0x01;
        	}
        	break;
        case LEFT:
        	if(player != 0x80 && player != 0xC0){
        	player -= 0x01;
        	}
        	break;
        case UP:
        	if(player > 0x87){
        	player -= 0x40;
        	}
        	break;
        case DOWN:
        	if (player < 0xC0){
        	player += 0x40;
        	}
        	break;
        }

        return player;
}

/**unsigned char movementCheck(unsigned char player, char button){
	if(isP1ButtonPressed(BIT1))
	{
	    		clearPlayer(player);
	    		player = movePlayer(player, RIGHT);
	    		printPlayer(player);
	    		TAR = 0;
	    		waitForP1ButtonRelease(BIT1);
	    		debounce();
	    	}
	    	else if(isP1ButtonPressed(BIT2)){
	    		clearPlayer(player);
	    		player = movePlayer(player, LEFT);
	    		printPlayer(player);
	    		TAR = 0;
	    		waitForP1ButtonRelease(BIT2);
	    		debounce();
	    	}
	    	else if(isP1ButtonPressed(BIT3)){
	    		clearPlayer(player);
	    		player = movePlayer(player, UP);
	    		printPlayer(player);
	    		TAR = 0;
	    		waitForP1ButtonRelease(BIT3);
	    		debounce();
	    	}
	    	else if(isP1ButtonPressed(BIT4)){
	    		clearPlayer(player);
	    		player = movePlayer(player, DOWN);
	    		printPlayer(player);
	    		TAR = 0;
	    		waitForP1ButtonRelease(BIT4);
	    		debounce();
	    	}
	return player;
}
**/
char didPlayerWin(unsigned char player)
{
        return player == 0xC7;
}

char testAndRespondToButtonPush(char buttonToTest)
{
	char pressed = 0;
    if (buttonToTest & P1IFG)
    {
        if (buttonToTest & P1IES)
        {
        	pressed = 1;
        } else
        {
            debounce();
        }

        P1IES ^= buttonToTest;
        P1IFG &= ~buttonToTest;
        return pressed;
    }
}
