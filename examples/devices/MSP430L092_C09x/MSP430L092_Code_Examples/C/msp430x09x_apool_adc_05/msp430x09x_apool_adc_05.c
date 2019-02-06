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
// Description: The following code example starts and stops the A-Pool with Timer_A0. 
// The starting and stopping ADC ramps can be observed on the AOUT pin. The ADC ramp 
// does not stop at a specific voltage level, because no SBSTP or CBSTP bits are set.
/****************************A_POOL ADC Conversion *******************************/
/*                                                                               */
/*                                                                               */
/*              +----L092---+                                                    */
/*              |*1      14 |                                                    */
/*              | 2      13 |                                                    */
/*              | 3      12 |<- Channel A0-Provide Analog Input                  */
/*              | 4      11 |                                                    */
/*              | 5      10 |                                                    */
/*              | 6       9 |                                                    */
/*              | 7       8 |-> AOUT                                             */
/*              +-----------+                                                    */
/*                                                                               */
/*  D.Dang/D.Archbold/D.Szmulewicz                                               */
/*  Texas Instruments Inc.                                                       */
/*  Built with IAR Version 5.10.4                                                */
/*********************************************************************************/

#include <msp430.h>

int Result;

int main(void)
{
   
   WDTCTL = WDTPW + WDTHOLD;                          // Stop WDT 
   TA0CCR0 = 600;                                     // Set start value to 600
   TA0CCR1 = 300;                                     // Set stop value to 300
   TA0CCTL0 = OUTMOD_3;                               // Set CCR0 outmode to Set/Reset
   TA0CCTL1 = OUTMOD_3;                               // Set CCR0 outmode to Set/Reset
   TA0CTL = TASSEL_2+MC_1+TACLR;                      // Set SMCLK to timer clock source +
                                                      // Set timer to up mode +
                                                      // Clear timer count register
// Begin Configuration of the A-POOL registers
   APCNF = CMPON+DBON+CONVON+APREFON+TA0EN;           // Enable comparator on +
                                                      // Enable DAC buffer +
                                                      // Enable conversion +
                                                      // Enable reference +
                                                      // Enable TimerA0 start
   APCTL = APPSEL_5+APPSEL_0+OSEL+ODEN+TBSTP;         // Set DAC buffer output to PSEL +
                                                      // Select output buffer +
                                                      // Enable output driver +
                                                      // Enable timer based stop    
      
while(1);
}
