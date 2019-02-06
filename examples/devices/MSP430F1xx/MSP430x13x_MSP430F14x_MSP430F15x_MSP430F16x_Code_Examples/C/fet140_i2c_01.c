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
//  MSP-FET430P140 Demo - I2C, Master Intf. to TMP100, Set P1.0 if Temp > 28C
//
//  Description: I2C interface to TMP100 temperature sensor in 9-bit mode.
//  Timer_A CCR0 interrupt is used as a wake up and to read the upper byte of
//  the TMP100 temperature register. If the temperature is greater than 28c,
//  P1.0 is set, else reset.  CPU is normally off and used only durring TA_ISR.
//  ACLK = n/a, MCLK = SMCLK = TACLK = I2CCLOCK = DCO~ 800k
//  //* MSP430F169 Device Required *//
//
//
//         /|\           /|\ /|\
//          |   TMP100   10k 10k     MSP430F169
//          |   -------   |   |  -----------------
//          +--|Vcc SDA|<-|---+>|P3.1         P1.0|--->
//          |  |       |  |     |                 |
//          +--|A1,A0  |  |     |                 |
//             |       |  |     |                 |
//          ---|Vss SCL|<-+-----|P3.3             |
//         \|/  -------         |                 |
//
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P1DIR |= 0x01;                            // Set P1.0 to output direction
  P3SEL |= 0x0a;                            // Assign I2C pins to module
  U0CTL |= I2C + SYNC;                      // Switch USART0 to I2C mode
  U0CTL &= ~I2CEN;                          // Recommended I2C init procedure
  I2CTCTL = I2CSSEL_2;                      // SMCLK
  I2CSCLH = 0x03;                           // High period of SCL
  I2CSCLL = 0x03;                           // Low period of SCL
  I2CNDAT = 0x01;                           // Transmit one byte
  I2CSA = 0x4e;                             // Slave address
  U0CTL |= I2CEN;                           // Enable I2C, 7 bit addr,
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
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
  if (I2CDRB > 0x1e) P1OUT |= 0x01;         // LED on
  else P1OUT &= ~0x01;                      // LED off
  U0CTL |= MST;                             // Master
  I2CTCTL |= I2CSTT + I2CSTP;               // Read, ST, SP (clears MST)
}
