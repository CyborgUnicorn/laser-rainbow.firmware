/** \file
 *
 *  Main source file for the LEDNotfier project. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "LaserRainbow.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 0,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** Counter for the software PWM */
static volatile uint8_t SoftPWM_Count;

/** Duty cycle for the first software PWM channel */
static volatile uint8_t SoftPWM_Channel1_Duty;

/** Duty cycle for the second software PWM channel */
static volatile uint8_t SoftPWM_Channel2_Duty;

/** Duty cycle for the third software PWM channel */
static volatile uint8_t SoftPWM_Channel3_Duty;

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs
 */
static FILE USBSerialStream;


/** Interrupt handler for managing the software PWM channels for the LEDs */
ISR(TIMER0_COMPA_vect, ISR_BLOCK)
{
	/*
	 *	Handle data from sensors
	 */
	/*uint8_t LEDMask = LEDS_ALL_LEDS;

	if (++SoftPWM_Count == 0b00011111)
	  SoftPWM_Count = 0;

	if (SoftPWM_Count >= SoftPWM_Channel1_Duty)
	  LEDMask &= ~LEDS_LED1;

	if (SoftPWM_Count >= SoftPWM_Channel2_Duty)
	  LEDMask &= ~LEDS_LED2;

	LEDs_SetAllLEDs(LEDMask);*/
}

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	/* Create a regular blocking character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateBlockingStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	sei();

	/*
	 *	Start listening for USB commands
	 */

	for (;;)
	{
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	//LEDs_Init();
	USB_Init();

	/* Timer Initialization */
	OCR0A  = 100;
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00);
	TIMSK0 = (1 << OCIE0A);

	/* PORTB setup */
	DDRB = 0x81;
	//PORTB = LEDS_LED1;
	//LEDs_TurnOnLEDs(LEDS_LED2);
}	

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	// WORKING
	/*if (USB_ControlRequest.bmRequestType == 0x40) {
		PORTB = 0x80;
	} else {
		PORTB = 0x01;
	}*/

	if ((USB_ControlRequest.bmRequestType == 0x40) && (USB_ControlRequest.bRequest == 7) && (USB_ControlRequest.wIndex == 1)) {
		if ( USB_ControlRequest.wValue == 1 )
			PORTB |= (0x80);
		else
			PORTB &= ~(0x80);
	} else if ((USB_ControlRequest.bmRequestType == 0x40) && (USB_ControlRequest.bRequest == 7) && (USB_ControlRequest.wIndex == 2)) {
		if ( USB_ControlRequest.wValue == 1 )
			PORTB |= (0x01);
		else
			PORTB &= ~(0x01);
	}

	//CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
	/*if(USB_ControlRequest.bmRequestType == 0x40)
	if(((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) == REQTYPE_CLASS) && ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_DEVICE))
	{
		if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION) == REQDIR_HOSTTODEVICE)
		{
			switch(USB_ControlRequest.bRequest)
			{
				case LED_CMD_SET:
					process_LED_CMD_SET();
					break;
				default:
					break;
		}
	} else {
			switch(USB_ControlRequest.bRequest)
			{
				case SERVO_CMD_GET:
					process_SERVO_CMD_GET();
					break;
				case SERVO_CMD_GETALL:
					process_SERVO_CMD_GETALL();
					break;
				default:
					break;
			}
		}
	}*/
}

void process_LED_CMD_SET(void) {

	/* marks the command as "accepted" by the
	application, so that LUFA does not process it: */
	Endpoint_ClearSETUP();

	/* mark the whole request as successful: */
	Endpoint_ClearStatusStage();

	/* process command parameters: */
	switch(USB_ControlRequest.wIndex) {
		case 1:
			PORTB |= (LEDS_LED1 & USB_ControlRequest.wValue);
			break;
		case 2:
			PORTB |= (LEDS_LED2 & USB_ControlRequest.wValue);
			break;
	}
}
