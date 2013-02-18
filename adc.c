/*
 *
 *  ADC source file for the RealFps project.
 */
 
#include "LaserRainbow.h"
#include "adc.h"

void adc_init(void) {
	/*
	  Enable ADC
	  -----------------------------------------------------------------------------
	  1 1 Internal 2.56V Voltage Reference with external capacitor on AREF pin
	  0 1 AVCC with external capacitor on AREF pin

	  ADMUX register
	  7 REFS1 Reference selection (bit 1)
	  6 REFS0 Reference selection (bit 0)
	  5 ADLAR Left adjust conversion result
	  4 -
	  3:0 Channel selection
	
	  ADCSRA register
	  7 ADEN ADC enable
	  6 ADSC ADC start conversion
	  5 ADAT auto trigger enable
	  4 ADIF ADC interrupt flag
	  3 ADIE Interrupt yadayada
	  2:0 Prescaler select bits
	*/

	/* Internal 2.56V Voltage Reference */
	ADMUX |= (1 << REFS1)|(1 << REFS0)/*|(1 << ADLAR)*/;

	/* Enable ADC with /64 prescaling */
	ADCSRA |= (1 << ADEN)|(1 << ADPS2)|(1 << ADPS1);
}

uint16_t adc_read(uint8_t channel) {
	uint16_t result = 0;

	/* Select channel */
	ADMUX = (ADMUX&0xF0)|channel;

	/* Start conversion */
	ADCSRA |= (1 << ADSC);

	/* Wait for conversion to complete (ADSC = 0) */
	/*while( (ADCSRA&(1<< ADSC)) == (1 << ADSC)) { }*/
	while( (ADCSRA&(1 << ADSC)) ) {
	}

	/* Read result, ADCL first because */
	result |= (ADCL);
	result |= (ADCH << 8);

	/* Return result */
	return result;
}

