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
//  MSP430F673x Demo - Port Mapping, Port3; Single function to multiple pins;
//                     Multiple runtime configurations
//
//  Description:  The port mapping of TIMERA0 is changed with each WDT interrupt.
//                TimerA0 provides 2 PWMs with 2 different duty cycles.
//                TimerA0 is sourced from ACLK.
//
//  ACLK = REFO~32kHz; MCLK = SMCLK = default DCO
//
//               MSP430F673x
//            -----------------
//        /|\|                 |
//         | |                 |
//         --|RST              |
//           |  P3.0(TA0CCR1,2)|--> 25,50%
//           |  P3.1(TA0CCR1,2)|--> 25,50%
//           |  P3.2(TA0CCR1,2)|--> 25,50%
//           |  P3.3(TA0CCR1,2)|--> 25,50%
//           |  P3.4(TA0CCR1,2)|--> 25,50%
//           |  P3.5(TA0CCR1,2)|--> 25,50%
//           |  P3.6(TA0CCR1,2)|--> 25,50%
//           |  P3.7(TA0CCR1,2)|--> 25,50%
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

#define PORT_MAP_RECFG                      // Multiple runtime Port Map configs

unsigned char count = 0;
const unsigned char PortSequence[2] = {  PM_TA0_1,
                                         PM_TA0_2  };

// Function definition
void Port_Mapping(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P3.x Port Mapping
    Port_Mapping();

    // Setup P3.x Pins
    P3DIR |= 0xFF;                          // P3.0 - P3.7 output
    P3SEL |= 0xFF;                          // P3.0 - P3.7 Port Map functions

    // Setup TA 0
    TA0CCR0 = 256;                          // PWM Period/2
    TA0CCTL1 = OUTMOD_6;                    // CCR1 toggle/set
    TA0CCR1 = 192;                          // CCR1 PWM duty cycle
    TA0CCTL2 = OUTMOD_6;                    // CCR2 toggle/set
    TA0CCR2 = 128;                          // CCR2 PWM duty cycle
    TA0CTL = TASSEL_1 | MC_3;               // ACLK, up-down mode

    // Setup WDT in interval mode
    WDTCTL = WDT_ADLY_1000;                 // WDT 1s, ACLK, interval timer
    SFRIE1 |= WDTIE;                        // Enable WDT interrupt

    while (1)
    {
        __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/ interrupt
        __no_operation();                   // For debugger

        Port_Mapping();
        count++;
        if (count == 2)
            count = 0;
    }
}

// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3 on return
}

void Port_Mapping(void)
{
    unsigned char i;
    volatile unsigned char *ptr;

    __disable_interrupt();                  // Disable interrupts; Port Map cnfg
    PMAPKEYID = PMAPKEY;                    // Enable access Port Mapping regs

#ifdef PORT_MAP_RECFG
    PMAPCTL = PMAPRECFG;                    // Allow reconfig during runtime
#endif

    ptr = &P3MAP0;
    for (i = 0; i < 8; i++)
    {
        *ptr = PortSequence[count];         // Enable TA0.1, TA0.2 on P3
        ptr++;
    }

    PMAPKEYID = 0;                          // Disable access Port Mapping regs
#ifdef PORT_MAP_EINT
    __enable_interrupt();                   // Re-enable all interrupts
#endif

}

