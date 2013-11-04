#include <msp430.h> 
#include "game.h"
#include "lcd.h"
#include "button.h"

void init_timer();
void init_buttons();

int TIMER = 0; //tracks if game is in progress or lost
int LOSE = 0;
int GAMEOVER = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initSPI();

    LCDinit();

    LCDclear();

    unsigned char player = initPlayer();

    init_timer();
    init_buttons();
    __enable_interrupt();
   	printPlayer(player);
    while(1)
    {

    	player = movementCheck(player);
    	if(LOSE == 1){
    		LCDclear();
    		print("GAME");
    		secondLine();
    		print("OVER");
    		firstLine();
    		GAMEOVER = 1;
    		waitForP1ButtonRelease(BIT1|BIT2|BIT3|BIT4);
    		debounce();
    	}
    	if(didPlayerWin(player)){
    		LCDclear();
    		print("YOU");
    		secondLine();
    		print("WON");
    		firstLine();
    		GAMEOVER = 1;
    		waitForP1ButtonRelease(BIT1|BIT2|BIT3|BIT4);
    		debounce();
    	}
    	if(GAMEOVER){
    		char buttonsToPoll[4] = {BIT1, BIT2, BIT3, BIT4};
    		while(!pollP1Buttons(buttonsToPoll, 4)){
    			//poll until something is pressed
    		}
    		TAR = 0;
    		LOSE = 0;
    		TIMER = 0;
    		GAMEOVER = 0;
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

