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
//   MSP430F534x Demo - Enters LPM3 with ACLK = LFXT1, REF0 disabled, 
//                      SVS disabled
//
//   Description: Configure ACLK = LFXT1 and enters LPM3. Measure current.
//   ACLK = LFXT1 = 32kHz, MCLK = SMCLK = default DCO
//
//                MSP430F534x
//             -----------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//            |                 |
//
//   B. Nisarga
//   Texas Instruments Inc.
//   Jun 2011
//   Built with CCSv4.2 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

//#define SYS14_WORKAROUND_REVE              // Uncomment this line if using RevE silicon 

#define INIT_MEMORY_ADDR1 0x0206
unsigned int *Init_Addr1 = ((unsigned int*)INIT_MEMORY_ADDR1); 

#ifdef SYS14_WORKAROUND_REVE
    #define INIT_MEMORY_ADDR2 0x0900
    unsigned int *Init_Addr2 = ((unsigned int*)INIT_MEMORY_ADDR2);
#endif
    
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer  
  
  // SYS14 Workaround
  *Init_Addr1 = 0x7F00;
  *(Init_Addr1+0x10) = 0x00E0;
  *(Init_Addr1+0x20) = 0xC140;
  *(Init_Addr1+0x30) = 0x07FF;
  
#ifdef SYS14_WORKAROUND_REVE  
  *Init_Addr2 = 0x9628;
  *(Init_Addr2+4) = 0x0000;
  *Init_Addr2 = 0x9600; 
#endif    

  // Enable XT1
  P5SEL |= BIT4+BIT5;                       // Port select XT1
  UCSCTL6 &= ~(XT1OFF);                     // XT1 On
  UCSCTL6 |= XCAP_3;                        // Internal load cap

  // Loop until XT1 & DCO stabilizes
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive
                                            // strength
  // Port Configuration
  P1OUT = 0x00;P2OUT = 0x00;P3OUT = 0x00;P4OUT = 0x00;P5OUT = 0x00;P6OUT = 0x00;
  PJOUT = 0x00;
  P1DIR = 0xFF;P2DIR = 0xFF;P3DIR = 0xFF;P4DIR = 0xFF;P5DIR = 0xFF;P6DIR = 0xFF;
  PJDIR = 0xFF;

  // Disable SVS
  PMMCTL0_H = PMMPW_H;                // PMM Password
  SVSMHCTL &= ~(SVMHE+SVSHE);         // Disable High side SVS 
  SVSMLCTL &= ~(SVMLE+SVSLE);         // Disable Low side SVS
  
  __bis_SR_register(LPM3_bits);       // Enter LPM3
  __no_operation();                   // For debugger
}

