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
//  MSP430FR413x Demo - Use RTC to measure frequency of the VLO by taking 20
//                          measurements and calculating the deltas.
//
//  Description: Sets RTCMOD to 9, to measure 10 VLOCLK cycles. It does this 20 times, then
//               calculates the deltas between the timer values to show the VLO frequency.
//               Uses SMCLK = 1MHz as the TA0 source.
//
//           MSP430FR4133
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST        P1.0|---> LED1
//        |               |
//
//
//  Kathryn Adamsky
//  Texas Instruments Inc.
//  June 2016
//  Built with Code Composer Studio v6.1.3
//******************************************************************************
#include <msp430.h>

#define NUMBER_TIMER_CAPTURES  20                   //Number of data points captured
#define VLO_CYCLES	10                              //RTCMOD value

volatile unsigned int timerAcaptureValues[NUMBER_TIMER_CAPTURES + 1];
volatile unsigned int timerAcaptureDeltas[NUMBER_TIMER_CAPTURES];
unsigned int timerAcapturePointer = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                       // Stop watchdog timer

    P1OUT &= ~BIT0;                                 // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                                  // Set P1.0 to output direction

    FRCTL0 = FRCTLPW | NWAITS0;                     // Set number of FRAM waitstates to 0

    PM5CTL0 &= ~LOCKLPM5;                           // Disable the GPIO power-on default high-impedance mode
                                                    // to activate previously configured port settings

    // Initialize RTC
    // RTC count re-load compare value at 10. You could change this value, just need to adjust calculation if you do
    RTCMOD = VLO_CYCLES-1;
    // Source = VLOCLK, /1 (no pre-divide because we want to measure VLO)
    RTCCTL = RTCSR | RTCPS__1; //do not start RTC yet


    // Configure clock
    __bis_SR_register(SCG0);                        // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                      // Set REFO as FLL reference source
    CSCTL0 = 0;                                     // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                         // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_2;                            // Set DCO = 4MHz
    CSCTL2 = FLLD_1 + 60;                           // DCODIV = 2MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                        // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));      // Poll until FLL is locked

    CSCTL4 |= SELMS__DCOCLKDIV | SELA__REFOCLK;     // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                                    // default DCODIV as MCLK and SMCLK source
    CSCTL5 |= DIVM__1 | DIVS__2;                    // SMCLK = 1MHz, MCLK = 2MHz

    // Timer0_A3 Setup
    //RTC is internally connected to TA0.1B - TA0CCTL1 select CCIS_1 = CCIxB
    TA0CCTL1 |= CM_1 | CCIS_1 | CCIE | CAP | SCS;
                                                    // Capture rising edge,
                                                    // Use CCI1B=RTC internal connection,
                                                    // Synchronous capture,
                                                    // Enable capture mode,
                                                    // Enable capture interrupt

    RTCCTL = RTCSS__VLOCLK | RTCSR | RTCPS__1;      //start RTC
    TA0CTL |= TASSEL_2 | MC_2 | TACLR;              // Use SMCLK as clock source, clear TA0R
                                                    // Start timer in continuous mode

    __enable_interrupt();
    while(1)
    {
    	__no_operation();
    }
}

// Timer0_A3 CC1-2, TA Interrupt Handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
	unsigned int i;
    switch(__even_in_range(TA0IV,TA0IV_TAIFG))
    {
        case TA0IV_NONE:
            break;                                  // No interrupt
        case TA0IV_TACCR1:
            timerAcaptureValues[timerAcapturePointer++] = TA0CCR1;
            if (timerAcapturePointer > NUMBER_TIMER_CAPTURES)
            {
            	//calculate deltas
            	for(i = 0; i < NUMBER_TIMER_CAPTURES; i++)
            	{
          			timerAcaptureDeltas[i] = timerAcaptureValues[i+1] - timerAcaptureValues[i];
            	}

            	//LED toggling shows measuring is done
                while (1)
                {
                    P1OUT ^= 0x01;                  // Toggle P1.0 (LED)
                    __delay_cycles(100000);
                }
            }
            break;
        case TA0IV_TACCR2:
            break;                                  // CCR2 not used
        case 6:
            break;                                  // reserved
        case 8:
            break;                                  // reserved
        case 10:
            break;                                  // reserved
        case 12:
            break;                                  // reserved
        case TA0IV_TAIFG:
            break;                                  // overflow
        default:
            break;
    }
}

// RTC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(RTCIV,RTCIV_RTCIF))
    {
        case  RTCIV_NONE:   break;                  // No interrupt
        case  RTCIV_RTCIF:                          // RTC Overflow
            P1OUT ^= BIT0;
            break;
        default: break;
    }
}
