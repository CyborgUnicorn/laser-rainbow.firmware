/** \file
 *
 *  Header file for LEDNotifier.c.
 */

#ifndef _LASERRAINBOW_H_
#define _LASERRAINBOW_H_

#define USB_CAN_BE_DEVICE

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <string.h>
		#include <stdio.h>

		#include "lzr_defines.h"

		#include "Descriptors.h"

		//#include <LUFA/Drivers/Board/LEDS.h>
		#include <LUFA/Drivers/USB/USB.h>


	/* Function Prototypes: */
		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);

		void SetupHardware(void);

		/* Macros: */
			/** LED mask for the first LED on the board. */
			#define LEDS_LED1        (1 << 7)

			#define LEDS_LED2		 (1)

			/** LED mask for all the LEDs on the board. */
			#define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2)

			/** LED mask for the none of the board LEDs. */
			#define LEDS_NO_LEDS     0


#endif