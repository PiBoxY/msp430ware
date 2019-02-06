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
/*   Description: A-Pool ADC conversions with Overdrive Compensation. A single   */
/*   ramp conversion using the ADC-DAC-REG with the DAC as ramp generator.  The  */
/*   counter is cleared by software; channels are set and conversion is triggered*/
/*   by software.  The comparator stops the counter when the ramp voltage crosses*/
/*   the selected input voltage.  The Result is stored in APINT.                 */
/****************************A_POOL ADC Conversion *******************************/
/*                                                                               */
/*                                                                               */
/*              +----L092---+                                                    */
/*              |*1      14 |                                                    */
/*              | 2      13 |                                                    */
/*              | 3      12 |<- Channel A0-Provide Analog Input   (0-256mV)      */
/*              | 4      11 |                                                    */
/*              | 5      10 |                                                    */
/*              | 6       9 |                                                    */
/*              | 7       8 |                                                    */
/*              +-----------+                                                    */
/*  D.Dang/D.Archbold/D.Szmulewicz                                               */
/*  Texas Instruments Inc.                                                       */
/*  Built with IAR Version 5.10.4                                                */
/*********************************************************************************/

#include <msp430.h>

int Result;

int main(void)
{
   
     WDTCTL = WDTPW + WDTHOLD;                           // Stop WDT 
     Result = 0;                                         // Initialize Result variable  

// Begin Configuration of the A-POOL registers
     APCTL = 0;                                        // Clear APCTL register
     APIE  = EOCIE;                                    // Enable Interrupt for End of Conversion  
    
     APCNF = CMPON+DBON+CONVON+APREFON+CLKSEL_MCLK;    // Configure A-POOL elements, Select MCLK as A-POOL Clock Source
     APINT = 0x00;                                     // Clear ADC-DAC-REG
     APCTL = OSEL+CBSTP+SBSTP+RUNSTOP+APPSEL_5+APNSEL_0;        // Set Channels and Start Conversion
 __bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0 w/ interrupts enabled
    
     Result = APINT;
     APIE  = EOCIE;                                   // Enable Interrupt for End of Conversion  
     APCTL |= RUNSTOP + SLOPE;                        // Set Down and Start Conversion 
 __bis_SR_register(LPM0_bits + GIE);                  // Enter LPM0 w/ interrupts enabled
    
     Result += APINT;                                 // Add register value to Result variable
     Result >>= 1;                                    // Place Breakpoint Here: 
     
}

//A_POOL Interrupt Service Routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=APOOL_VECTOR
__interrupt void A_POOL(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(APOOL_VECTOR))) A_POOL (void)
#else
#error Compiler not supported!
#endif
{
    APIFG = 0;                                        // Clear Interrupt Flag      
__bic_SR_register_on_exit(LPM0_bits);                 // Exit Active to start Down conversion 

}

