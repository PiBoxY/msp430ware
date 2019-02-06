/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/* 
 * ======== usbConstructs.h ========
 */

/*----------------------------------------------------------------------------
 * The following function names are deprecated.  These were updated to new 
 * names to follow OneMCU naming convention.
 +---------------------------------------------------------------------------*/
#ifndef DEPRECATED
#define hidSendDataWaitTilDone         USBHID_sendDataAndWaitTillDone
#define hidSendDataInBackground        USBHID_sendDataInBackground
#define hidReceiveDataInBuffer         USBHID_receiveDataInBuffer
#define cdcSendDataWaitTilDone         USBCDC_sendDataAndWaitTillDone
#define cdcSendDataInBackground        USBCDC_sendDataInBackground
#define cdcReceiveDataInBuffer         USBCDC_receiveDataInBuffer
#define phdcSendDataWaitTilDone        USBPHDC_sendDataAndWaitTillDone
#define phdcSendDataInBackground       USBPHDC_sendDataInBackground
#define phdcReceiveDataInBuffer        USBPHDC_receiveDataInBuffer 
#endif

//*****************************************************************************
//
//! \addtogroup usb_api
//! @{
//
//******************************************************************************


//*****************************************************************************
//
//! Completely Sends the Data in dataBuf
//! 
//! \param *dataBuf is the address of the data buffer.
//! \param size is the size of the data.
//! \param intfnum intfNum is which HID interface is being used.
//! \param ulTimeout is the (32-bit) number of polls to USBHID_getInterfaceStatus().
//!
//! Sends the data in \b dataBuf, of size \b size, using the post-call polling method.
//! It does so over interface \b intfNum. The function doesn???t return until the
//! send has completed. Because of this, the user buffer can be edited
//! immediately after the function returns, without consequence.  The
//! function assumes that size is non-zero.  It assumes no previous send
//! operation is underway. 
//! 
//! The 32-bit number \b ulTimeout selects how many times USBHID_getInterfaceStatus() will
//! be polled while waiting for the operation to complete. If the value is zero,
//! then no timeout is employed; it will wait indefinitely. When choosing a
//! number, it is advised to consider MCLK speed, as a faster CPU will cycle
//! through the calls more quickly.  The function provides the simplest coding,
//! at the expense of wasted cycles and potentially allowing MCU execution to
//! become "locked" to the host, a disadvantage if the host (or bus) is slow.
//! 
//! The function also checks all valid return codes, and returns non-zero if an
//! error occurred.  In many applications, the return value can simply be
//! evaluated as zero or non-zero, where nonzero means the call failed for
//! reasons of host or bus non-availability. Therefore, it may desirable for the
//! application to break from execution. Other applications may wish to handle
//! return values 1 and 2 in different ways.
//! 
//! It's recommended not to call this function from within an event handler.
//! This is because if an interface currently has an open send operation, the
//! operation will never complete during the event handler; rather, only after
//! the ISR that spawned the event returns. Thus the USBHID_getInterfaceStatus() polling
//! would loop indefinitely (or timeout). It???s better to set a flag from within
//! the event handler, and use this flag to trigger the calling of this function
//! from within main().
//! 
//! \return \b 0 if the call succeeded; all data has been sent.
//! \return \b 1 if the call timed out, either because the host is unavailable
//! 	or a COM port with an active application on the host wasn't opened.
//! \return \b 2 if the bus is unavailable.
//
//*****************************************************************************
uint8_t  USBHID_sendDataAndWaitTillDone (uint8_t* dataBuf,
    uint16_t size,
    uint8_t intfNum,
    uint32_t ulTimeout);


//*****************************************************************************
//
//! Completely Sends the Data in dataBuf
//! 
//! \param *dataBuf is the address of the data buffer.
//! \param size is the size of the data.
//! \param intfnum intfNum is which HID interface is being used.
//! \param ulTimeout is the (32-bit) number of polls to USBHID_getInterfaceStatus().
//!
//! Sends the data in \b dataBuf, of size \b size, using the pre-call polling
//! method. It does so over interface \b intfNum. The send operation may still
//! be active after the function returns, and \b dataBuf should not be edited
//! until it can be verified that the operation has completed. The function
//! assumes that size is non-zero.  This call assumes a previous send operation
//! might be underway.
//! 
//! The 32-bit number \b ulTimeout selects how many times USBHID_getInterfaceStatus()
//! will be polled while waiting for the previous operation to complete. If the
//! value is zero, then no timeout is employed; it will wait indefinitely. When
//! choosing a number, it is advised to consider MCLK speed, as a faster CPU
//! will cycle through the calls more quickly.  The function provides simple
//! coding while also taking advantage of the efficiencies of background
//! processing.  If a previous send operation is underway, this function does
//! waste cycles polling, like xxxsendDataWaitTilDone(); however it's less
//! likely to do so since much of the sending presumably took place in the 
//! background since the last call to xxxsendDataInBackground().
//! 
//! The function also checks all valid return codes, and returns non-zero if an
//! error occurred.  In many applications, the return value can simply be
//! evaluated as zero or non-zero, where nonzero means the call failed for
//! reasons of host or bus non-availability. Therefore, it may desirable for the
//! application to break from execution. Other applications may wish to handle
//! return values 1 and 2 in different ways.
//! 
//! It???s recommended not to call this function from within an event handler.
//! This is because if an interface currently has an open send operation, the
//! operation will never complete during the event handler; rather, only after
//! the ISR that spawned the event returns. Thus the USBHID_getInterfaceStatus() polling
//! would loop indefinitely (or timeout). It???s better to set a flag from within
//! the event handler, and use this flag to trigger the calling of this function
//! from within main().
//! 
//! \return \b 0 if the call succeeded; all data has been sent.
//! \return \b 1 if the call timed out, either because the host is unavailable
//! 	or a COM port with an active application on the host wasn't opened.
//! \return \b 2 if the bus is unavailable.
//
//*****************************************************************************
uint8_t USBHID_sendDataInBackground (uint8_t* dataBuf,
    uint16_t size,
    uint8_t intfNum,
    uint32_t ulTimeout);


//*****************************************************************************
//
//! Opens a Receive Operation
//! 
//! \param *dataBuf is the address of the data buffer.
//! \param size is the size of the data.
//! \param intfnum intfNum is which HID interface is being used.
//!
//! Opens a brief receive operation for any data that has already been received
//! into the USB buffer over interface \b intfNum. This call only retrieves data
//! that is already waiting in the USB buffer -- that is, data that has already
//! been received by the MCU.  It assumes a previous, open receive operation
//! (began by a direct call to USBxxx_receiveData()) is NOT underway on this
//! interface; and no receive operation remains open after this call returns.
//! It doesn't check for kUSBxxx_busNotAvailable, because it doesn't matter if
//! it's not.  The data in the USB buffer is copied into \b dataBuf, and the
//! function returns the number of bytes received.
//! 
//! \b size is the maximum that is allowed to be received before exiting; i.e.,
//! it is the size allotted to \b dataBuf.  If \b size bytes are received, the
//! function ends, returning \b size. In this case, it???s possible that more
//! bytes are still in the USB buffer; it might be a good idea to open another
//! receive operation to retrieve them. For this reason, operation is simplified
//! by using large \b size values, since it helps ensure all the data is
//! retrieved at one time.
//! 
//! This function is usually called when a USBHID_handleDataReceived() event
//! flags the application that data has been received into the USB buffer.
//! 
//! \return The number of bytes received into \b dataBuf.
//
//*****************************************************************************
uint16_t USBHID_receiveDataInBuffer(uint8_t*,uint16_t,uint8_t);



//*****************************************************************************
//
//! Completely Sends the Data in dataBuf
//! 
//! \param *dataBuf is the address of the data buffer.
//! \param size is the size of the data.
//! \param intfnum intfNum is which interface is being used.
//! \param ulTimeout is the (32-bit) number of polls to USBCDC_getInterfaceStatus().
//!
//! Sends the data in \b dataBuf, of size \b size, using the post-call polling method.
//! It does so over interface \b intfNum. The function doesn???t return until the
//! send has completed. Because of this, the user buffer can be edited
//! immediately after the function returns, without consequence.  The
//! function assumes that size is non-zero.  It assumes no previous send
//! operation is underway. 
//! 
//! The 32-bit number \b ulTimeout selects how many times USBCDC_getInterfaceStatus() will
//! be polled while waiting for the operation to complete. If the value is zero,
//! then no timeout is employed; it will wait indefinitely. When choosing a
//! number, it is advised to consider MCLK speed, as a faster CPU will cycle
//! through the calls more quickly.  The function provides the simplest coding,
//! at the expense of wasted cycles and potentially allowing MCU execution to
//! become "locked" to the host, a disadvantage if the host (or bus) is slow.
//! 
//! The function also checks all valid return codes, and returns non-zero if an
//! error occurred.  In many applications, the return value can simply be
//! evaluated as zero or non-zero, where nonzero means the call failed for
//! reasons of host or bus non-availability. Therefore, it may desirable for the
//! application to break from execution. Other applications may wish to handle
//! return values 1 and 2 in different ways.
//! 
//! It???s recommended not to call this function from within an event handler.
//! This is because if an interface currently has an open send operation, the
//! operation will never complete during the event handler; rather, only after
//! the ISR that spawned the event returns. Thus the USBCDC_getInterfaceStatus() polling
//! would loop indefinitely (or timeout). It???s better to set a flag from within
//! the event handler, and use this flag to trigger the calling of this function
//! from within main().
//! 
//! \return \b 0 if the call succeeded; all data has been sent.
//! \return \b 1 if the call timed out, either because the host is unavailable
//! 	or a COM port with an active application on the host wasn't opened.
//! \return \b 2 if the bus is unavailable.
//
//*****************************************************************************
uint8_t USBCDC_sendDataAndWaitTillDone (uint8_t* dataBuf,
    uint16_t size,
    uint8_t intfNum,
    uint32_t ulTimeout);

//*****************************************************************************
//
//! Completely Sends the Data in dataBuf
//! 
//! \param *dataBuf is the address of the data buffer.
//! \param size is the size of the data.
//! \param intfnum intfNum is which interface is being used.
//! \param ulTimeout is the (32-bit) number of polls to USBCDC_getInterfaceStatus().
//!
//! Sends the data in \b dataBuf, of size \b size, using the pre-call polling
//! method. It does so over interface \b intfNum. The send operation may still
//! be active after the function returns, and \b dataBuf should not be edited
//! until it can be verified that the operation has completed. The function
//! assumes that size is non-zero.  This call assumes a previous send operation
//! might be underway.
//! 
//! The 32-bit number \b ulTimeout selects how many times USBCDC_getInterfaceStatus()
//! will be polled while waiting for the previous operation to complete. If the
//! value is zero, then no timeout is employed; it will wait indefinitely. When
//! choosing a number, it is advised to consider MCLK speed, as a faster CPU
//! will cycle through the calls more quickly.  The function provides simple
//! coding while also taking advantage of the efficiencies of background
//! processing.  If a previous send operation is underway, this function does
//! waste cycles polling, like xxxsendDataWaitTilDone(); however it's less
//! likely to do so since much of the sending presumably took place in the 
//! background since the last call to xxxsendDataInBackground().
//! 
//! The function also checks all valid return codes, and returns non-zero if an
//! error occurred.  In many applications, the return value can simply be
//! evaluated as zero or non-zero, where nonzero means the call failed for
//! reasons of host or bus non-availability. Therefore, it may desirable for the
//! application to break from execution. Other applications may wish to handle
//! return values 1 and 2 in different ways.
//! 
//! It???s recommended not to call this function from within an event handler.
//! This is because if an interface currently has an open send operation, the
//! operation will never complete during the event handler; rather, only after
//! the ISR that spawned the event returns. Thus the USBCDC_getInterfaceStatus() polling
//! would loop indefinitely (or timeout). It???s better to set a flag from within
//! the event handler, and use this flag to trigger the calling of this function
//! from within main().
//! 
//! \return \b 0 if the call succeeded; all data has been sent.
//! \return \b 1 if the call timed out, either because the host is unavailable
//! 	or a COM port with an active application on the host wasn't opened.
//! \return \b 2 if the bus is unavailable.
//
//*****************************************************************************
uint8_t USBCDC_sendDataInBackground (uint8_t* dataBuf,
    uint16_t size,
    uint8_t intfNum,
    uint32_t ulTimeout);

//*****************************************************************************
//
//! Opens a Receive Operation
//! 
//! \param *dataBuf is the address of the data buffer.
//! \param size is the size of the data.
//! \param intfnum intfNum is which CDC interface is being used.
//!
//! Opens a brief receive operation for any data that has already been received
//! into the USB buffer over interface \b intfNum. This call only retrieves data
//! that is already waiting in the USB buffer -- that is, data that has already
//! been received by the MCU.  It assumes a previous, open receive operation
//! (began by a direct call to USBxxx_receiveData()) is NOT underway on this
//! interface; and no receive operation remains open after this call returns.
//! It doesn't check for kUSBxxx_busNotAvailable, because it doesn't matter if
//! it's not.  The data in the USB buffer is copied into \b dataBuf, and the
//! function returns the number of bytes received.
//! 
//! \b size is the maximum that is allowed to be received before exiting; i.e.,
//! it is the size allotted to \b dataBuf.  If \b size bytes are received, the
//! function ends, returning \b size. In this case, it???s possible that more
//! bytes are still in the USB buffer; it might be a good idea to open another
//! receive operation to retrieve them. For this reason, operation is simplified
//! by using large \b size values, since it helps ensure all the data is
//! retrieved at one time.
//! 
//! This function is usually called when a USBCDC_handleDataReceived() event
//! flags the application that data has been received into the USB buffer.
//! 
//! \return The number of bytes received into \b dataBuf.
//
//*****************************************************************************
uint16_t USBCDC_receiveDataInBuffer(uint8_t*,uint16_t,uint8_t);


//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************


uint8_t USBPHDC_sendDataAndWaitTillDone (uint8_t* dataBuf,
    uint16_t size,
    uint8_t intfNum,
    uint32_t ulTimeout);


uint8_t USBPHDC_sendDataInBackground (uint8_t* dataBuf,
    uint16_t size,
    uint8_t intfNum,
    uint32_t ulTimeout);


uint16_t USBPHDC_receiveDataInBuffer(uint8_t *,uint16_t,uint8_t);
//Released_Version_5_20_06_02
