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
//  MSP430x21x1 Demo - Timer_A UART 115k, 12MHz DCO SMCLK
//
//  Description: This program demonstrates a full-duplex 115k-baud UART using
//  Timer_A3 and a the DCO. A character is echoed on the Hyperterminal of a
//  a PC. The DCO frequency settings are stored in INFOA flash segment.
//  ACLK = LFXT1 = 32768, MCLK = SMCLK = saved DCO 12MHz
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//               MSP430F21x1
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P1.3|--------> Power for MAX3221
//           |   CCI0A/TXD/P1.1|-------->
//           |                 | 115200 8N1
//           |   CCI0B/RXD/P2.2|<--------
//
//   H. Grewal / A. Dannenberg
//   Texas Instruments, Inc
//   July 2005
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//*****************************************************************************

#include <msp430.h>

#define RXD   0x04                          // RXD on P2.2
#define TXD   0x02                          // TXD on P1.1

//   Conditions for 115200 Baud SW UART, DCO = 12000000Hz
#define Bitime_5  52                       // ~ 0.5 bit length
#define Bitime    104                      // ~ 115384 baud

unsigned int RXTXData;
unsigned char BitCnt;

void TX_Byte(void);
void RX_Ready(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  if (CALBC1_12MHZ==0xFF)					// If calibration constants erased
  {											// do not load, trap CPU!!	
    while(1);                               
  }
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_12MHZ;                   // Set DCO
  DCOCTL = CALDCO_12MHZ;                    // Set DCO step + modulation

  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

  CCTL0 = OUT;                              // TXD Idle as Mark
  P1SEL = TXD;                              // P1.1/TA0 for TXD function
  P1DIR = TXD;                              // TXD output on P1
  P2SEL = RXD;                              // P2.2/TA0 as RXD input

  P1DIR |= 0x08;                            // Power MAX3221
  P1OUT |= 0x08;                            //

// Mainloop
  for (;;)
  {
  RX_Ready();                               // UART ready to RX one Byte
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interr until char RXed
  TX_Byte();                                // TX Back RXed Byte Received
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

// Function Readies UART to Receive Character into RXTXData Buffer
// Sync capture not possible as DCO=TACLK=SMCLK can be off !!
void RX_Ready (void)
{
  BitCnt = 0x8;                             // Load Bit counter
  CCTL0 = CM1 + CCIS0 + OUTMOD0 + CAP + CCIE; // Neg Edge, Cap
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

// RX
  if (CCTL0 & CCIS0)                        // RX on CCI0B?
  {
    if( CCTL0 & CAP )                       // Capture mode = start bit edge
    {
    CCTL0 &= ~ CAP;                         // Switch from capture to compare mode
    CCR0 += Bitime_5;
    __bic_SR_register_on_exit(SCG1 + SCG0); // DCO reamins on after reti
    }
    else
    {
    RXTXData = RXTXData >> 1;
    if (CCTL0 & SCCI)                       // Get bit waiting in receive latch
      RXTXData |= 0x80;
    BitCnt --;                              // All bits RXed?
    if (BitCnt == 0)
//>>>>>>>>>> Decode of Received Byte Here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    {
      CCTL0 &= ~ CCIE;                      // All bits RXed, disable interrupt
      __bic_SR_register_on_exit(LPM3_bits); // Clear LPM3 bits from 0(SR)
    }
//>>>>>>>>>> Decode of Received Byte Here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }
  }
// TX
  else
  {
    if ( BitCnt == 0)
      CCTL0 &= ~ CCIE;                      // All bits TXed, disable interrupt
    else
    {
      CCTL0 |=  OUTMOD2;                    // TX Space
      if (RXTXData & 0x01)
      CCTL0 &= ~ OUTMOD2;                   // TX Mark
      RXTXData = RXTXData >> 1;
      BitCnt --;
    }
  }
}
