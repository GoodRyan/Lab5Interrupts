#include <msp430.h> 
#include "game.h"
#include "lcd.h"
#include "button.h"

void init_timer();
void init_buttons();

int TIMER = 0; //tracks if game is in progress or lost
int LOSE = 0;

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
    		while(!(isP1ButtonPressed(BIT1|BIT2|BIT3|BIT4))){
    			//wait for button press to start game again
    		}
    		waitForP1ButtonRelease(BIT1|BIT2|BIT3|BIT4);
    		TAR = 0; //clear the clock
    		LOSE = 0;
    		TIMER = 0;
    		LCDclear();
    		debounce();
    	}
    	if(didPlayerWin(player)){
    		LCDclear();
    		print("YOU");
    		secondLine();
    		print("WON");
    		firstLine();
    		while(!(isP1ButtonPressed(BIT1|BIT2|BIT3|BIT4))){
    			//wait for button press to start game again
    		}
    		waitForP1ButtonRelease(BIT1|BIT2|BIT3|BIT4);
    		TACTL |= TACLR; //clear timer
    		LCDclear();
    		debounce();
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

