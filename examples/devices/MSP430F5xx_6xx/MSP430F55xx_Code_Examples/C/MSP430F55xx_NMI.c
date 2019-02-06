/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
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
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430F552x Demo - NMI on a high-to-low transition on the RST/NMI pin from LPM4
//
//  Description: The code will configure the RST/NMI pin to NMI mode and
//  thereafter a high-to-low transition on RST/NMI will trigger the UNMI_ISR
//  routine which in turn will toggle port pin P1.0 (LED). Note that the
//  enabling of global interrupts is not needed since NMIs are not gated by
//  this. However for all practical purposes global interrupts (GIE) will
//  likely need to be enabled in any real-world application.
//
//  *** NOTE *** To observe this example's functionality it is necessary to
//  disconnect the JTAG debugger from the MSP430 to allow application usage
//  of the RST/NMI pin.
//
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//               MSP430F552x
//            -----------------
//           |              XIN|-
//           |                 |
//           |             XOUT|-
//     /|\   |                 |
//      --o--|RST/NMI      P1.0|-->LED
//     \|/
//
//   Andreas Dannenberg
//   Texas Instruments Inc.
//   May 2012
//   Built Code Composer Studio v5.2
//******************************************************************************

#include <msp430.h>

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;				// Stop watchdog timer

	P1OUT &= ~0x01;							// Clear P1.0 output latch
	P1DIR |= 0x01;                          // Set P1.0 to output direction

	SFRRPCR |= SYSNMIIES | SYSNMI;			// Select NMI function for the RST/NMI pin,
											// interrupt on falling edge
											// (pull-up R on RST/NMI is already enabled after PUC)

	SFRIE1 |= NMIIE;						// Set NMI pin interrupt enable

    __bis_SR_register(LPM4_bits);     		// Enter LPM4
}

// User NMI interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not supported!
#endif
{
	// Efficiently decode the User NMI interrupt source
	switch (__even_in_range(SYSUNIV, SYSUNIV_SYSBUSIV)) {
	   case SYSUNIV_NMIIFG :
		   P1OUT ^= 0x01;                   // Toggle P1.0 = LED
		   break;
	   case SYSUNIV_OFIFG :
	   case SYSUNIV_ACCVIFG :
	   case SYSUNIV_BUSIFG :
	   default :
		   break;
	}
}
