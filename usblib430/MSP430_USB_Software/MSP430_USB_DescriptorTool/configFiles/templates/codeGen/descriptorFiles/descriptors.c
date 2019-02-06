<MUDTCG_COPYRIGHT>
<MUDTCG_DATE>
/*-----------------------------------------------------------------------------+
| Include files 
|-----------------------------------------------------------------------------*/
#include <USB_API/USB_Common/device.h>
#include <USB_API/USB_Common/defMSP430USB.h>
#include <USB_API/USB_Common/usb.h>              // USB-specific Data Structures
#include "descriptors.h"
<MUDTCG_DESC_C_INCLUDES_FOR_CDC_OR_HID>
<MUDTCG_DESC_C_INCLUDES_FOR_MSC>
<MUDTCG_DESC_C_INCLUDES_FOR_PHDC>

<MUDTCG_DESC_C_GLOBALS_FOR_CDC_OR_HID>

/*-----------------------------------------------------------------------------+
| Device Descriptor 
|-----------------------------------------------------------------------------*/
uint8_t const abromDeviceDescriptor[SIZEOF_DEVICE_DESCRIPTOR] = {
<MUDTCG_DESC_C_ABROMDEVICEDESC_STRUCT>
};

/*-----------------------------------------------------------------------------+
| Configuration Descriptor                                                     |
|-----------------------------------------------------------------------------*/
const struct abromConfigurationDescriptorGroup abromConfigurationDescriptorGroup=
{
    /* Generic part */
    {
        // CONFIGURATION DESCRIPTOR (9 bytes)
        SIZEOF_CONFIG_DESCRIPTOR,                          // bLength
        DESC_TYPE_CONFIG,                                  // bDescriptorType
        DESCRIPTOR_TOTAL_LENGTH, 0x00,                     // wTotalLength
        USB_NUM_INTERFACES,                                // bNumInterfaces
        USB_CONFIG_VALUE,                                  // bConfigurationvalue
        CONFIG_STRING_INDEX,                               // iConfiguration Description offset
        USB_SUPPORT_SELF_POWERED | USB_SUPPORT_REM_WAKE,   // bmAttributes, bus power, remote wakeup
        USB_MAX_POWER                                      // Max. Power Consumption
    },
<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_MSC_STRUCT>
<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_CDC_STRUCT>
<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_HID_STRUCT>
<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_PHDC_STRUCT>	
};

/*-----------------------------------------------------------------------------+
| String Descriptor                                                            |
|-----------------------------------------------------------------------------*/
uint8_t const abromStringDescriptor[] = {
<MUDTCG_DESC_C_ABROMSTRINGDESC>
};

<MUDTCG_DESC_C_HID_REPORT_DESC>

<MUDTCG_DESC_C_HID_REPORT_DESC_POINTERS>

/**** Populating the endpoint information handle here ****/
const struct tUsbHandle stUsbHandle[]=
{
<MUDTCG_DESC_C_HID_USBHANDLE>
};

//-------------DEVICE REQUEST LIST---------------------------------------------

const tDEVICE_REQUEST_COMPARE tUsbRequestList[] = 
{

<MUDTCG_CDC_REQUEST_LIST>
<MUDTCG_HID_REQUEST_LIST>
<MUDTCG_MSC_REQUEST_LIST>
<MUDTCG_PHDC_REQUEST_LIST>

    {
        //---- USB Standard Requests -----//
            // clear device feature
            USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_CLEAR_FEATURE,
            FEATURE_REMOTE_WAKEUP,0x00,         // feature selector
            0x00,0x00,
            0x00,0x00,
            0xff,&usbClearDeviceFeature,
    },
    {

            // clear endpoint feature
            USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_ENDPOINT,
            USB_REQ_CLEAR_FEATURE,
            FEATURE_ENDPOINT_STALL,0x00,
            0xff,0x00,
            0x00,0x00,
            0xf7,&usbClearEndpointFeature,
    },
    {
            // get configuration
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_GET_CONFIGURATION,
            0x00,0x00, 
            0x00,0x00, 
            0x01,0x00,
            0xff,&usbGetConfiguration,
    },
    {
            // get device descriptor
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_GET_DESCRIPTOR,
            0xff,DESC_TYPE_DEVICE,              // bValueL is index and bValueH is type
            0xff,0xff,
            0xff,0xff,
            0xd0,&usbGetDeviceDescriptor,
    },
    {
            // get configuration descriptor
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_GET_DESCRIPTOR,
            0xff,DESC_TYPE_CONFIG,              // bValueL is index and bValueH is type
            0xff,0xff,
            0xff,0xff,
            0xd0,&usbGetConfigurationDescriptor,
    },
    {
            // get string descriptor
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_GET_DESCRIPTOR,
            0xff,DESC_TYPE_STRING,              // bValueL is index and bValueH is type
            0xff,0xff,
            0xff,0xff,
            0xd0,&usbGetStringDescriptor,
    },
    {
           // get interface
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
            USB_REQ_GET_INTERFACE,
            0x00,0x00,
            0xff,0xff,
            0x01,0x00,
            0xf3,&usbGetInterface,
    },
    {
            // get device status
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_GET_STATUS,
            0x00,0x00,
            0x00,0x00,
            0x02,0x00,
            0xff,&usbGetDeviceStatus, 
    },
    {
            // get interface status
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
            USB_REQ_GET_STATUS,
            0x00,0x00,
            0xff,0x00,
            0x02,0x00,
            0xf7,&usbGetInterfaceStatus,
    },
    {
            // get endpoint status
            USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_ENDPOINT,
            USB_REQ_GET_STATUS,
            0x00,0x00,
            0xff,0x00,
            0x02,0x00,
            0xf7,&usbGetEndpointStatus,
    },
    {
            // set address
            USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_SET_ADDRESS,
            0xff,0x00,
            0x00,0x00,
            0x00,0x00,
            0xdf,&usbSetAddress,
    },
    {
            // set configuration
            USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_SET_CONFIGURATION,
            0xff,0x00,
            0x00,0x00,
            0x00,0x00,
            0xdf,&usbSetConfiguration,
    },
    {
           // set device feature
            USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE,
            USB_REQ_SET_FEATURE,
            0xff,0x00,                      // feature selector
            0x00,0x00,
            0x00,0x00,
            0xdf,&usbSetDeviceFeature,
    },
    {
            // set endpoint feature
            USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_ENDPOINT,
            USB_REQ_SET_FEATURE,
            0xff,0x00,                      // feature selector
            0xff,0x00,                      // endpoint number <= 127
            0x00,0x00,
            0xd7,&usbSetEndpointFeature,
    },
    {
            // set interface
            USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
            USB_REQ_SET_INTERFACE,
            0xff,0x00,                      // feature selector
            0xff,0x00,                      // interface number
            0x00,0x00,
            0xd7,&usbSetInterface,
    },
    {

            // end of usb descriptor -- this one will be matched to any USB request
            // since bCompareMask is 0x00.
            0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, 
            0x00,&usbInvalidRequest     // end of list
    }
	
};

<MUDTCG_MSC_DESC_INFO>

/*-----------------------------------------------------------------------------+
| END OF Descriptor.c FILE
|-----------------------------------------------------------------------------*/


//Released_Version_5_20_06_02
