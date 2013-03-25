/*
 *
 *  Main source file for the RealFps project.
 */


#include "LaserRainbow.h"
#include "adc.h"
#include "adc_sampler.h"

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

static uint16_t counter;
static uint16_t cnt;

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
	cnt = 0;
	counter = 0;

	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	USB_Init();

	adc_init();

	/* PORTB setup */
	DDRB = LEDS_ALL_LEDS;
	PORTB = LEDS_LED1;
	/*DDRF = 0x00;*/

	sei();

	lzr_oci_init();
	lzr_oci_debug(1);

	for (;;)
	{
		/* Read ADCs */

		HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();


		if (counter == 32768) {
			counter = 0;
			PORTB ^= LEDS_LED1;
		}
		if ( cnt == 512 ) {
			lzr_oci_push_sample();
			cnt = 0;
		}
		counter++;
		cnt++;
	}

	lzr_oci_destroy();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	PORTB ^= LEDS_LED2;
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	PORTB ^= LEDS_LED2;
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
	/*
	Lzr_report* report = (Lzr_report*)ReportData;
	report->a = 1;
	report->b = cnt;
	report->c = -1;
	report->adc0 = adc_read(0);
	report->adc1 = adc_read(1);
	*ReportSize = GENERIC_REPORT_SIZE;
	cnt++;
	*/

	/*
	Lzr_oscilloscope* report = (Lzr_oscilloscope*)ReportData;
	report->t = counter;
	report->adc0 = adc_read(0);
	*ReportSize = GENERIC_REPORT_SIZE;
	
	++cnt;
	*/

	/*
	uint8_t *data = ReportData;
	for ( uint16_t i=0; i < 128; ++i ) {
		data[i] = i;
 	}

 	data[0] = (uint8_t)(counter << 8);
 	data[1] = (uint8_t)counter;

	*ReportSize = GENERIC_REPORT_SIZE;
	*/

	if ( lzr_oci_samples_ready() ) {
		uint8_t *data = ReportData;
	
		lzr_oci_read( ReportData );
		
		data[60] = 'A';
		data[61] = 'B';
		data[62] = 'C';
		data[63] = '\0';
		/*data[124] = 'E';
		data[125] = 'F';
		data[126] = 'G';
		data[127] = '\0';
		*/
		

		*ReportSize = GENERIC_REPORT_SIZE;

		return true;
	}

	return false;
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
	// Unused (but mandatory for the HID class driver) in this demo, since there are no Host->Device reports
}