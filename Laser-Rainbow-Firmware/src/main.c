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


//#define __AVR_ATmega32U4__

#include <avr/io.h>
#include <util/delay.h>

#include "config/config.h"

#include "usb/usb_drv.h"
#include "usb/scheduler.h"

void board_init();
void configure_usb();

int main (void)
{
	unsigned int i = 0;
	double delay = 100;
	board_init();
	
	PORTB = 0b10000000;
	
	configure_usb();
	
	//start_boot();
	
	
	/*
	while(1 == 1)						//Infinite loop
	{
		PORTB = 0b10000000;
		
		_delay_ms(delay);
				
		PORTB = 0b00000001;
		
		_delay_ms(delay);
		
		PORTB = 0b10000000;
		
		_delay_ms(delay);
		
		PORTB = 0b00000001;
		
		_delay_ms(delay);
		
	}	
	*/
}

void board_init()
{
	DDRB = 0b10000001;
	DDRC = 0x00;
	DDRD = 0x00;
}

void configure_usb()
{
	// Enable the USB pad regulator (whatever that means)
	Usb_enable_regulator();
	
	scheduler();
}