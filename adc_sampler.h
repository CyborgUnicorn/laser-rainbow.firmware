#ifndef _ADC_SAMPLER_H_
#define _ADC_SAMPLER_H_

#define MAX_NUM_CHANNELS 		12
#define MAX_UINT16_TO_SAMPLE 	GENERIC_REPORT_SIZE / 2
#define MAX_UINT8_TO_SAMPLE 	GENERIC_REPORT_SIZE

#define DEBUG_NONE				0
#define DEBUG_CHANNEL_SAMPLE	1
#define DEBUG_SIN				2

typedef struct {
	uint16_t *channels;
} lzr_sample;

void lzr_oci_debug( uint8_t debug );

void lzr_oci_init(uint16_t in_channels);

void lzr_oci_destroy(void);

uint8_t lzr_oci_samples_ready(void);

void lzr_oci_push_sample(void);

void lzr_oci_read( uint16_t *out );


#endif