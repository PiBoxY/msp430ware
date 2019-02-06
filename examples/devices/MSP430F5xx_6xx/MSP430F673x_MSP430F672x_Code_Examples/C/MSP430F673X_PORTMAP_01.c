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
//  MSP430F673x Demo - Port Mapping, Port1; Single runtime configuration
//
//  Description:  Port1 is port mapped to output TimerA digital signals.
//  TACCR1-2 generate different PWM DutyCycles and have been output on Port1
//  pins. TACCR0 generates a square wave of freq ACLK/2^10 ~32Hz.
//
//  MCLK = SMCLK = default DCO; ACLK = REFO~32kHz
//
//               MSP430F673x
//            -----------------
//        /|\|                 |
//         | |                 |
//         --|RST              |
//           |    P1.0(TA0CCR0)|--> ACLK/2^10 ~ 32Hz
//           |    P1.1(TA0CCR1)|--> 25%
//           |    P1.2(TA0CCR2)|--> 50%
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

// Refer D/S for Port2 Port Mapping definitions
const unsigned char P1mapping[3] = {  PM_TA0_0,
                                      PM_TA0_1,
                                      PM_TA0_2  };
// Function Definition
void Port_Mapping(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P1.x Port Mapping
    Port_Mapping();

    // Setup P1.x Pins
    P1DIR |= BIT0 | BIT1 | BIT2;            // P1.0 - P1.2 output
    P1SEL |= BIT0 | BIT1 | BIT2;            // P1.0 - P1.2 Port Map functions

    // Setup TA0
    TA0CCTL0 = OUTMOD_4;                    // CCR0 toggle/set
    TA0CCR0 = 256;                          // PWM Period/2
    TA0CCTL1 = OUTMOD_6;                    // CCR1 toggle/set
    TA0CCR1 = 192;                          // CCR1 PWM duty cycle
    TA0CCTL2 = OUTMOD_6;                    // CCR2 toggle/set
    TA0CCR2 = 128;                          // CCR2 PWM duty cycle
    TA0CTL = TASSEL_1 | MC_3;               // ACLK, up-down mode

    __bis_SR_register(LPM3_bits);           // Enter LPM3
    __no_operation();                       // For debugger
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

    ptr = &P1MAP0;
    for (i = 0; i < 3; i++)
    {
        *ptr = P1mapping[i];                // Enable TA0 on P1
        ptr++;
    }

    PMAPKEYID = 0;                          // Disable access Port Mapping regs
#ifdef PORT_MAP_EINT
    __enable_interrupt();                   // Re-enable all interrupts
#endif

}

