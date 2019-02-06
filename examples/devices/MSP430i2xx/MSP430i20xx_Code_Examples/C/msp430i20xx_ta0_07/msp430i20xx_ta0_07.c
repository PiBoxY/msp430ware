/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//  MSP430i20xx Demo - Timer_A0, Toggle P1.4, CCR1 Cont. Mode ISR, DCO SMCLK/8
//
//  Description: Use Timer_A CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, CCR1 and CCR2 output
//  units are optionally selected with port pins P1.5 and P1.6 in toggle
//  mode. As such, these pins will toggle when respective CCRx registers match
//  the TAR counter. Interrupts are also enabled with all CCRx units,
//  software loads offset to next interval only - as long as the interval offset
//  is added to CCRx, toggle rate is generated in hardware. Timer_A overflow ISR
//  is used to toggle P1.4 with software.
//
//  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
//  * Ensure low_level_init.c is included when building/running this example *
//
//               MSP430i20xx
//             -----------------
//         /|\|                |
//          | |                |
//          --|RST   P1.6/TA0.2|--> CCR2
//            |      P1.5/TA0.1|--> CCR1
//            |            P1.4|--> LED (Overflow/Software)
//
//  T. Witt
//  Texas Instruments, Inc
//  September 2013
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include "msp430.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1SEL1 |= BIT5 | BIT6;              // P1.5, P1.6 CCRx Function
    P1DIR  |= BIT4 | BIT5 | BIT6;       // Set P1.4-6 as output

    TA0CCTL1 = OUTMOD_4 | CCIE;         // CCR1 Toggle, Interrupt Enabled
    TA0CCTL2 = OUTMOD_4 | CCIE;         // CCR2 Toggle, Interrupt Enabled
    TA0CTL = TASSEL_2 | MC_2 | ID_3 | TAIE; // SMCLK/8, Cont. Mode, Interrupt Enabled

    __bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts
    __no_operation();                   // For debugger
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TA0IV,TA0IV_TAIFG)) {
        case TA0IV_NONE: break;
        case TA0IV_TACCR1:
                         TA0CCR1 += 2000; // Add offset to CCR1
                         break;
        case TA0IV_TACCR2:
                   TA0CCR2 += 5000;       // Add offset to CCR2
                   break;
        case TA0IV_TAIFG:
                   P1OUT ^= BIT4;         // Toggle LED
                   break;
        default:   break;
    }
}
