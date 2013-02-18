#ifndef _LZR_DEFINES_H_
#define _LZR_DEFINES_H_

typedef struct {
	unsigned char a;
	unsigned char b;
	unsigned char c;

	uint16_t adc0;
	uint16_t adc1;
} Lzr_report;

typedef struct {
	uint32_t t;
	uint8_t v;
} Lzr_voltage;

#endif 