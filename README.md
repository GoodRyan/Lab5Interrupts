Lab5Interrupts
==============

A Simple Game

General Notes
=============
Buttons are assumed to be active low. The original buttons library was slightly altered to properly initialize the buttons
for their new intended use.

The only library original to this program is game.c/game.h and thus these are the libraries that will be discussed.

Functions
=========

unsigned char initPlayer() - When a player variable is assigned to this value, they will be placed at 0x80, the top left
hand corner of the screen.

void printPlayer(unsigned char player) - when given a player variable, prints the player at their current location.

void clearPlayer(unsigned char player) - when given a player variable, clears the player from the screen.

unsigned char movePlayer(unsigned char player, unsigned char direction) - when given a player and a direction,
moves the player in the desired direction as long as that direction is not off of the screen.

char didPlayerWin(unsigned char player) - given a player variable, returns true if the player has reached the bottom
right hand corner of the screen (0xC7) and false in any other case.

unsigned char movementCHeck(unsigned char player, char button) - polls to see if a button has been pressed (this is not
used in Bfunctionality).

char testAndRespondToButtonPush(char buttonToTest) - Facilitates the button responses via interrupt, returning
true when the button in question has been pressed (replaces movementCheck for Bfunctionality).


License
=======
Anyone is free to use and modify this for any purpose, provided they document it.
