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
//  MSP430G2230 Demo - SPI 3-wire Master Incremented Data
//
//  Description: SPI Master communicates full-duplex with SPI Slave using
//  3-wire mode. Incrementing data is sent by the master starting at 0x01. 
//  Received data is expected to be same as the previous transmission. If high, 
//  P1.2/LED indicates valid data reception.
//  ACLK = VLO, MCLK = SMCLK = Default DCO
//
//  Note: Ensure the slave device is running before the master device starts 
//    code execution 
//
//                  Slave                      Master
//          (msp430g2230_usi_03.c)
//                MSP430G2230               MSP430G2230
//             -----------------          -----------------
//            |              XIN|-    /|\|              XIN|-
//            |                 |      | |                 |
//            |             XOUT|-     --|RST          XOUT|-
//            |                 | /|\    |                 |
//            |          RST/NMI|--+     |                 |
//            |                 |        |                 |
//            |                 |        |             P1.2|->LED
//            |         SDI/P1.7|<-------|P1.6/SDO         |
//            |         SDO/P1.6|------->|P1.7/SDI         |
//            |        SCLK/P1.5|<-------|P1.5/SCLK        |
//
//  B. Nisarga
//  Texas Instruments, Inc
//  December 2011
//  Built with CCE Version: 5.1 and IAR Embedded Workbench Version: 5.4
//******************************************************************************

#include <msp430.h>

unsigned char MST_Data,SLV_Data;

int main(void)
{
  volatile unsigned int i;
 
  WDTCTL = WDTPW | WDTHOLD;             // Stop watchdog timer
  
  /* Initialization Code */
  P1REN = 0x1B;                         // Terminate unavailable Port1 pins (P1.0/1/3/4) properly
                                        // Config as Input with pull-down enabled
  BCSCTL3 |= LFXT1S_2;                  // Select VLO as low freq clock     
  /* End Initialization Code */ 
  
  P1DIR |= BIT2|BIT5|BIT6|BIT7;         // Config available P1.x pins as outputs
  
  USICTL0 |= USIPE7 | USIPE6 | USIPE5 | USIMST | USIOE; // Port, SPI master
  USICTL1 |= USIIE;                     // Counter interrupt, flag remains set
  USICKCTL = USIDIV_4 | USISSEL_2;      // /16 SMCLK
  USICTL0 &= ~USISWRST;                 // USI released for operation

  MST_Data = 0x01;                      // Initialize data values
  SLV_Data = 0x00;                      //
  
  USISRL = MST_Data;                    // init-load data
  USICNT = 8;                           // init-load counter
  __bis_SR_register(LPM0_bits + GIE);   // Enter LPM0 w/ interrupt
}

// USI interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USI_VECTOR
__interrupt void universal_serial_interface(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USI_VECTOR))) universal_serial_interface (void)
#else
#error Compiler not supported!
#endif
{
  if (SLV_Data & USISRL)
    P1OUT |= BIT2;
  else
    P1OUT &= ~BIT2;
  
  MST_Data++;
  SLV_Data++;
  
  USISRL = MST_Data;
  USICNT = 8;                           // re-load counter
  
  __delay_cycles(40);                   // Add time between transmissions to
                                        // make sure slave can process information  
}
