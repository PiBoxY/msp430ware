/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//   MSP430FR60xx Demo - Output USSXT OSC at 8MHz on a pin.
//
//   Description: Configure USSXT = 8MHz and output on a pin. This can be used
//   for monitoring the clock or use the clock as the source of another subsystem.
//
//           MSP430FR6047
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|---> LED
//        |               |-USSXTIN
//        |               |-USSXTOUT
//        |           P8.7|---> USSXT_BOUT (8MHz)
//
//   Katie Pier
//   Texas Instruments Inc.
//   June 2017
//   Built with IAR Embedded Workbench V6.50 & Code Composer Studio V7.1
//******************************************************************************
#include <msp430.h>

#define OSCTYPE__CRYSTAL OSCTYPE_0

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure P8.7 as USSXT_BOUT
    P8DIR |= BIT7;
    P8SEL1 |= BIT7;
    P8SEL0 &= ~BIT7;

    // Configure P1.0 as output for LED
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Clock System Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3;                     // Set DCO to 8MHz
    // Set SMCLK = MCLK = DCO, ACLK = VLOCLK
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__8;   // MCLK = SMCLK = 1MHz
    CSCTL0_H = 0;                           // Lock CS registers

    //Configure USSXT Oscillator
    HSPLLUSSXTLCTL = OSCTYPE__CRYSTAL |  USSXTEN;

    // Set up timer to wait in LPM for crystal stabilization time = 4096 clocks for crystal resonator.
    // For 8MHz XTAL, 4096 clocks = 512us. Using VLO = 9.4kHz, wait 5 timer clock cycles = 532us.
    TA4CCR0 = 5;
    TA4CCTL0 = CCIE;                        // Enable Interrupt
    TA4CTL = TASSEL__ACLK | TACLR | MC__UP; // Timer sourced from ACLK (VLO), clear timer
    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3 w/interrupt
    __no_operation();                       // For debug

    // Check if oscillator is stable
    while((HSPLLUSSXTLCTL & OSCSTATE) == 0);

    // Output oscillator on pin
    HSPLLUSSXTLCTL &= ~XTOUTOFF;

    while(1)
    {
        P1OUT ^= BIT0;                      // Toggle LED to show oscillator init complete
        __delay_cycles(50000);              // Wait 100,000 CPU cycles
    }
}

// Timer A4 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER4_A0_VECTOR
__interrupt void Timer4_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER4_A0_VECTOR))) Timer4_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // Stop the timer and wake from LPM
    TA4CTL = MC__STOP;
    __bic_SR_register_on_exit(LPM3_bits | GIE);
    __no_operation();
}

