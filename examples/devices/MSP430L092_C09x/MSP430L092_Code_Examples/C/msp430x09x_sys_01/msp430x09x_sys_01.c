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
//********************************************************************************
//  Description: Toggle P1.0 using system NMI ISR. Software Toggles P1.0 in VMA
//  interrupt by writing to vacant memory address 0x2400. 
//  Proper use of the SYSSNIV interrupt vector generator is demonstrated
/****************************VMA**************************************************/
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*                               +----L092---+                                   */
/*                               |*1      14 |                                   */
/*                               | 2      13 |                                   */
/*                               | 3      12 |                                   */
/*                               | 4      11 |                                   */
/*                               | 5      10 |                                   */
/*     P1.0 -Toggle Output  <-   | 6       9 |                                   */
/*                               | 7       8 |                                   */
/*                               +-----------+                                   */
/*                                                                               */
/*  D.Dang/D.Archbold/D.Szmulewicz                                               */
/*  Texas Instruments Inc.                                                       */
/*  Built with IAR Version 5.10.4                                                */
/*********************************************************************************/

#include <msp430.h>  
int *vma_ptr;
int main(void)
{ 
  	WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT
    P1DIR = BIT0;                               // Set P1.0 as output
    vma_ptr = (int *)0x2400;                    // Initialize ponter to Vacant memory
    SFRIE1 = VMAIE;                             // Enable VMA interrupt
  
    while(1) 
  { 
  	    *vma_ptr = 0x56;                        // Write to Vacant memory
        __bis_SR_register(LPM0_bits);           // Enter LPM0
  }	    
}

// SYSSNIV Interrupt Vector handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SYSNMI_VECTOR  
__interrupt void SNI_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SYSNMI_VECTOR))) SNI_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(SYSSNIV, 10))         // Efficient switch-implementation
  { 
  	case  2: break;                             // Vector 2: SVMIFG
  	case  4: P1OUT ^= BIT0;                     // Vector 4: VMAIFG 
    __bic_SR_register_on_exit(LPM0_bits);       // Exit Active
  	         break;   
  	case  6: break;                             // Vector 6: JMBINIFG
  	case  8: break;                             // Vector 8: JMBOUTIFG
  	case 10: break;                             // Vector 10: Reserved
  }	
}         


