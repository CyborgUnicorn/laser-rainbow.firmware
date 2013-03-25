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
	uint16_t t;
	uint16_t adc0;
} Lzr_oscilloscope;

typedef struct {
	uint32_t t;
	uint8_t v;
} Lzr_voltage;

typedef struct {
	uint16_t cnt; // count
	uint16_t rh; // humidity
	uint16_t t; // temperature
	uint8_t chk; // checksum
} Lzr_humi_temp;


 #define GENERIC_REPORT_SIZE 64 //sizeof(Lzr_humi_temp)

#endif 