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
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRRTCCTLIGHT OWNER OR
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
//  MSP430FR235x Demo - ICC, Nested interrupt with software configured priority
//
//  Description: Use Interrupt Compare Controller(ICC) to show nested interrupt
//               with software configured priority. There are 3 interrupts used
//               in this code. Set P1 interrupt highest priority. Set P2 interrupt
//               lowest priority. Set RTC interrupt medium priority. Use 3 LEDs
//               to indicate the execution of 3 interrupts. When RTC interrupt is
//               being served, P1 interrupt can preempt RTC interrupt but P2 interrupt
//               can only be served after RTC interrupt execution is finished.
//               Press buttons on P1.3 and P2.0 and then watch the LEDs status.
//
//               RTC Clock = XT1 = 32kHz, MCLK = SMCLK = default DCODIV = ~1MHz.
//
//           MSP430FR2355
//         ---------------
//     /|\|               |
//      | |      XIN(P2.7)|--
//      --|RST            |  ~32768Hz
//        |     XOUT(P2.6)|--
//        |               |
//        |          P1.0 |---> LED0 indicate RTC interrupt (Medium priority)
//        |          P1.1 |---> LED1 indicate Port1 interrupt (Highest priority)
//        |          P1.2 |---> LED2 indicate Port2 interrupt (Lowest priority)
//        |          P1.3 |<--- button1
//        |          P2.0 |<--- button2
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2016
//   Built with IAR Embedded Workbench v6.50 & Code Composer Studio v6.2
//******************************************************************************
#include <msp430.h>

void ICC_init();                           // Initialize and enable ICC

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P2SEL1 |= BIT6 | BIT7;                  // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag

    P1OUT &= ~(BIT0 | BIT1 | BIT2);         // Clear P1.0 P1.1 P1.2 output latch for a defined power-on state
    P1DIR |= BIT0 | BIT1 | BIT2;            // Set P1.0 P1.1 P1.2 to output direction

    P1OUT |= BIT3;                          // Configure P1.3 as pulled-up
    P1REN |= BIT3;                          // P1.3 pull-up register enable
    P1IES |= BIT3;                          // P1.3 Hi/Low edge
    P1IE |= BIT3;                           // P1.3 interrupt enabled

    P2OUT |= BIT0;                          // Configure P2.0 as pulled-up
    P2REN |= BIT0;                          // P2.0 pull-up register enable
    P2IES |= BIT0;                          // P2.0 Hi/Low edge
    P2IE |= BIT0;                           // P2.0 interrupt enabled


    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    P1IFG &= ~BIT3;                         // P1.3 IFG cleared
    P2IFG &= ~BIT0;                         // P2.0 IFG cleared

    // RTC count re-load compare value at 256.
    // 1024/32768 * 256 = 8 sec.
    RTCMOD = 256-1;
                                            // Initialize RTC
    // Source = 32kHz crystal, divided by 1024
    RTCCTL = RTCSS__XT1CLK | RTCSR | RTCPS__1024 | RTCIE;

    ICC_init();                             // Initialize and enable ICC

    __bis_SR_register(LPM3_bits);           // Enter LPM3
}

// Initialize and enable ICC
void ICC_init()
{
    __bic_SR_register(GIE);                  // Disable global interrupt
    // setting customized interrupt priorities
    ICCILSR0 = 0xFF3F;                       // Set P1 interrupt highest priority(ILSR3=00)
                                             // Set P2 interrupt lowest priority (ILSR2=11)
    ICCILSR1 = 0xF7FF;                       // Set RTC interrupt medium priority(ILSR13=01)
    ICCILSR2 = 0xFFFF;
    ICCILSR3 = 0xFFFF;
    // enable ICC module
    ICCSC |= ICCEN;
    __bis_SR_register(GIE);                  // Enable global interrupt
}

// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
    P1IFG &= ~BIT3;                         // Clear P1.3 IFG
    P1OUT |= BIT1;                          // Set P1.1 to indicate port interrupt
    __delay_cycles(500000);                 // Keep P1.1 output high for 0.5 second
    P1OUT &= ~BIT1;                         // Clear P1.1 output
}

// Port 2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
    P2IFG &= ~BIT0;                         // Clear P2.0 IFG
    P1OUT |= BIT2;                          // Set P1.2 to indicate port interrupt
    __delay_cycles(500000);                 // Keep P1.2 output high for 0.5 second
    P1OUT &= ~BIT2;                         // Clear P1.2 output
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
        case  RTCIV_NONE:   break;          // No interrupt
        case  RTCIV_RTCIF:                  // RTC Overflow
            __bis_SR_register(GIE);         // Enable global interrupt
            P1OUT |= BIT0;                  // Set P1.0 to indicate RTC interrupt
            __delay_cycles(4000000);        // Keep P1.0 output high for 4 second
            P1OUT &= ~BIT0;                 // Clear P1.0 output
            break;
        default: break;
    }
}
