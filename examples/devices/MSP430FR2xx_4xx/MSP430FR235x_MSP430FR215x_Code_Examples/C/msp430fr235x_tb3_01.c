/* --COPYRIGHT--,BSD_EX
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
//*******************************************************************************
//  MSP430FR235x Demo - Timer3_B7, PWM TB3.1-4, Up Mode, 32KHz ACLK
//
//  Description: This program generates four PWM outputs on P6.0, P6.1, P6.2, P6.3
//  using Timer3_B configured for up mode. The value in CCR0, 100-1, defines the PWM
//  period and the values in CCR1, CCR2, CCR3, CCR4 define the PWM duty cycles. Using
//  ACLK as TBCLK, the timer period is ~3ms with a 80% duty cycle on P6.0, a 50%
//  duty cycle on P6.1, a 30% duty cycle on P6.2 and a 20% duty cycle on P6.3.
//  ACLK =TBCLK = XT1~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz
//
//
//           MSP430FR2355
//         ---------------
//     /|\|            XIN|--
//      | |               |  ~32768Hz
//      --|RST        XOUT|--
//        |               |
//        |     P6.0/TB3.1|--> CCR1 - 80% PWM
//        |     P6.1/TB3.2|--> CCR2 - 50% PWM
//        |     P6.2/TB3.3|--> CCR3 - 30% PWM
//        |     P6.3/TB3.4|--> CCR4 - 20% PWM
//
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2016
//   Built with IAR Embedded Workbench v6.50 & Code Composer Studio v6.2
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                         // Stop WDT

    // Configure GPIO
    P6DIR |= BIT0 | BIT1 | BIT2 | BIT3;               // P6.0 P6.1 P6.2 P6.3 output
    P6SEL0 |= BIT0 | BIT1 | BIT2 | BIT3;              // P6.0 P6.1 P6.2 P6.3 options select
    P2SEL1 |= BIT6 | BIT7;                            // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);                         // Test oscillator fault flag
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
    
    // Setup Timer3_B
    TB3CCR0 = 100-1;                                  // PWM Period
    TB3CCTL1 = OUTMOD_7;                              // CCR1 reset/set
    TB3CCR1 = 80;                                     // CCR1 PWM duty cycle
    TB3CCTL2 = OUTMOD_7;                              // CCR2 reset/set
    TB3CCR2 = 50;                                     // CCR2 PWM duty cycle
    TB3CCTL3 = OUTMOD_7;                              // CCR3 reset/set
    TB3CCR3 = 30;                                     // CCR3 PWM duty cycle
    TB3CCTL4 = OUTMOD_7;                              // CCR4 reset/set
    TB3CCR4 = 20;                                     // CCR4 PWM duty cycle
    TB3CTL = TBSSEL_1 | MC_1 | TBCLR;                 // ACLK, up mode, clear TBR

    __bis_SR_register(LPM3_bits);                     // Enter LPM3
    __no_operation();                                 // For debug
}
