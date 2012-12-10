/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>

#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator

int main (void)
{
	board_init();

	// Insert application code here, after the board has been initialized.
	PORTB_DIR = 0xa8;				// PORTB as OUTPUT
	PORTB_OUT = 0b01001010;				// All pins of PORTB LOW

	PORTC_DIR = 0x4c;				// PORTB as OUTPUT
	PORTC_OUT = 0b01001010;				// All pins of PORTB LOW

	unsigned int i;
	while(1 == 1)					//Infinite loop
	{
	//
		//PORTB_OUT = 0x0f;				// All pins of PORTB LOW
//
		//for(i=0;i<10;i++) { }
//
		////for(i=0;i<65535;i++);	//delay
		////for(i=0;i<65535;i++);	//delay
////
		//PORTB_OUT = 0x00;				// All pins of PORTB LOW
		////tbi(PORTB, PB0);			//here the toggling takes place
	}	
}
