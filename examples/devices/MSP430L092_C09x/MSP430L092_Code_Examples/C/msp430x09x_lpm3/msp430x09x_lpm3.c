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
//  MSP430L092 Demo - Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK
//
//  Description: This program operates MSP430 normally in LPM3, pulsing P1.0
//  WDT ISR used to wake-up system. P1.0 I/O configured
//  as low output to eliminate floating inputs. Current consumption does
//  increase when P1.0 is set. Demo for measuring LPM3 current.
//  ACLK = LFXT1/4 = 32768/4, MCLK = SMCLK = default DCO ~1.2MHz
//  //* External watch crystal on XIN XOUT is required for ACLK *//
//
//           MSP430F21x2
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->Output
//
//  D.Dang/ D.Archbold/ D.Szmulewicz                                           
//  Texas Instruments Inc.                                                 
// Built with IAR Version 5.10.4                                                    
//****************************************************************************
#include <msp430.h>

volatile unsigned int i;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;  
    P1DIR |= BIT0;
    CCSCTL0 = CCSKEY;                       // Unlock CCS
	CCSCTL4 |= SELA_1;                      // Select LFCLK/VLO as the ACLK source          
    CCSCTL5 &= ~DIVA_2;                     // Divide fACLK by 4
	CCSCTL0_H |= 0xFF;                      // Lock CCS
	/* Lock by writing to upper byte */
	
	
    WDTCTL = WDT_ADLY_16;                   // WDT SMCLK Delay Interval specify                  
    SFRIE1 |= WDTIE;                        // Enable WDT interrupt
  while(1)
  {
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3, enable interrupts
    P1OUT |= 0x01;                          // Set P1.0 
    for (i = 4; i > 0; i--);                // Delay
    P1OUT &= ~0x01;                         // Clear P1.0 
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bits from 0(SR)
}
