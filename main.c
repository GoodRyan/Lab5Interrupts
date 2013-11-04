#include <msp430.h> 
#include "game.h"
#include "lcd.h"
#include "button.h"

void init_timer();
void init_buttons();

char TIMER = 0; //tracks if game is in progress or lost
char LOSE = 0;
char BUTTON_PRESSED = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initSPI();

    LCDinit();

    LCDclear();

    unsigned char player = initPlayer();
    int gameOver = 0;

    init_timer();
    init_buttons();
    __enable_interrupt();
   	printPlayer(player);
    while(1)
    {
    	if (BUTTON_PRESSED != 0)
    	{
    		clearPlayer(player);

			if(BUTTON_PRESSED == 1){
				player = movePlayer(player, RIGHT);
			}
			else if(BUTTON_PRESSED == 2){
				player = movePlayer(player, LEFT);
			}
			else if(BUTTON_PRESSED == 3){
				player = movePlayer(player, UP);
			}
			else if(BUTTON_PRESSED == 4){
				player = movePlayer(player, DOWN);
			}

    		printPlayer(player);

    		TAR = 0;
    		TIMER = 0;

        	BUTTON_PRESSED = 0;
    	}

    	if(LOSE == 1){
    		LCDclear();
    		print("GAME");
    		secondLine();
    		print("OVER");
    		firstLine();
    		gameOver = 1;
    		waitForP1ButtonRelease(BIT1|BIT2|BIT3|BIT4);
    		debounce();
    	}
    	if(didPlayerWin(player)){
    		//debounce();
    		LCDclear();
    		print("YOU");
    		secondLine();
    		print("WON");
    		firstLine();
    		gameOver = 1;
    		//waitForP1ButtonRelease(BIT1|BIT2|BIT3|BIT4);
    		waitForP1ButtonRelease(BIT1);
    		waitForP1ButtonRelease(BIT2);
    		waitForP1ButtonRelease(BIT3);
    		waitForP1ButtonRelease(BIT4);
    	}
    	if(gameOver){
    		char buttonsToPoll[4] = {BIT1, BIT2, BIT3, BIT4};
    		while(!pollP1Buttons(buttonsToPoll, 4)){
    			//poll until something is pressed
    		}
    		TAR = 0;
    		LOSE = 0;
    		TIMER = 0;
    		gameOver = 0;
    		LCDclear();
    		player = initPlayer();
    		printPlayer(player);
    	}
    }

    return 0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
    TACTL &= ~TAIFG;            // clear interrupt flag
    TIMER += 1;
    if (TIMER == 4){
    	LOSE = 1;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
	if(testAndRespondToButtonPush(BIT1)){
		BUTTON_PRESSED = 1;
		P1IFG &= ~BIT1;
	}
	else if(testAndRespondToButtonPush(BIT2)){
		BUTTON_PRESSED = 2;
		P1IFG &= ~BIT2;
	}
	else if(testAndRespondToButtonPush(BIT3)){
		BUTTON_PRESSED = 3;
		P1IFG &= ~BIT3;
	}
	else if(testAndRespondToButtonPush(BIT4)){
		BUTTON_PRESSED = 4;
		P1IFG &= ~BIT4;
	}
}

void init_timer()
{
	 TACTL &= ~(MC1|MC0);
	 TACTL |= TACLR;
	 TACTL |= TASSEL1;
	 TACTL |= ID1|ID0;
	 TACTL &= ~TAIFG;
	 TACTL |= MC1;
	 TACTL |= TAIE;
}

void init_buttons()
{
	configureP1PinAsButton(BIT1|BIT2|BIT3|BIT4);

}

