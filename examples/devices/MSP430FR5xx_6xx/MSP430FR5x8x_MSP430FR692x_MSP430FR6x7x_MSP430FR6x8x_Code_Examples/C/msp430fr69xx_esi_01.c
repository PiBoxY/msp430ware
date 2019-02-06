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
//  MSP430FR6989 Demo - ESI Module example
//
//  Description: This demo uses 2 PWM output to simulate a quadrature signal
//  that is fed directly into the eSIF module. The scan interface then uses
//  the COUNT_VALUE to determine how many revolutions before an interrupt
//  triggers.
//
//                     MSP430FR6989
//                  -----------------
//            RST -|             P1.0|--> LED
//                 |                 |
//                 |       TA0.1/P1.6|-------
//                 |       TA0.2/P1.7|---   |
//                -|LFIN             |  |   |
//     32KHz XTAL  |                 |  |   |
//                -|LFOUT  ESCI1/P9.4|<--   |
//                 |       ESCI0/P9.5|<------
//                 |    ESITEST4/P1.4|---> ESI_CLK
//                 |             P1.3|---> TOGGLES_AFTER_COUNT_VALUE_REVS
//                 |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

void ESI_init(void);

// User selectable values
#define INPUT_SIGNAL_FREQUENCY         20                    // In Hz
#define COUNT_VALUE                    10
//#define CLOCKWISE_ROTATION
#define COUNTER_CLOCKWISE_ROTATION

// Do not modify
#define TIMER_FREQUENCY    32768
#define TIMER_PERIOD       TIMER_FREQUENCY/INPUT_SIGNAL_FREQUENCY

#ifdef COUNTER_CLOCKWISE_ROTATION
#define FIRST_TRIGGER   65535
#endif
#ifdef CLOCKWISE_ROTATION
#define FIRST_TRIGGER   10
#endif

const unsigned char psmTable[] = {
        0x00,
        0x03,
        0x0C,
        0x49,
        0x00,
        0x01,
        0x48,
        0x09,
        0X00,
        0X41,
        0X08,
        0X09,
        0X40,
        0X01,
        0X08,
        0x09
 };


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure P1.0 as a generic purpose I/O to toggle the LED
    // Configure P1.6 and P1.7 to output PWM on TA0.1 and TA0.2
    // Configure P1.3 to output TSM clk via ESITEST4
    P1DIR = BIT0 | BIT3 | BIT6 | BIT7;
    P1SEL0 = BIT6 | BIT7;
    P1SEL1 = BIT3 | BIT6 | BIT7;

    // Configure P9.4 and P9.5 as input pin in ESICH0 and ESICH1 mode
    P9SEL0 |= BIT4 | BIT5;
    P9SEL1 |= BIT4 | BIT5;

    // Configure PJ.4 and PJ.5 for 32kHz crystal on LFXT
    PJSEL0 = BIT4 | BIT5;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure Clock System Setup
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    // Set SMCLK = MCLK = DCO, and ACLK = LFXT
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1
    CSCTL4 &= ~LFXTOFF;

    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear XT1 fault flag
      SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag

    CSCTL0_H = 0;                           // Lock CS registers

    // Configure Timer0 for PWM output
    TA0CTL = TASSEL__ACLK | TACLR | TAIE;   // ACLK, clear TAR, and enable timer overflow
    TA0CCR0 = TIMER_PERIOD;                 // PWM Period

    // Configure capture control register 1 to generate PWM1
#ifdef     COUNTER_CLOCKWISE_ROTATION
    TA0CCTL1 = OUTMOD_6 | CCIE;             // CCR1 toggle/set, enable capture control register
    TA0CCR1 = TIMER_PERIOD/4;               // Set PWM duty cycle of 25%
#endif
#ifdef     CLOCKWISE_ROTATION
    TA0CCTL1 = OUTMOD_6;                    // CCR1 toggle/set
    TA0CCR1 = TIMER_PERIOD/2;               // Set PWM duty cycle of 50%
#endif

    // Configure capture control register 2 to generate PWM2
#ifdef     COUNTER_CLOCKWISE_ROTATION
    TA0CCTL2 = OUTMOD_6 | CCIE;             // CCR2 toggle/set
    TA0CCR2 = TIMER_PERIOD/2;               // CCR2 PWM duty cycle
#endif
#ifdef     CLOCKWISE_ROTATION
    TA0CCTL2 = OUTMOD_6;                    // CCR2 toggle/set
    TA0CCR2 = TIMER_PERIOD/4;               // CCR2 PWM duty cycle
#endif

    TA0CTL |= MC__UP;                       // Configure timer for up mode

    ESI_init();

    ESIINT2 &= ~(ESIIFG3);                  // Clears the ESICNT1 interrupt flag
    ESIINT1 |= (ESIIE3);                    // Enable the ESICNT1 interrupt

    ESICTL |= ESIEN;

    // Enter low power mode 3 and wait for interrupt triggers
    __bis_SR_register(LPM3_bits | GIE);
    __no_operation();
}

void ESI_init(void)
{
    volatile unsigned int i;
    unsigned char *pt_psmTable;

    // Initialize ESI AFE with one of the comparator input from ESICIx channel
    ESIAFE = ESICA1X;

    // Initialize ESI TSM (Timing State Machine)
#if INPUT_SIGNAL_FREQUENCY==20
    // Configure TSM with trigger division of 390, and start trigger of ACLK
    ESITSM = ESIDIV3A2 | ESIDIV3A1 | ESIDIV3A0 | ESIDIV3B2 | ESIDIV3B1 | ESITSMTRG_1;
#elif INPUT_SIGNAL_FREQUENCY==50
    // Configure TSM with trigger division of 162, and start trigger of ACLK
    ESITSM = ESIDIV3A2 + ESIDIV3B2 | ESITSMTRG_1;
#elif INPUT_SIGNAL_FREQUENCY==400
    // Configure TSM with trigger division of 18, and start trigger of ACLK
    ESITSM = ESIDIV3A2 | ESITSMTRG_1;
#elif INPUT_SIGNAL_FREQUENCY==2000
    // Configure TSM with trigger division of 2, and start trigger of ACLK
    ESITSM = ESITSMTRG_1;
#endif

    // Initialize ESI PSM (Processing State Machine)
    // Configure ESICNT0 enable for up counter, reset the ESI counter 0
    // Configure ESICNT1 enable for up/down counter, reset the ESI counter 1
    // Configure ESICNT1 enable for down counter, reset the ESI counter 2
    // Configure TSM clock signal from Timing State Machine
    ESIPSM = ESICNT0EN | ESICNT0RST | ESICNT1EN | ESICNT1RST | ESICNT2EN | ESICNT2RST | ESIV2SEL | ESITEST4SEL_2;

    // Configure eSIF oscillator control register
    // Sets the internal oscillator frequency adjust with internal
    // oscillator calibration started.
    ESIOSC = ESICLKFQ5 + ESICLKGON + ESIHFSEL;

    // Select PPUS1 = ESIOUT0
    // Select PPUS2 = ESIOUT1
    ESICTL = ESIS1SEL_0 | ESIS2SEL_1;

    // Timing State Structure initialization
    // TSM State 0 (Idle) configuration
    // In idle mode ACLK is fed to ESI module. Everything is disabled


    // Timing State Structure initialization
    // TSM State 0 (Idle) configuration
    // In idle mode ACLK is fed to ESI module. Everything is disabled
    ESITSM0 = ESICLK;

    // TSM State 1 configuration
    // Turn on AFE component and wait for component to stabilize (3 usec)
    // Turn ON : COMPARATOR, DAC
    // Excitation and LC damping remain disable
    ESITSM1 = ESICA | ESICLKON | ESIDAC | ESIREPEAT_13;

    // TSM State 2 configuration
    // Turn ON : COMPARATOR, DAC
    ESITSM2 = ESICA | ESICLKON | ESIRSON | ESIDAC;

    // TSM State 3 configuration
    ESITSM3 = 0;

    // TSM State 4 configuration
    // Turn on AFE component and wait for component to stabilize (3 usec)
    // Turn ON : COMPARATOR, DAC
    // Excitation and LC damping remain disable
    ESITSM4 = ESICH_1 | ESICA | ESICLKON | ESIDAC | ESIREPEAT_13;

    // TSM State 5 configuration
    // Turn on AFE component and wait for component to stabilize (3 usec)
    // Turn ON : COMPARATOR, DAC
    // Excitation and LC damping remain disable
    ESITSM5 = ESICH_1 | ESICA | ESICLKON | ESIRSON | ESIDAC;

    // TSM State 6 configuration
    ESITSM6 = ESISTOP | ESICLK;

    // Comparator DAC1 Threshold Setting
    ESIDAC1R0 = 0x07FF;
    ESIDAC1R1 = 0x0010;
    ESIDAC1R2 = 0x07FF;
    ESIDAC1R3 = 0x0010;

    // Threshold for rotation counter
    ESITHR1 = FIRST_TRIGGER;

    // Copy the TSM_Table into ESI TSM registers
    pt_psmTable = (unsigned char *)&ESIRAM0;                // Destination pointer

    for (i = 0; i < (sizeof(psmTable) - 1); i++)
    {
        *pt_psmTable++ = psmTable[i];
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ESCAN_IF_VECTOR
__interrupt void ESCAN_IF_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ESCAN_IF_VECTOR))) ESCAN_IF_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(ESIIV, ESIIV_ESIIFG2))
    {
        case ESIIV_NONE:    break;          // No interrupt pending
        case ESIIV_ESIIFG1: break;          // Rising edge of the ESISTOP(tsm)
        case ESIIV_ESIIFG0: break;          // ESIOUT0 to ESIOUT3 conditions selected by ESIIFGSETx bits
        case ESIIV_ESIIFG8: break;          // ESIOUT4 to ESIOUT7 conditions selected by ESIIFGSET2x bits
        case ESIIV_ESIIFG3:
            P1OUT ^= BIT0;                  // Toggle P1.0 LED
#ifdef     COUNTER_CLOCKWISE_ROTATION
            ESITHR1-= COUNT_VALUE;
#endif
#ifdef     CLOCKWISE_ROTATION
            ESITHR1+= COUNT_VALUE;
#endif
            ESIINT2 &= ~(ESIIFG3);          // Clears the ESICNT1 interrupt flag
            break;
        case ESIIV_ESIIFG6: break;          // PSM transitions to a state with a Q7 bit
        case ESIIV_ESIIFG5: break;          // PSM transitions to a state with a Q6 bit
        case ESIIV_ESIIFG4: break;          // ESICNT2 counter conditions selected with the ESIIS2x bits
        case ESIIV_ESIIFG7: break;          // ESICNT0 counter conditions selected with the ESIIS0x bits
        case ESIIV_ESIIFG2: break;          // Start of a TSM sequence
        default:            break;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TA0IV, TA0IV_TA0IFG))
    {
        case TA0IV_NONE: break;             // No interrupt pending
        case TA0IV_TACCR1:                  // Timer_A capture control register 1
#ifdef     COUNTER_CLOCKWISE_ROTATION
            if(TA0CCR1 < TA0CCR2)
            {
                TA0CCR1 += TA0CCR2;
            }
            else
            {
                TA0CCR1 = TA0CCR0 >> 2;
            }
#endif
            break;
        case TA0IV_TACCR2:                      // Timer_A capture control register 2
#ifdef     CLOCKWISE_ROTATION
            if(TA0CCR2 < TA0CCR1)
            {
                TA0CCR2 += TA0CCR1;
            }
            else
            {
                TA0CCR2 = TA0CCR0 >> 2;
            }
#endif
            break;
        case TA0IV_TAIFG: break;            // Timer overflow flag
        default:           break;
    }
}
