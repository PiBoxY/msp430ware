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
//  MSP430xG461x Demo - Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK
//                      and send Time via UART
//
//  Description: This program toggles P5.1 by xor'ing P5.1 inside of
//  a Real Time Clock ISR. The Real Time Clock ISR is called once a minute using
//  the Alarm function provided by the RTC. ACLK used to clock basic timer.
//  The actual time is send send via UART
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//
//
//                MSP430FG4619
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P5.1|-->LED
//            |                 |
//            |      P2.4/UC0TXD|----------->
//            |                 | 2400 - 8N1
//            |      P2.5/UC0RXD|<-----------
//
//  S.Schauer / A. Dannenberg
//  Texas Instruments Inc.
//  June 2007
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include <msp430.h>
//------------------------------------------------------------------------------
void tx_char(char c);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop Watchdog Timer

  FLL_CTL0 = XCAP14PF;                  // Configure load caps

  P2SEL = 0x30;                         // P2.4,5 = USART0 TXD/RXD
  P5DIR |= 0x02;                        // Set P5.1 to output direction

  // SetupUSCI0:
  UCA0CTL1 = UCSWRST;                   // To set hold the module in reset
  UCA0CTL1 = UCSSEL0;                   // ACLK
  UCA0BR0 = 13;                         // 32k - 2400 baudrate control setting
  UCA0BR1 = 0;
  UCA0MCTL = UCBRS2+UCBRS1;             // Second modulation stage values
  UCA0STAT = UCFE+UCOE+UCPE+UCBRK+UCRXERR;

  // RTC
  RTCCTL = RTCBCD+RTCHOLD+RTCMODE_3+RTCTEV_0+RTCIE;
                                        // RTC enable, BCD mode,
                                        // alarm every Minute,
                                        // enable RTC interrupt
  // Init time
  RTCSEC =  0x00;                       // Set Seconds
  RTCMIN =  0x58;                       // Set Minutes
  RTCHOUR = 0x08;                       // Set Hours

  // Init date
  RTCDOW =  0x02;                       // Set DOW
  RTCDAY =  0x23;                       // Set Day
  RTCMON =  0x08;                       // Set Month
  RTCYEAR = 0x2005;                     // Set Year

  RTCCTL &= ~RTCHOLD;                   // Enable RTC

  while (1)
  {
    __bis_SR_register(LPM3_bits + GIE); // Enter LPM3 w/ interrupt
    __no_operation();                   // Required for debugger

    tx_char((RTCHOUR>>4)+0x30);         // Send high nibble of hour
    tx_char((RTCHOUR&0x0F)+0x30);       // Send low nibble of hour
    tx_char(':');                       // Send ':'
    tx_char((RTCMIN>>4)+0x30);          // Send high nibble of minute
    tx_char((RTCMIN&0x0F)+0x30);        // Send low nibble of minute
    tx_char('\n');                      // Send new line
  }
}

void tx_char(char c)
{
  while ((IFG2 & UCA0TXIFG)==0){}       // wait till TXbuf empty
  UCA0TXBUF = c;                        // TX char
}
// Basic Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=BASICTIMER_VECTOR
__interrupt void basic_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(BASICTIMER_VECTOR))) basic_timer (void)
#else
#error Compiler not supported!
#endif
{
  P5OUT ^= 0x02;                        // Toggle P5.1 using exclusive-OR
  LPM3_EXIT;                            // Exit LPM after interrupt
}
