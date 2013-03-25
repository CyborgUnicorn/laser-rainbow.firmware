#ifndef _ADC_SAMPLER_H_
#define _ADC_SAMPLER_H_

#define NUM_CHANNELS 6
#define MAX_NUM_SAMPLES 5

typedef struct {
	uint16_t channels[NUM_CHANNELS];	
} lzr_sample;

void lzr_oci_debug( uint8_t debug );

void lzr_oci_init(void);

void lzr_oci_destroy(void);

uint8_t lzr_oci_samples_ready(void);

void lzr_oci_push_sample(void);

void lzr_oci_read( lzr_sample *out );


#endif