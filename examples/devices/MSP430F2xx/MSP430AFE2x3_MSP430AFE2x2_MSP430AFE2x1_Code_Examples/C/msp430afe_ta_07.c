//******************************************************************************
//  MSP430AFE25x Demo - Timer_A, Toggle P1.0-2, Cont. Mode ISR, DCO SMCLK
//
//  Description: Use Timer_A CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, CCR0 and CCR1 output
//  units are optionally selected with port pins P1.2 and P1.1 in toggle
//  mode. As such, these pins will toggle when respective CCRx registers match
//  the TAR counter. Interrupts are also enabled with all CCRx units,
//  software loads offset to next interval only - as long as the interval offset
//  is aded to CCRx, toggle rate is generated in hardware. Timer_A overflow ISR
//  is used to toggle P1.0 with software. Proper use of the TAIV interrupt
//  vector generator is demonstrated.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO ~1MHz
//  As coded and assuming ~1MHz DCO, toggle rates are:
//  P1.2 = CCR0 ~ 1MHz/(2*200) ~2500Hz
//  P1.1 = CCR1 ~ 1MHz/(2*1000) ~500Hz
//  P1.0 = overflow ~ 1MHz/(2*65536) ~8Hz
//
//               MSP430AFE25x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.1/TA1|--> CCR1
//           |         P1.2/TA0|--> CCR0
//           |             P1.0|--> Overflow/software
//
//  K. Naveen
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include <msp430.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1SEL |= BIT2 + BIT1;                     // P1.1 - P1.2 option select
                                            // P1SEL2 = 0  
  P1DIR |= BIT2 + BIT1 + BIT0;              // P1.0 - P1.2 outputs
  CCTL0 = OUTMOD_4 + CCIE;                  // CCR0 toggle, interrupt enabled
  CCTL1 = OUTMOD_4 + CCIE;                  // CCR1 toggle, interrupt enabled
  TACTL = TASSEL_2 +  MC_2 + TAIE;          // SMCLK, Contmode, int enabled

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A0 (void)
#else
#error Compiler not supported!
#endif
{
  CCR0 += 200;                              // Add Offset to CCR0
}

// Timer_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA1_VECTOR))) Timer_A1 (void)
#else
#error Compiler not supported!
#endif
{
  switch( TAIV )
  {
  case  2: CCR1 += 1000;                    // Add Offset to CCR1
           break;
  case 10: P1OUT ^= BIT0;                   // Timer_A3 overflow
           break;
 }
}

