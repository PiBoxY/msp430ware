/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//***************************************************************************************
//  MSP430FR231x Demo - Timer0_B3 Capture of ACLK
//
//  Description; Capture a number of periods of the ACLK clock and store them in an array.
//  When the set number of periods is captured the program is trapped and the LED on
//  P1.0 is toggled. At this point halt the program execution read out the values using
//  the debugger.
//  ACLK = REFOCLK = 32kHz, MCLK = SMCLK = default DCODIV = 1MHz.
//
//                MSP430FR2311
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.1|-->ACLK
//            |             P1.0|-->LED
//
//
//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

#define NUMBER_TIMER_CAPTURES        20

volatile unsigned int timerBcaptureValues[NUMBER_TIMER_CAPTURES];
unsigned int timerBcapturePointer = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                       // Stop watchdog timer

    // Configure GPIO
    P1DIR |= BIT0;                                  // Set P1.0 as output
    P1OUT |= BIT0;                                  // P1.0 high

    P1SEL1 |= BIT1;                                 // Set as ACLK pin, second function
    P1DIR |= BIT1;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Timer0_B3 Setup
    TB0CCTL0 |= CM_1 | CCIS_1 | CCIE | CAP | SCS;
                                                    // Capture rising edge,
                                                    // Use CCI0B=ACLK,
                                                    // Synchronous capture,
                                                    // Enable capture mode,
                                                    // Enable capture interrupt

    TB0CTL |= TBSSEL_2 | MC_2 | TBCLR;              // Use SMCLK as clock source, clear TB0R
    // Start timer in continuous mode

    __bis_SR_register(LPM0_bits | GIE);
    __no_operation();
}

// Timer0_B3 CCR0, TB Interrupt Handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) TIMER0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    TB0CCTL0 &= ~CCIFG;
    timerBcaptureValues[timerBcapturePointer++] = TB0CCR0;
    if (timerBcapturePointer >= 20)
    {
        while (1)
        {
            P1OUT ^= 0x01;                  // Toggle P1.0 (LED)
            __delay_cycles(100000);
        }
    }


}
