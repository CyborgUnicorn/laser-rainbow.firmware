/** \file
 *
 *  Header file for LEDNotifier.c.
 */

#ifndef _LASERRAINBOW_H_
#define _LASERRAINBOW_H_

#define USB_CAN_BE_DEVICE
#define __INCLUDE_FROM_LEDS_H

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <string.h>
		#include <stdio.h>

		#include "Descriptors.h"

		#include "Config/LEDs.h"
		#include <LUFA/Drivers/USB/USB.h>

	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);

#endif

