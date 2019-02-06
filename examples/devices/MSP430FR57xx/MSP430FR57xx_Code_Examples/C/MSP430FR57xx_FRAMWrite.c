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
//   MSP430FR57xx Demo - Long word writes to FRAM
//
//   Description: Use long word write to write to 512 byte blocks of FRAM.
//   Toggle LED after every 100 writes.
//   NOTE: Running this example for extended periods will impact the FRAM
//   endurance. MPU is also enabled by default in CCS.
//   ACLK = VLO, MCLK = SMCLK = 4MHz
// 
//           MSP430FR57x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |               |  
//        |          P1.0 |---> LED
//
//   Kathryn Adamsky
//   Texas Instruments Inc.
//   July 2016 (Updated) | August 2010 (Created)
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************

#include <msp430.h>

void FRAMWrite(void);

#define WRITE_SIZE 128
unsigned char count = 0;
unsigned long data;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(FRAM_write)
unsigned long FRAM_write[WRITE_SIZE] = {0};
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent unsigned long FRAM_write[WRITE_SIZE] = {0};
#elif defined(__GNUC__)
unsigned long __attribute__((persistent)) FRAM_write[WRITE_SIZE] = {0};
#else
#error Compiler not supported!
#endif

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT  
  // Configure MCLK for 8MHz operation 
  CSCTL0_H = 0xA5;
  CSCTL1 |= DCOFSEL0 + DCOFSEL1;            // Set max. DCO setting
  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // ACLK = VLO
  CSCTL3 = DIVA_0 + DIVS_1 + DIVM_1;        // MCLK = SMCLK = DCO/2
  
  // Turn off temp sensor
  REFCTL0 |= REFTCOFF; 
  REFCTL0 &= ~REFON;

  // Turn on LED 
  P1DIR |= BIT0;
  // Initialize dummy data
  data = 0x11111111;

  while(1)
  { 
  data += 0x00010001;
  FRAMWrite();                              // Endless loop 
  count++;
  if (count > 100)
    {
      P1OUT ^= 0x01;                        // Toggle LED to show 512K bytes 
      count = 0;                            // ..have been written
      data = 0x00010001;
    }

  }
  
}


void FRAMWrite (void)

{
 unsigned int i=0;

  for ( i= 0; i< WRITE_SIZE; i++)
  {
      FRAM_write[i] = data;
  }
  
}


