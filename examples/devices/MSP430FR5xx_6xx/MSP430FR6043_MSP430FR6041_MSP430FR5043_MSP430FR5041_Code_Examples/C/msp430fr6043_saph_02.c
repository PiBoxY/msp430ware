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
//   MSP430FR60xx Demo - Pulse generation using the SAPH PPG and PHY.
//
//   Description: Configure the PPG and PHY in the SAPH module manually (register
//   mode instead of ASQ controlled) to generate two sets of high frequency pulses on the
//   dedicated CH0_OUT pin in Dual Tone mode, approximately once per second.
//
//           MSP430FR6043
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|---> LED
//        |               |-USSXTIN
//        |               |-USSXTOUT
//        |        CH0_OUT|---> 5 X-pulses @5MHz and 10 E-pulses @10MHz ~1/sec 
//
//   Gary Gao & Matt Calvo
//   Texas Instruments Inc.
//   Februray 2018
//   Built with IAR Embedded Workbench V7.10 & Code Composer Studio V7.3
//******************************************************************************
#include <msp430.h>

#define OSCTYPE__CRYSTAL OSCTYPE_0

void HSPLL_init(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure P1.0 as output for LED
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;

     // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Clock System Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    // Set SMCLK = MCLK = DCO, ACLK = VLOCLK
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__8;   // MCLK = SMCLK = 1MHz
    CSCTL0_H = 0;                           // Lock CS registers

    HSPLL_init();                           // Initialize the HSPLL and wait for it to lock

    // Set up the PPG settings
    SAPHKEY = KEY;                          // Unlock the SAPH registers
    SAPHPGC = PLEV_0 | PPOL_0 | 0x000A;     // 10 excitation pulses, 0 stop pulses, 
	                                        //output low when inactive, high polarity
    SAPHPGLPER = 4;                         // Low phase = 4 HSPLL cycles = 50ns
    SAPHPGHPER = 4;                         // High phase = 4 HSPLL cycles = 50ns

    SAPH_AXPGCTL=XMOD_2 | 0x0005;           // Dual Tone mode, 5 Extra excitation pulses
    SAPH_AXPGLPER = 8;                      // Low phase = 8 HSPLL cycles = 100ns
    SAPH_AXPGHPER = 8;                      // High phase = 8 HSPLL cycles = 100ns

    SAPHPGCTL = TRSEL_2 | PPGCHSEL_0 | PGSEL_0;// TA2.1 trigger, CH0 output, register mode

    // Set up the PHY to output PPG on dedicated CH0_OUT pin
    SAPHOSEL = PCH0SEL__PPGSE;              // Output PPG

    // Enable the PPG
    SAPHPGCTL |= PPGEN;

    // Configure TA2.1 for ~1/sec to trigger the pulse generation and toggle LED
    TA2CCR0 = 9400;
    TA2CCR1 = 4700;
    TA2CCTL1 = OUTMOD_7 | CCIE;             // Enable output signal to trigger PPG, enable Interrupt
    TA2CTL = TASSEL__ACLK | TACLR | MC__UP; // Timer sourced from ACLK (VLO), clear timer

    while(1)
    {
        __bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/interrupt
        __no_operation();                   // For debug
    }
}

// Timer A2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A1_VECTOR
__interrupt void Timer2_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A1_VECTOR))) Timer2_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TA2IV, TAIV__TAIFG))
    {
        case TAIV__NONE:   break;            // No interrupt
        case TAIV__TACCR1:
            P1OUT ^= BIT0;                   // Toggle LED to show new cycle
            break;
        case TAIV__TAIFG: break;             // overflow
        default: break;
    }
}

void HSPLL_init(void)
{
    // Configure USSXT Oscillator
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

    // Init PLL
    // Use the PLLM setting to get 80MHz output from our 8MHz input
    // Equation: PLL output clock frequency x 2 = input clock frequency x (PLLM+1)
    // Input clock frequency = 8MHz
    // Desired PLL output clock frequency = 80MHz
    // PLLM = 19
    HSPLLCTL = PLLM4 | PLLM1 | PLLM0 | PLLINFREQ; //PLLM = 19, PLL input frequency > 6MHz

    // Power up the UUPS to start the PLL
    UUPSCTL |= USSPWRUP;

    // Wait for UUPS to power up
    while((UUPSCTL & UPSTATE_3) != UPSTATE_3);

    // Wait for PLL to lock
    while(!(HSPLLCTL & PLL_LOCK));
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
