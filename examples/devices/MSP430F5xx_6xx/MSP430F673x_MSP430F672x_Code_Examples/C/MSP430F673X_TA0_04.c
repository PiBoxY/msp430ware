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
//  MSP430F673x Demo - Timer0_A0, Toggle P1.0, Overflow ISR, 32kHz ACLK
//
//  Description: Toggle P1.0 using software and the Timer0_A overflow ISR.
//  In this example an ISR triggers when TA overflows. Inside the ISR P1.0
//  is toggled. Toggle rate is exactly 0.5Hz. Proper use of the TAIV interrupt
//  vector generator is demonstrated.
//  ACLK = XT1 = 32768Hz, MCLK = SMCLK = default DCO ~1.045MHz
//
//              MSP430F673x
//            ---------------
//        /|\|               |
//         | |               |
//         --|RST            |
//           |               |
//           |           P1.0|-->LED
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                // Stop WDT

    // Setup P1.0 output
    P1DIR |= BIT0;                           // Set P1.0 to output direction
    P1OUT &= ~BIT0;                          // Clear P1.0

    // Setup LFXT1
    UCSCTL6 &= ~(XT1OFF);                    // XT1 On
    UCSCTL6 |= XCAP_3;                       // Internal load cap
    // Loop until XT1 fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                   // Clear fault flags
    } while (SFRIFG1 & OFIFG);               // Test oscillator fault flag

    // Setup TA0
    TA0CTL = TASSEL_1 | MC_2 | TACLR | TAIE; // ACLK, cont. mode, clear TAR
                                             // Enable interrupt

    __bis_SR_register(LPM3_bits | GIE);      // Enter LPM3, enable interrupts
    __no_operation();                        // For debugger
}

// Timer0_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(TA0IV, 14))
    {
        case  TA0IV_NONE: break;             // No interrupt
        case  TA0IV_TA0CCR1: break;          // TA0CCR1_CCIFG
        case  TA0IV_TA0CCR2: break;          // TA0CCR2_CCIFG
        case  6: break;                      // Reserved
        case  8: break;                      // Reserved
        case 10: break;                      // Reserved
        case 12: break;                      // Reserved
        case TA0IV_TA0IFG:                   // TA0IFG
            P1OUT ^= BIT0;                   // Toggle P1.0
            break;
        default: break;
    }
}

