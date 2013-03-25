#include "LaserRainbow.h"
#include "adc_sampler.h"
#include "adc.h"

lzr_sample *samples;

lzr_sample *ptr_sample_write;

lzr_sample *ptr_first;
lzr_sample *ptr_last;

lzr_sample *ptr_last_read;

uint8_t flag_has_data;
uint8_t is_debug;
uint16_t debug_sample;

void lzr_oci_init(void) 
{
	samples = (lzr_sample*)malloc( sizeof(lzr_sample) * MAX_NUM_SAMPLES );
	
	ptr_first = &samples[0];
	ptr_last = &samples[MAX_NUM_SAMPLES-1]; 

	ptr_sample_write = ptr_first;
	ptr_last_read = ptr_first;

	flag_has_data = 0;
	is_debug = 0;
	debug_sample = 0;
}

void lzr_oci_destroy(void)
{
	free(samples);
}

void lzr_oci_debug( uint8_t debug )
{
	is_debug = debug;
}

uint8_t lzr_oci_samples_ready(void)
{
	return flag_has_data;
}

void lzr_oci_read_sample( lzr_sample *sample ) 
{
	uint32_t i;

	for ( i = 0; i < NUM_CHANNELS; ++i ) {
		if ( is_debug == 1 )
			sample->channels[i] = (i << 8) + debug_sample;
		else
			sample->channels[i] = adc_read(i);
	}
}

void lzr_oci_push_sample(void)
{
	lzr_oci_read_sample( ptr_sample_write );
	++ptr_sample_write;
	++debug_sample;

	if ( ptr_sample_write > ptr_last )
		ptr_sample_write = ptr_first;

	if ( ptr_sample_write == ptr_last_read )
		flag_has_data = 1;
}

void lzr_oci_read( lzr_sample *out )
{
	uint32_t i;
	lzr_sample *ptr;

	ptr_last_read = ptr_sample_write;
	flag_has_data = 0;
	debug_sample = 0;

	ptr = ptr_sample_write;
	for ( i = 0; i < MAX_NUM_SAMPLES; ++i ) {
		
		out[i] = samples[i]; 

		ptr++;
		if ( ptr > ptr_last )
			ptr = ptr_first;
	}
}