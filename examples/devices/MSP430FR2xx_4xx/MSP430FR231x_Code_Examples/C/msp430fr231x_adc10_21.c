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
//  MSP430FR231x Demo - ADC, Window Comparator, 1.5V ref, Timer trigger, DCO
//
//  Description: This example works on Repeat-Single-Channel Mode.
//  A1 is sampled in every 0.5s using the TB1.1 trigger.
//  Window comparator is used to generate interrupts to indicate when the input
//  voltage goes above the High_Threshold or below the Low_Threshold or
//  is inbetween the high and low thresholds. Timer_B1 is used as an interval
//  timer used to control the LED at P1.0 to toggle slow/fast or turn off
//  according to the ADC Hi/Lo/IN interupts.
//  ACLK = XT1 = 32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
//
//               MSP430FR2311
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//       >---|P1.1/A1     P1.0 |--> LED 
//
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

#define    High_Threshold    682                        //~1V
#define    Low_Threshold     341                        //~0.5V

unsigned int SlowToggle_Period = 20000-1;
unsigned int FastToggle_Period = 1000-1;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                           // Stop WDT
  
    P1DIR |= BIT0;                                      // Set P1.0 output direction
    P1OUT &= ~BIT0;                                     // Clear P1.0
    
    // Configure ADC A1 pin
    P1SEL0 |= BIT1;
    P1SEL1 |= BIT1;
    
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
    
    // Configure XT1 oscillator
    P2SEL1 |= BIT6 | BIT7;                              // P2.6~P2.7: crystal pins
    CSCTL4 = SELA__XT1CLK;                              // Set ACLK = XT1; MCLK = SMCLK = DCO
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                  // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);                           // Test oscillator fault flag

    
    // Configure ADC; 
    ADCCTL0 = ADCSHT_2 | ADCMSC | ADCON;                // ADCON
    ADCCTL1 = ADCSHP | ADCSHS_2 | ADCCONSEQ_2;          // Rpt single ch; TB1.1 trigger
    ADCCTL2 = ADCRES;                                   // 10-bit conversion results
    ADCMCTL0 = ADCINCH_1 | ADCSREF_1;                   // Vref 1.5v, A1
    ADCHI = High_Threshold;                             // Window Comparator Hi-threshold 
    ADCLO = Low_Threshold;                              // Window Comparator Lo-threshold
    ADCIE |= ADCHIIE | ADCLOIE | ADCINIE;               // Enable ADC conv complete interrupt
  
    // Configure reference
    PMMCTL0_H = PMMPW_H;                                // Unlock the PMM registers
    PMMCTL2 |= INTREFEN;                                // Enable internal reference
    __delay_cycles(400);                                // Delay for reference settling

    // Configure TB0 period-timer
    TB0CCTL0 = CCIE;                                    // CCR0 interrupt enabled
    TB0CTL = TBSSEL_1 | TBCLR;                          // ACLK, clear TBR

    // Configure ADC timer trigger TB1.1
    TB1CCR0 = 16384-1;                                  // PWM Period
    TB1CCR1 = 8192-1;                                   // Duty cycle TB1.1
    TB1CCTL1 = OUTMOD_4;                                // TB1CCR1 toggle
    TB1CTL = TBSSEL_1 | MC_1 | TBCLR;                   // ACLK, up mode
    ADCCTL0 |= ADCENC;                                  // Enable conversion
    
    __bis_SR_register(LPM3_bits | GIE);                 // Enter LPM3 w/ interrupts
    __no_operation();                                   // Only for debugger
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE: 
            break;                      
        case ADCIV_ADCOVIFG: 
            break;                      
        case ADCIV_ADCTOVIFG: 
            break;                         
        case ADCIV_ADCHIIFG:                            // ADCHI; A1> 1V
            ADCIFG &= ~ADCHIIFG;                        // Clear interrupt flag
            TB0CTL &= ~MC_1;                            // Turn off Timer
            TB0CCR0 = FastToggle_Period;                // Set Timer Period for fast LED toggle
            TB0CTL |= MC_1;
            break;                          
        case ADCIV_ADCLOIFG:                            // ADCLO; A1 < 0.5V
            ADCIFG &= ~ADCLOIFG;                        // Clear interrupt flag
            TB0CTL &= ~MC_1;                            // Turn off Timer
             TB0CCR0 = SlowToggle_Period;               // Set Timer Period for slow LED toggle
            TB0CTL |= MC_1;
            break;                          
        case ADCIV_ADCINIFG:                            // ADCIN; 0.5V < A1 < 1V
            ADCIFG &= ~ADCINIFG;                        // Clear interrupt flag
            TB0CTL &= ~MC_1;                            // Turn off Timer
            P1OUT &= ~BIT0;
            break;                          
        case ADCIV_ADCIFG: 
            break;                                           
        default:
            break; 
    }   
}

// Timer0 B0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) TIMER0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= BIT0;                                        // Toggle LED P1.0
}
