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
//******************************************************************************
//  MSP430FR413x Demo - Output 32768Hz crystal on XT1 and observe failsafe
//
//  Description: Configure ACLK = XT1 crystal = 32768Hz,
//               MCLK = DCO + XT1CLK REF = 1MHz,
//               SMCLK = MCLK = 1MHz.
//               Toggle LED to indicate that the program is running.
//
//           MSP430FR4133
//         ---------------
//     /|\|               |
//      | |      XIN(P4.1)|--
//      --|RST            |  ~32768Hz
//        |     XOUT(P4.2)|--
//        |               |
//        |          P1.0 |---> LED
//        |          P1.4 |---> MCLK = 1MHz
//        |          P8.0 |---> SMCLK = 1MHz
//        |          P8.1 |---> ACLK = 32768Hz
//
//
//   William Goh
//   Texas Instruments Inc.
//   June 2013
//   Built with IAR Embedded Workbench v5.60 & Code Composer Studio v6.0.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P4SEL0 |= BIT1 | BIT2;                  // set XT1 pin as second function

    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag

    __bis_SR_register(SCG0);                // disable FLL
    CSCTL3 |= SELREF__XT1CLK;               // Set XT1CLK as FLL reference source
    CSCTL0 = 0;                             // clear DCO and MOD registers
    __bic_SR_register(SCG0);                // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
    CSCTL7 &= ~DCOFFG;                         // Clear DCO fault flag
    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;  // set ACLK = XT1CLK = 32768Hz
                                               // DCOCLK = MCLK and SMCLK source

    // Now that osc is running enable fault interrupt
    SFRIE1 |= OFIE;

    P1DIR |= BIT0 + BIT4;                   // set MCLK and LED pin as output
    P1SEL0 |= BIT4;                         // set MCLK pin as second function
    P8DIR |= BIT0 | BIT1;                   // set ACLK and SMCLK pin as output
    P8SEL0 |= BIT0 | BIT1;                  // set ACLK and SMCLK pin as second function

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    while(1)
    {
        P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
        __delay_cycles(500000);             // Delay for 500000*(1/MCLK)=0.5s
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt void UNMI_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not supported!
#endif
{
    do
    {
        // set a breakpoint on the line below to observe XT1 operating from VLO
        // when the breakpoint is hit during a crystal fault
        CSCTL7 &= ~XT1OFFG;                 // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
        P1OUT |= BIT0;
        __delay_cycles(25000);              // time for flag to get set again
    }while (SFRIFG1&OFIFG);                 // Test oscillator fault flag

    P1OUT &= ~BIT0;
}
