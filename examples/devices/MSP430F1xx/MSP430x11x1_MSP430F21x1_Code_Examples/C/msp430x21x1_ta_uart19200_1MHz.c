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
//  MSP430x21x1 Demo - Timer_A UART 19200 TX, 1MHz DCO
//
//  Description: This program demonstrates a half-duplex 19200-baud UART using
//  Timer_A3 and a the DCO.  The program will wait in LPM0 sending the
//  string 'Hello World' with 112ms interval between characters as timed by a
//  WDT interrupt. The Set_DCO subroutine will calibrate the DCOCLK
//  to ~1Mhz.
//  ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = target DCO
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//               MSP430F21x1
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P1.3|--------> Power for MAX3221
//           |   CCI0A/TXD/P1.1|--------> "Hello World"
//           |                 | 19200 8N1
//           |   CCI0B/RXD/P2.2|<--------
//
//   M.Buccini / A. Dannenberg
//   Texas Instruments, Inc
//   July 2005
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//*****************************************************************************

#include <msp430.h>

#define RXD   0x04                          // RXD on P2.2
#define TXD   0x02                          // TXD on P1.1

//   Conditions for 19200 Baud SW UART, DCO = 1048576MHz
#define Bitime_5  27                        // ~ 0.5 bit length
#define Bitime    55                        // ~ 19200 baud
#define DELTA     256                       // Target DCO = DELTA*(4096)

unsigned int RXTXData;
unsigned char BitCnt;
unsigned char i = 0;

void TX_Byte(void);
void Set_DCO(void);

static const char string1[] = { "Hello World\r\n" };

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  BCSCTL1 |= DIVA_3;                        // ACLK = LFXT1CLK/8
  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear
  Set_DCO();                                // Set DCO

  WDTCTL = WDT_MDLY_32;                     // WDT 16ms*8, ACLK, interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt

  CCTL0 = OUT;                              // TXD Idle as Mark
  P1SEL = TXD;                              // P1.1/TA0 for TXD function
  P1DIR = TXD;                              // TXD output on P1
  P2SEL = RXD;                              // P2.2/TA0 as RXD input

  P1DIR |= 0x08;                            // Power MAX3221
  P1OUT |= 0x08;                            //

// Mainloop
  for (;;)
  {
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM3
    RXTXData = string1[i];
    TX_Byte();                              // TX
    i++;
    if (i > sizeof string1-1) i = 0;
  }
}

// Function Transmits Character from RXTXData Buffer
void TX_Byte (void)
{
  BitCnt = 0xA;                             // Load Bit counter, 8data + ST/SP
  CCR0 = TAR;                               // Current state of TA counter
  CCR0 += Bitime;                           // Some time till first bit
  RXTXData |= 0x100;                        // Add mark stop bit to RXTXData
  RXTXData = RXTXData << 1;                 // Add space start bit
  CCTL0 = OUTMOD0 + CCIE;                   // TXD = mark = idle
  while ( CCTL0 & CCIE );                   // Wait for TX completion
}

// Set DCO to selected frequency
void Set_DCO(void)
{
  unsigned int Compare, Oldcapture = 0;

  BCSCTL1 |= DIVA_3;                        // ACLK= LFXT1CLK/8
  CCTL2 = CM_1 + CCIS_1 + CAP;              // CAP, ACLK

  while (1)
  {
    while (!(CCIFG & CCTL2));               // Wait until capture occured
    CCTL2 &= ~CCIFG;                        // Capture occured, clear flag
    Compare = CCR2;                         // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = CCR2;                      // Save current captured SMCLK

    if (DELTA == Compare) break;            // If equal, leave "while(1)"
    else if (DELTA < Compare)               // DCO is too fast, slow it down
    {
      DCOCTL--;
      if (DCOCTL == 0xFF)
      {
        if (!(BCSCTL1 == (XT2OFF + DIVA_3)))
        BCSCTL1--;                          // Did DCO roll under?, Sel lower RSEL
      }
    }
    else
    {
      DCOCTL++;
      if (DCOCTL == 0x00)
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3 + 0x0F)))
          BCSCTL1++;                        // Did DCO roll over? Sel higher RSEL
        }
    }
  }
  CCTL2 = 0;                                // Stop CCR2
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  CCR0 += Bitime;                           // Add Offset to CCR0

// TX
    if ( BitCnt == 0)
    CCTL0 &= ~ CCIE;                        // All bits TXed, disable interrupt
    else
    {
      CCTL0 |=  OUTMOD2;                    // TX Space
      if (RXTXData & 0x01)
      CCTL0 &= ~ OUTMOD2;                   // TX Mark
      RXTXData = RXTXData >> 1;
      BitCnt --;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(LPM0_bits);   // Clear LPM3 bits from 0(SR)
}
