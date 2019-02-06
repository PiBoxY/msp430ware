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
//  MSP430F15x Demo - USART0, SPI/UART 2400 alternating TX ->echo, 32kHz ACLK
//
//  Description: This example illustrates how the USART0 can be used for both
//  SPI and UART communication at the same time. To demonstrate this code, the
//  TX and RX lines need to be tied together for the  UART as well as the 
//  SIM0 and SOMI of the SPI. This code alternates between these two serial 
//  communication functionalities. Only one MSP430 is required for this demo.
//
//  In the Mainloop USART0 is configured for UART mode and a  
//  character is transmitted. Then USART0 is configured for SPI master 3-wire 
//  mode and a character is transmitted. Interrupts occur on RX .
//  UART mode: ACLK = UCLK0 = LFXT1 = 32768, 
//  SPI mode: SMCLK = DCO~ 800k
//  Baud rate divider with 32768hz XTAL @2400 = 32768Hz/2400 = 13.65 (000Dh)
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
// 
//
//                MSP430F155
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->|
//           |                 | 2400 - 8N1 |
//           |             P3.5|<-----------|
//           |                 |
//           |             P3.2|----------->|
//           |             SIMO|            |
//           |             P3.1|<-----------|
//           |             SOMI|
//
//  R. B. Elliott  / H. Grewal
//  Texas Instruments Inc.
//  Oct 2007
//  Built with IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include <msp430.h>
volatile unsigned int i=0;
void Config_UART (void);
void Config_SPI (void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
// Mainloop
  for (;;)
  {
    
  Config_UART();
  IE1 |= URXIE0;
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = 0x55;
  __bis_SR_register(LPM0_bits+ GIE);
  
  Config_SPI ();
  P1OUT ^=0x01;
  IE1 |= URXIE0;
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = 0x57;
  __bis_SR_register(LPM0_bits+ GIE);
 
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0RX_VECTOR
__interrupt void SPI0_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) SPI0_rx (void)
#else
#error Compiler not supported!
#endif
{

    while ((IFG1 & UTXIFG0) == 0);
    TXBUF0 = RXBUF0;
    while (!(U0TCTL & TXEPT));              // UART TX buffer empty?
    __bic_SR_register_on_exit(LPM0_bits + GIE);
}

void Config_UART (void)
{
  P3SEL = 0x30;                             // P3.4,5 = USART0 TXD/RXD
  U0CTL = SWRST + CHAR;                     // **Reset UART state machine**
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UTCTL0 = SSEL0;                           // UCLK = ACLK
  UBR00 = 0x0D;                             // 32k/2400 - 13.65
  UBR10 = 0x00;                             //
  UMCTL0 = 0x6B;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  i = 10000;                              // Delay
  do (i--);
  while (i != 0);
}

void Config_SPI (void)
{
  P3SEL &= ~ 0x10;                          // Clear UART 3.4 Port setting for I2C
  P1OUT = 0x000;                            // Setup P1.0 for LED output
  P1DIR |= 0x001;
  P3SEL = 0x00E;                            // Setup P3 for SPI mode
  U0CTL = CHAR + SYNC + MM + SWRST;         // 8-bit, SPI, Master
  U0TCTL = CKPL + SSEL1 + STC;              // Polarity, SMCLK, 3-wire
  U0BR0 = 0x002;                            // SPICLK = SMCLK/2
  U0BR1 = 0x000;
  U0MCTL = 0x000;
  ME1 = USPIE0;                             // Module enable
  U0CTL &= ~SWRST;                          // SPI enable
 
  i = 10000;                                // Delay
  do (i--);
  while (i != 0);
}
