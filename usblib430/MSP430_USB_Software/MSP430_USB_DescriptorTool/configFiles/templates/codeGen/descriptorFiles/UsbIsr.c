<MUDTCG_COPYRIGHT>
<MUDTCG_DATE>
/*-----------------------------------------------------------------------------+
| Include files                                                                |
|-----------------------------------------------------------------------------*/
#include <USB_API/USB_Common/device.h>        // Basic Type declarations
#include <USB_API/USB_Common/defMSP430USB.h>
#include "descriptors.h"
#include <USB_API/USB_Common/usb.h>           //USB-specific Data Structures
#include <USB_API/USB_Common/UsbIsr.h>
#include <string.h>
<MUDTCG_USBISR_C_INCLUDES>

/*----------------------------------------------------------------------------+
| External Variables                                                          |
+----------------------------------------------------------------------------*/
extern uint8_t  bFunctionSuspended;
extern __no_init tEDB0 __data16 tEndPoint0DescriptorBlock;
extern __no_init tEDB __data16 tInputEndPointDescriptorBlock[];
extern __no_init tEDB __data16 tOutputEndPointDescriptorBlock[];
extern volatile uint8_t bHostAsksUSBData;
extern volatile uint8_t bTransferInProgress;
extern volatile uint8_t bSecondUartTxDataCounter[];
extern volatile uint8_t* pbSecondUartTxData;
extern uint8_t bStatusAction;
extern uint16_t wUsbEventMask;
int16_t CdcToHostFromBuffer(uint8_t);
int16_t CdcToBufferFromHost(uint8_t);
int16_t CdcIsReceiveInProgress(uint8_t);
int16_t HidToHostFromBuffer(uint8_t);
int16_t HidToBufferFromHost(uint8_t);
int16_t HidIsReceiveInProgress(uint8_t);
extern uint16_t wUsbHidEventMask;
int16_t PHDCToHostFromBuffer(uint8_t);
int16_t PHDCToBufferFromHost(uint8_t);
int16_t PHDCIsReceiveInProgress(uint8_t);
uint16_t USB_determineFreq(void);
/*----------------------------------------------------------------------------+
| General Subroutines                                                         |
+----------------------------------------------------------------------------*/
<MUDTCG_USBISR_C_IUSBINTERRUPTHANDLER>
//Check if the setup interrupt is pending.
    //We need to check it before other interrupts,
    //to work around that the Setup Int has lower priority then Input Endpoint 0
    if (USBIFG & SETUPIFG)
    {
        bWakeUp = SetupPacketInterruptHandler();  
#ifdef USB10_WORKAROUND
        tEndPoint0DescriptorBlock.bIEPCNFG &= ~EPCNF_UBME; // Clear ME to gate off SETUPIFG clear event
        tEndPoint0DescriptorBlock.bOEPCNFG &= ~EPCNF_UBME; // Clear ME to gate off SETUPIFG clear event
#endif
        USBIFG &= ~SETUPIFG;    // clear the interrupt bit
#ifdef USB10_WORKAROUND
        tEndPoint0DescriptorBlock.bIEPCNFG |= EPCNF_UBME; // Set ME to continue with normal operation
        tEndPoint0DescriptorBlock.bOEPCNFG |= EPCNF_UBME; // Set ME to continue with normal operation
#endif
    }   
    switch (__even_in_range(USBVECINT & 0x3f, USBVECINT_OUTPUT_ENDPOINT7))
    {
    case USBVECINT_NONE:
        break;
    case USBVECINT_PWR_DROP:
      __no_operation();
        break;
    case USBVECINT_PLL_LOCK:
        break;
    case USBVECINT_PLL_SIGNAL:
        break;
    case USBVECINT_PLL_RANGE:
        if (wUsbEventMask & USB_CLOCK_FAULT_EVENT)
        {
            bWakeUp = USB_handleClockEvent();
        }
        break;
    case USBVECINT_PWR_VBUSOn:
        PWRVBUSonHandler();
        if (wUsbEventMask & USB_VBUS_ON_EVENT)
        {
            bWakeUp = USB_handleVbusOnEvent();
        }
        break;
    case USBVECINT_PWR_VBUSOff:
        PWRVBUSoffHandler();
        if (wUsbEventMask & USB_VBUS_OFF_EVENT)
        {
            bWakeUp = USB_handleVbusOffEvent();
        }
        break;
    case USBVECINT_USB_TIMESTAMP:
        break;
    case USBVECINT_INPUT_ENDPOINT0:
        IEP0InterruptHandler();
        break;
    case USBVECINT_OUTPUT_ENDPOINT0:
        OEP0InterruptHandler();
        break;
    case USBVECINT_RSTR:
        USB_reset();
        if (wUsbEventMask & USB_RESET_EVENT)
        {
            bWakeUp = USB_handleResetEvent();
        }
        break;
    case USBVECINT_SUSR:
        USB_suspend();
        if (wUsbEventMask & USB_SUSPENDED_EVENT)
        {
            bWakeUp = USB_handleSuspendEvent();
        }
        break;
    case USBVECINT_RESR:
        USB_resume();
        if (wUsbEventMask & USB_RESUME_EVENT)
        {
            bWakeUp = USB_handleResumeEvent();
        }
        //-- after resume we will wake up! Independ what event handler says.
        bWakeUp = TRUE;
        break;
    case USBVECINT_SETUP_PACKET_RECEIVED:
        // NAK both IEP and OEP enpoints
        tEndPoint0DescriptorBlock.bIEPBCNT = EPBCNT_NAK;
        tEndPoint0DescriptorBlock.bOEPBCNT = EPBCNT_NAK;
        SetupPacketInterruptHandler();
        break;
    case USBVECINT_STPOW_PACKET_RECEIVED:
        break;
	case USBVECINT_INPUT_ENDPOINT1:
<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
        break;
    case USBVECINT_INPUT_ENDPOINT2:
<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
         break;
    case USBVECINT_INPUT_ENDPOINT3:
<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
        break;
    case USBVECINT_INPUT_ENDPOINT4:
<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
		break;
    case USBVECINT_INPUT_ENDPOINT5:
<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
		break;
    case USBVECINT_INPUT_ENDPOINT6:
<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
		break;
    case USBVECINT_INPUT_ENDPOINT7:
<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
		break;
    case USBVECINT_OUTPUT_ENDPOINT1:
<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
	    break;
    case USBVECINT_OUTPUT_ENDPOINT2:
<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
        break;
    case USBVECINT_OUTPUT_ENDPOINT3:
<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
        break;
    case USBVECINT_OUTPUT_ENDPOINT4:
<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
        break;
    case USBVECINT_OUTPUT_ENDPOINT5:
<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
        break;
    case USBVECINT_OUTPUT_ENDPOINT6:
<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
        break;
    case USBVECINT_OUTPUT_ENDPOINT7:
<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
        break;
    default:
        break;
    }

<MUDTCG_USBISR_C_EXIT_LPM>
}
	


/*----------------------------------------------------------------------------+
| Interrupt Sub-routines                                                      |
+----------------------------------------------------------------------------*/
uint8_t SetupPacketInterruptHandler(void)
{
    uint8_t bTemp;
    uint8_t bWakeUp = FALSE;
    USBCTL |= FRSTE;      // Function Reset Connection Enable - set enable after first setup packet was received
    usbProcessNewSetupPacket:
    // copy the MSB of bmRequestType to DIR bit of USBCTL
    if((tSetupPacket.bmRequestType & USB_REQ_TYPE_INPUT) == USB_REQ_TYPE_INPUT)
    {
        USBCTL |= DIR;
    }
    else
    {
        USBCTL &= ~DIR;
    }
    bStatusAction = STATUS_ACTION_NOTHING;
    // clear out return data buffer
    for(bTemp=0; bTemp<USB_RETURN_DATA_LENGTH; bTemp++)
    {
        abUsbRequestReturnData[bTemp] = 0x00;
    }
    // decode and process the request
    bWakeUp = usbDecodeAndProcessUsbRequest();
    // check if there is another setup packet pending
    // if it is, abandon current one by NAKing both data endpoint 0
    if((USBIFG & STPOWIFG) != 0x00)
    {
        USBIFG &= ~(STPOWIFG | SETUPIFG);
        goto usbProcessNewSetupPacket;
    }
    return bWakeUp;
}

//----------------------------------------------------------------------------
void PWRVBUSoffHandler(void)
{
   uint16_t MCLKFreq = USB_determineFreq();
   uint16_t DelayConstant_250us = ((MCLKFreq >> 6) + (MCLKFreq >> 7) + (MCLKFreq >> 9));
   volatile uint16_t i, j;

    //wait 1 ms till enable USB 
    for(j = 0; j < 4; j++)
    {
        for (i = 0; i < (DelayConstant_250us); i++){
            _NOP();
        }
   }
    if (!(USBPWRCTL & USBBGVBV))
    {
        USBKEYPID   =    0x9628;        // set KEY and PID to 0x9628 -> access to configuration registers enabled
        bEnumerationStatus = 0x00;      // device is not enumerated
        bFunctionSuspended = FALSE;     // device is not suspended
        USBCNF     =    0;              // disable USB module
        USBPLLCTL  &=  ~UPLLEN;         // disable PLL
        USBPWRCTL &= ~(VBOFFIE + VBOFFIFG + SLDOEN);          // disable interrupt VBUSoff
        USBKEYPID   =    0x9600;        // access to configuration registers disabled
    }
}

//----------------------------------------------------------------------------
void PWRVBUSonHandler(void)
{
   uint16_t MCLKFreq = USB_determineFreq();
   uint16_t DelayConstant_250us = ((MCLKFreq >> 6) + (MCLKFreq >> 7) + (MCLKFreq >> 9));
   volatile uint16_t i, j;

    //wait 1 ms till enable USB 
    for(j = 0; j < 4; j++)
    {
        for (i = 0; i < (DelayConstant_250us); i++){
            _NOP();
        }
   }
    if (USBPWRCTL & USBBGVBV)                //Checking for USB Bandgap and VBUS valid before modifying USBPWRCTL
    {
        USBKEYPID =  0x9628;                // set KEY and PID to 0x9628 -> access to configuration registers enabled
        USBPWRCTL |= VBOFFIE;               // enable interrupt VBUSoff
        USBPWRCTL &= ~ (VBONIFG + VBOFFIFG);             // clean int flag (bouncing)
        USBKEYPID =  0x9600;                // access to configuration registers disabled
    }
}

//----------------------------------------------------------------------------
void IEP0InterruptHandler(void)
{
    USBCTL |= FRSTE;                              // Function Reset Connection Enable
    tEndPoint0DescriptorBlock.bOEPBCNT = 0x00;     
    if(bStatusAction == STATUS_ACTION_DATA_IN)
    {
        usbSendNextPacketOnIEP0();
    }
    else
    {
        tEndPoint0DescriptorBlock.bIEPCNFG |= EPCNF_STALL; // no more data
    }
}

//----------------------------------------------------------------------------
uint8_t OEP0InterruptHandler(void)
{
    uint8_t bWakeUp = FALSE;
    USBCTL |= FRSTE;                              // Function Reset Connection Enable
    tEndPoint0DescriptorBlock.bIEPBCNT = 0x00;    
    if(bStatusAction == STATUS_ACTION_DATA_OUT)
    {
        usbReceiveNextPacketOnOEP0();
        if(bStatusAction == STATUS_ACTION_NOTHING)
        {
#           ifdef _CDC_
                if(tSetupPacket.bRequest == USB_CDC_SET_LINE_CODING)
                {
                    bWakeUp = Handler_SetLineCoding();
                }
#          endif
#ifdef _HID_
                if (tSetupPacket.bRequest == USB_REQ_SET_REPORT) {
                    bWakeUp = USBHID_handleEP0SetReportDataAvailable(tSetupPacket.wIndex);
                }
#endif
          }
    }
    else
    {
        tEndPoint0DescriptorBlock.bOEPCNFG |= EPCNF_STALL; // no more data
    }
    return (bWakeUp);
}



/*----------------------------------------------------------------------------+
| End of source file                                                          |
+----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/	
	//Released_Version_5_20_06_02
