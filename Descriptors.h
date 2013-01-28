/** \file
 *
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		#include <avr/pgmspace.h>

		#include <LUFA/Drivers/USB/USB.h>

 		
 	#define GENERIC_REPORT_SIZE 24
	
	/* Type Defines: */
		/** Type define for the device configuration descriptor structure. This must be defined in the
		 *  application code, as the configuration descriptor contains several sub-descriptors which
		 *  vary between devices, and which describe the device's usage to the host.
		 */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;

			// Generic HID Interface
			USB_Descriptor_Interface_t            HID_Interface;
			USB_HID_Descriptor_HID_t              HID_GenericHID;
	        USB_Descriptor_Endpoint_t             HID_ReportINEndpoint;
		} USB_Descriptor_Configuration_t;

	/* Macros: */
		/** Endpoint address of the Generic HID reporting IN endpoint. */
		#define GENERIC_IN_EPADDR         (ENDPOINT_DIR_IN | 1)

		/** Size in bytes of the Generic HID reporting endpoint. */
		#define GENERIC_EPSIZE            8

	/* Function Prototypes: */
		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
		                                    const uint8_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);
#endif
