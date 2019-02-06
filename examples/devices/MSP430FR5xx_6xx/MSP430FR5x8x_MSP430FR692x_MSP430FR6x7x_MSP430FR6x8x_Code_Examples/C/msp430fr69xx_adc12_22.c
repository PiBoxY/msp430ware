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
//  MSP430FR69xx Demo - ADC12, Window Comparator, 2.5V ref, 2's Complement
//
//  Description; A1 is sampled in single ch/ single conversion mode.
//  Window comparator is used to generate interrupts to
//  indicate when the input voltage goes above the High_Threshold or below the
//  Low_Threshold or is in between the high and low thresholds. TimerB0 is used
//  as an interval timer used to control the LED at P1.0 to toggle slow/fast
//  or turn off according to the ADC12 Hi/Lo/IN interupts.
//  NOTE: This example shows how to write to the ADC threshold registers when
//  2's complement mode is used.
//
//               MSP430FR6989
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//       >---|P1.1/A1     P1.0 |--> LED
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

#define High_Threshold  0x4CC0      // ~2V expressed in 2's complement
#define Low_Threshold   0xE670      // ~1V expressed in 2's complement

// To calculate the thresholds for the window comparator in 2's C mode
// Use (Vin/Vref* 4096) - 2048
// Append zeros to get a 16 bit result
// If the result is a negative number, take the 2's C before appending zeros

volatile unsigned int SlowToggle_Period = 20000-1;
volatile unsigned int FastToggle_Period = 1000-1;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1OUT &= ~BIT0;                           // Clear P1.0
  P1DIR |= BIT0;                            // Set P1.0 output direction
  P1SEL1 |= BIT1;                           // Configure ADC P1.1/A1
  P1SEL0 |= BIT1;
  PJSEL0 |= BIT4 | BIT5;                    // For XT1

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Clock System Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_3;                       // Set DCO to 8MHz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__8 | DIVM__8;     // MCLK = SMCLK = 1MHz
  CSCTL4 &= ~LFXTOFF;                       // Enable LFXT1
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers

  // Configure ADC12
  // tsample = 16ADC12CLK cycles, tconvert = 14 ADC12CLK cycles
  // software trigger for SOC, MODOSC, single ch-single conversion,
  // 2's complement mode
  // tsample controlled by SHT0x settings
  // Channel 1, reference = internal, enable window comparator
  // Set thresholds for ADC12 interrupts
  // Enable Interrupts
  ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
  ADC12CTL1 = ADC12SHS_0 | ADC12SSEL_0 | ADC12CONSEQ_0 | ADC12SHP;
  ADC12CTL2 |= ADC12DF;
  ADC12MCTL0 = ADC12INCH_1 | ADC12VRSEL_1 | ADC12WINC;
  ADC12HI = High_Threshold;
  ADC12LO = Low_Threshold;
  ADC12IER2 = ADC12HIIE | ADC12LOIE | ADC12INIE;

  // Configure internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  REFCTL0 |= REFVSEL_2 | REFON;             // Select internal ref = 2.5V
                                            // Internal Reference ON
  while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator
                                            // to settle
  // Configure TB0 period-timer for LED toggle
  TB0CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TB0CTL = TBSSEL__ACLK | TBCLR;            // ACLK, clear TBR

  while(1)
  {
    ADC12CTL0 |= ADC12ENC | ADC12SC;        // Enable & start conversion
    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts
    __no_operation();                       // for debugger
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
    case ADC12IV_NONE:        break;        // Vector  0:  No interrupt
    case ADC12IV_ADC12OVIFG:  break;        // Vector  2:  ADC12MEMx Overflow
    case ADC12IV_ADC12TOVIFG: break;        // Vector  4:  Conversion time overflow
    case ADC12IV_ADC12HIIFG:                // Is A1 > 2V?: High Interrupt
      ADC12IFGR2 &= ~ADC12HIIFG;            // Clear interrupt flag
      TB0CTL &= ~MC__UP;                    // Turn off Timer
      TB0CCR0 = FastToggle_Period;          // Set Timer Period for fast LED toggle
      TB0CTL |= MC__UP;
      __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
      break;
    case ADC12IV_ADC12LOIFG:                // Is A1 < 1V?: Low Interrupt
      ADC12IFGR2 &= ~ADC12LOIFG;            // Clear interrupt flag
      TB0CTL &= ~MC__UP;                    // Turn off Timer
      P1OUT &= ~BIT0;                       // Turn off LED
      __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
      break;
    case ADC12IV_ADC12INIFG:
      ADC12IFGR2 &= ~ADC12INIFG;            // Clear interrupt flag
      TB0CTL &= ~MC__UP;                    // Turn off Timer
      TB0CCR0 = SlowToggle_Period;          // Set Timer Period for slow LED toggle
      TB0CTL |= MC__UP;
      __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
      break;                                // Vector 10:  ADC12IN
    case ADC12IV_ADC12IFG0:   break;        // Vector 12:  ADC12MEM0
    case ADC12IV_ADC12IFG1:   break;        // Vector 14:  ADC12MEM1
    case ADC12IV_ADC12IFG2:   break;        // Vector 16:  ADC12MEM2
    case ADC12IV_ADC12IFG3:   break;        // Vector 18:  ADC12MEM3
    case ADC12IV_ADC12IFG4:   break;        // Vector 20:  ADC12MEM4
    case ADC12IV_ADC12IFG5:   break;        // Vector 22:  ADC12MEM5
    case ADC12IV_ADC12IFG6:   break;        // Vector 24:  ADC12MEM6
    case ADC12IV_ADC12IFG7:   break;        // Vector 26:  ADC12MEM7
    case ADC12IV_ADC12IFG8:   break;        // Vector 28:  ADC12MEM8
    case ADC12IV_ADC12IFG9:   break;        // Vector 30:  ADC12MEM9
    case ADC12IV_ADC12IFG10:  break;        // Vector 32:  ADC12MEM10
    case ADC12IV_ADC12IFG11:  break;        // Vector 34:  ADC12MEM11
    case ADC12IV_ADC12IFG12:  break;        // Vector 36:  ADC12MEM12
    case ADC12IV_ADC12IFG13:  break;        // Vector 38:  ADC12MEM13
    case ADC12IV_ADC12IFG14:  break;        // Vector 40:  ADC12MEM14
    case ADC12IV_ADC12IFG15:  break;        // Vector 42:  ADC12MEM15
    case ADC12IV_ADC12IFG16:  break;        // Vector 44:  ADC12MEM16
    case ADC12IV_ADC12IFG17:  break;        // Vector 46:  ADC12MEM17
    case ADC12IV_ADC12IFG18:  break;        // Vector 48:  ADC12MEM18
    case ADC12IV_ADC12IFG19:  break;        // Vector 50:  ADC12MEM19
    case ADC12IV_ADC12IFG20:  break;        // Vector 52:  ADC12MEM20
    case ADC12IV_ADC12IFG21:  break;        // Vector 54:  ADC12MEM21
    case ADC12IV_ADC12IFG22:  break;        // Vector 56:  ADC12MEM22
    case ADC12IV_ADC12IFG23:  break;        // Vector 58:  ADC12MEM23
    case ADC12IV_ADC12IFG24:  break;        // Vector 60:  ADC12MEM24
    case ADC12IV_ADC12IFG25:  break;        // Vector 62:  ADC12MEM25
    case ADC12IV_ADC12IFG26:  break;        // Vector 64:  ADC12MEM26
    case ADC12IV_ADC12IFG27:  break;        // Vector 66:  ADC12MEM27
    case ADC12IV_ADC12IFG28:  break;        // Vector 68:  ADC12MEM28
    case ADC12IV_ADC12IFG29:  break;        // Vector 70:  ADC12MEM29
    case ADC12IV_ADC12IFG30:  break;        // Vector 72:  ADC12MEM30
    case ADC12IV_ADC12IFG31:  break;        // Vector 74:  ADC12MEM31
    case ADC12IV_ADC12RDYIFG: break;        // Vector 76:  ADC12RDY
    default: break;
  }
}

// Timer1 A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) TIMER0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= BIT0;                            // Toggle LED P1.0
}

