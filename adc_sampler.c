#include "LaserRainbow.h"
#include "adc_sampler.h"
#include "adc.h"

uint8_t count_set_16bits( uint16_t bits );

lzr_sample *samples;

lzr_sample *ptr_sample_write;

lzr_sample *ptr_first;
lzr_sample *ptr_last;

lzr_sample *ptr_last_read;

uint8_t flag_has_data;
uint8_t debug_flag;
uint16_t debug_sample;
uint16_t debug_sin_offset;

uint16_t channels;
uint8_t num_channels;
uint8_t num_samples;

void lzr_oci_init(uint16_t in_channels) 
{
	uint8_t i;

	channels = in_channels;

	num_channels = count_set_16bits(channels);
	if ( num_channels > MAX_NUM_CHANNELS )
		num_channels = MAX_NUM_CHANNELS;

	// max package size is MAX_BYTES_TO_SAMPLE byte, determine number of samples base on that and number of channels
	num_samples = MAX_UINT16_TO_SAMPLE / num_channels;

	samples = (lzr_sample*)malloc( sizeof(lzr_sample) * (uint32_t)num_samples );

	for ( i = 0; i < num_samples; ++i ) {
		samples[i].channels = (uint16_t*)malloc( sizeof(uint16_t) * num_channels );
	}

	ptr_first = &samples[0];
	ptr_last = &samples[num_samples-1]; 

	ptr_sample_write = ptr_first;
	ptr_last_read = ptr_first;

	flag_has_data = 0;
	debug_flag = DEBUG_NONE;
	debug_sample = 0;
	debug_sin_offset = 0;
}

void lzr_oci_destroy(void)
{
	free(samples1);
	free(samples2);
}

void lzr_oci_debug( uint8_t flag )
{
	debug_flag = flag;
}

uint8_t lzr_oci_samples_ready(void)
{
	return flag_has_data;
}

uint16_t lzr_oci_read_sample_value( uint8_t channel_index ) {
	if ( debug_flag == DEBUG_CHANNEL_SAMPLE )
		return (channel_index << 8) + debug_sample;
	else if ( debug_flag == DEBUG_SIN ) {
		if ( (++debug_sin_offset) == 360 ) {
			debug_sin_offset = 0;
		}
		return (uint16_t) ( (1.0f + sin( .0174444 * (float) ((debug_sin_offset)) ) ) * 512.f );
	}
	else
		return adc_read(channel_index);
}

void lzr_oci_read_sample( lzr_sample *sample ) 
{
	uint8_t i, ii;

	for ( i = 0, ii=0; i < MAX_NUM_CHANNELS; ++i ) {

		if ( (channels & (1<<i)) != 0) {
			//???
			sample->channels[ii] = lzr_oci_read_sample_value(i);
			++ii;
		}
		
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

void lzr_oci_read( uint16_t *out )
{
	uint32_t i, ii, idx;
	
	ptr_last_read = ptr_sample_write;
	
	flag_has_data = 0;
	debug_sample = 0;
	idx = 0;
	
	for ( i = 0; i < num_samples; ++i ) {

		for ( ii = 0; ii < num_channels; ++ii  )
			out[idx++] = samples[i].channels[ii]; 

	}

	if ( (++debug_sin_offset) == 360 ) {
		debug_sin_offset = 0;
	}
}

/** utils **/
uint8_t count_set_16bits( uint16_t bits ) {
	uint8_t i, num_set;

	num_set = 0;
	for ( i = 0; i < 16; ++i ) {
		if ( (bits & (1<<i)) != 0 )
			++num_set;
	}

	return num_set;
}