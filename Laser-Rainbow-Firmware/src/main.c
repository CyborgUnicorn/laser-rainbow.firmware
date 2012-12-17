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
//#include <asf.h>
//#include "board.h"
#include <avr/io.h>
#include "config/compiler.h"
#include "usb/usb_standard_request.h"

#define tbi(x,y) x ^= _BV(y)		//toggle bit - using bitwise XOR operator

void board_init();

void sof_action()
{
	//cpt_sof++;
}

int main (void)
{
	board_init();

	 //Usb_enable_regulator();

	// Insert application code here, after the board has been initialized.
	//PORTA_DIR = 0xff;					// PORTB as OUTPUT
	//PORTA_OUT = 0b01001010;				// All pins of PORTB LOW
	//
	//PORTB_DIR = 0xff;					// PORTB as OUTPUT
	//PORTB_OUT = 0b01001010;				// All pins of PORTB LOW
//
	//PORTC_DIR = 0xff;					// PORTB as OUTPUT
	//PORTC_OUT = 0b01001010;				// All pins of PORTB LOW
//
	//PORTD_DIR = 0xff;					// PORTB as OUTPUT
	//PORTD_OUT = 0b01001010;				// All pins of PORTB LOW
//
//
	//PORTA_OUT = 0x00;
	//
	//unsigned int i = 0;
	while(1 == 1)						//Infinite loop
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

void board_init()
{
}

void configurePorts()
{
}

void configureUsb()
{
}

