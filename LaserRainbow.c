#include <util/delay.h>
#include "LaserRainbow.h"

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

static uint16_t counter;
/** HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Generic_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = 0,
				.ReportINEndpoint             =
					{
						.Address              = GENERIC_IN_EPADDR,
						.Size                 = GENERIC_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
			},
	};


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	counter = 0;

	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	USB_Init();

	/* PORTB setup */
	DDRB = LEDS_ALL_LEDS;
	PORTB = LEDS_LED1;

	sei();

	GlobalInterruptEnable();

	for (;;)
	{
		/* Read ADCs */

		HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();
		
		if (counter == 32768) {
			counter = 0;
			//PORTB ^= LEDS_LED1;
		}
		counter++;
		
		//ReadStupidPin();
	}
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	//PORTB ^= LEDS_LED2;
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	//PORTB ^= LEDS_LED2;
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);
	USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}

uint16_t rh = 0;
uint16_t t = 0;
uint8_t chk = 0;

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean true to force the sending of the report, false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
	//PORTB ^= LEDS_LED2;
	uint8_t* Data = (uint8_t*)ReportData;
	uint16_t i = 0;


		Data[0] = 'R';
		Data[1] = 'H';
		Data[2] = '=';
		Data[3] = (uint8_t)(rh >> 8);
		Data[4] = (uint8_t)(rh);
		Data[5] = ',';
		Data[6] = 'T';
		Data[7] = '=';
		Data[8] = (uint8_t)(t >> 8);
		Data[9] = (uint8_t)(t);
		Data[10] = ',';
		Data[11] = 'c';
		Data[12] = 'h';
		Data[13] = 'k';
		Data[14] = '=';
		Data[15] = chk;
		Data[16] = 'Q';
		Data[17] = 'R';
		Data[18] = 'S';
		Data[19] = 'T';
		Data[20] = 'U';
		Data[21] = 'V';
		Data[22] = 'W';
		Data[23] = 'X';

		*ReportSize = GENERIC_REPORT_SIZE;

	//PORTB ^= LEDS_LED1;

	return true;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
	/*if(latestWriteSize > 0) {
		free(latestWrite);
	}*/

	/*hasWritten = 1;

	latestWriteSize = ReportSize * sizeof(uint8_t);
	latestWrite = malloc(latestWriteSize);
	memcpy(latestWrite, ReportData, latestWriteSize);
	latestReportId = ReportID;
	latestReportType = ReportType;
*/
}

void ReadStupidPin() {

#define VCCPIN 5
#define DATAPIN 6
#define DATA_IS_HIGH (PINF &= (1 << DATAPIN) == 1)
#define DATA_IS_LOW (PINF |= (0 << DATAPIN) == 0)

	uint8_t i = 0;

	DDRF |= (1 << VCCPIN);	// Set VCC pin as output
	PORTF |= (1 << VCCPIN);  // Enable VCC

	// PF5 = VCC
	// PF6 = Data

	// Pull down 1-10 ms
	// Pull up 20-40 us
	// Wait for response
	DDRF |= (1 << DATAPIN);	// Data as output
	PORTF |= (1 << DATAPIN);	// High first...
	_delay_us(5);
	PORTF |= (0 << DATAPIN);	// then low for 5 ms...
	_delay_ms(5);
	PORTF |= (1 << DATAPIN);	// then high for 30 us...
	_delay_us(30);
	DDRF |= (0 << DATAPIN);	// Data as input
	// Sensor does stuff here

	// Sensor pulls low for 80 us
	// Sensor pulls high for 80 us 
	// Actual data
	// Low voltage for 50 us
	// Actual bit

	while(DATA_IS_LOW);
	while(DATA_IS_HIGH);

	// Humidity
	for(i = 0; i < 16; i++) {
		while(DATA_IS_LOW);
		_delay_us(35);

		if (DATA_IS_HIGH) {
			rh |= 0x01 << i;
		}
	}

	for(i = 0; i < 16; i++) {
		while(DATA_IS_LOW);
		_delay_us(35);

		if (DATA_IS_HIGH) {
			t |= 0x01 << i;
		}
	}

	// checksum
	for(i = 0; i < 8; i++) {
		while(DATA_IS_LOW);
		_delay_us(35);

		if (DATA_IS_HIGH) {
			chk |= 0x01 << i;
		}
	}

	if (((uint8_t)(rh >> 8) + (uint8_t)rh + (uint8_t)(t >> 8) + (uint8_t)t) != chk) {
		// ERRROROROROR!
	} else {
		PORTB ^= LEDS_LED2;
	}
}	