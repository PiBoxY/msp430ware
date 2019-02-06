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
//  MSP430x11x1 Demo - Timer_A, UART 9600 Echo, 32kHz ACLK
//
//  Description: This program demonstrates a half-duplex 9600-baud UART using
//  Timer_A3 and a 32kHz crystal. Normal operation in LMP0, DCO continously
//  stabilized, interrupt driven using CCR2. DCO used for Timer_A clock.
//  On valid RX character, the character is echoed back using 8N1 protocol.
//  ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = ~2MHz DCO
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//                MSP430F1121
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |   CCI0A/TXD/P1.1|-------->
//           |                 | 9600 8N1
//           |   CCI0B/RXD/P2.2|<--------
//
#define RXD       0x04                      // RXD on P2.2
#define TXD       0x02                      // TXD on P1.1

//   Conditions for 9600 Baud SW UART, DCO ~ 2MHz

#define Bitime_5  104                       // ~ 0.5 bit length
#define Bitime    208                       // ~ 9615 baud
#define DELTA     488                       // Target DCO = DELTA*(4096) ~2MHz

unsigned int RXTXData;
unsigned char BitCnt;

void TX_Byte (void);
void RX_Ready (void);

//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//*****************************************************************************

#include <msp430.h>


unsigned int Compare, Oldcapture;

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  BCSCTL1 |= DIVA_3;                        // ACLK = LFXT1CLK/8
  CCTL0 = OUT;                              // TXD Idle as Mark
  CCTL2 = CM_1 + CCIS_1 + CAP + CCIE;       // CAP, ACLK, interrupt
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, continuous mode
  P1SEL = TXD;                              // P1.1/TA0 for TXD function
  P1DIR = TXD;                              // TXD output on P1
  P2SEL = RXD;                              // P2.2/TA0 as RXD input

// Mainloop
  for (;;)
  {
  RX_Ready();                               // UART ready to RX one Byte
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interr until char RXed
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
  CCTL0 = CM1 + CCIS0 + OUTMOD0 + CAP + CCIE;   // Neg Edge, Cap
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
      if (CCTL0 & SCCI)                     // Get bit waiting in receive latch
      RXTXData |= 0x80;
      BitCnt --;                            // All bits RXed?
      if ( BitCnt == 0)
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
}



// Timer_A3 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA1_VECTOR))) Timer_A1 (void)
#else
#error Compiler not supported!
#endif
{
  switch( TAIV )
  {
    case  2: break;                         // CCR1 not used
    case  4:
    {
    Compare = CCR2;                         // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = CCR2;                      // Save current captured SMCLK
    if (DELTA < Compare)
      {
        DCOCTL--;
        if (DCOCTL == 0xFF)                 // DCO is too fast, slow it down
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3)))
          BCSCTL1--;                        // Did DCO role under?, Sel lower RSEL
        }
      }
    else
      {
        DCOCTL++;                           // DCO is too slow, speed it down
        if (DCOCTL == 0x00)
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3 + 0x07)))
          BCSCTL1++;                        // Did DCO role over? Sel higher RSEL
        }
      }
    }
  case 10: break;                           // not used
  }
}
