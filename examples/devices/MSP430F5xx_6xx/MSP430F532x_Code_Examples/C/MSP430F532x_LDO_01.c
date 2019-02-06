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
//   MSP430F532x Demo - Internal LDO powering device VCC and PortU
//
//   Description: This example code shows the device can be automatically 
//   powered up by the internal LDO when sufficicnt is present at the LDOI pin 
//   and how the external connection between LDOO and VCC can be used to power 
//   the device VCC.
//    
//   Note: 
//    1.To power the MSP430 device VCC via the internal LDO, connect  LDOO to  
//      VCC externally. If using TI target board (MSP-TS430RGC64B), jumper pin 
//      header JP4 
//    2.Ensure current drawn by device VCC is < ILDOO spec
//
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430F532x
//             -----------------
//        /|\|              VCC|<-
//         | |                 |  |(jumper JP4 on MSP-TS430RGC64B)
//         --|RST          LDOO|--
//           |                 |
//           |             LDOI|<-- LDO Input Voltage (3.76V to 5.5V - refer to the d/s specs for actual range)
//           |                 |                              
//           |             PU.0|--> Toggle
//           |             PU.1|--> Toggle (complementary to PU.0)
//
//   B. Nisarga
//   Texas Instruments Inc.
//   Oct 2011
//   Built with CCSv4 and IAR Embedded Workbench Version: 5.3
//******************************************************************************
#include <msp430.h>    
                            

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  
  // Configure PU.0 as output pins
  LDOKEYID = 0x9628;                        // Enable access to config registers 
  PUCTL |= PUOPE + PUOUT1;                  // PortU input enable
                                            // PU.0 = low and PU.1 = high  
  LDOPWRCTL |= LDOOVLIE;                    // Enable LDO overload indication interrupt
  LDOKEYID = 0x0;                           // Disbale access to config registers

  while(1)                                  // continuous loop
  {
    for(i=50000;i>0;i--);                   // Delay
    LDOKEYID = 0x9628;                      // Enable access to config registers
    PUCTL ^= (PUOUT0 + PUOUT1);             // XOR PU.0/1    
    LDOKEYID = 0x0;                         // Disbale access to config registers    
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=LDO_PWR_VECTOR
__interrupt void LDOInterruptHandler(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(LDO_PWR_VECTOR))) LDOInterruptHandler (void)
#else
#error Compiler not supported!
#endif
{
  if(LDOPWRCTL & VUOVLIFG)
  {
      LDOKEYID = 0x9628;                    // Enable access to config registers
      LDOPWRCTL &= ~VUOVLIFG;               // Software clear IFG
      LDOKEYID = 0x0;                       // Disbale access to config registers        
      while(1);                             // Over load indication; take necessary
                                            // steps in application firmware
  }
}
