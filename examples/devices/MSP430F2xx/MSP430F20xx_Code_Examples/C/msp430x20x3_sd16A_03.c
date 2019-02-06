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
//  MSP430F20x3 Demo - SD16A Sequence of conversions
//
//  The SD16A takes a sample of a single sequence of channels: A0, A1, then A2. 
//  Sampling begins with ch A0.  The 4th conversion result of each channel is 
//  stored in memory.
//
//
//                MSP430F20x3
//             ------------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//    Vin+ -->|A0+ P1.0         |
//            |A01- = VSS       |
//            |                 |
//    Vin+ -->|A1+ P1.2         |
//            |A1- = VSS        |
//            |                 |
//    Vin+ -->|A2+ P1.4         |
//            |A2- = VSS        |
//            |                 |
//            |            VREF |---+
//            |                 |   |
//            |                 |  -+- 100nF
//            |                 |  -+-
//            |                 |   |
//            |            AVss |---+
//            |                 |
//
//
//  R. B. Elliott  / H. Grewal
//  Texas Instruments
//  April 2007
//  Built with IAR Embedded Workbench Version 3.42A and CCE V3.0
//******************************************************************************
#include <msp430.h>

/* Arrays to store SD16 conversion results */
/* NOTE: arrays need to be global to       */
/*       prevent removal by compiler       */
static volatile unsigned int ChA0results = 0x00;
static volatile unsigned int ChA1results = 0x00;
static volatile unsigned int ChA2results = 0x00;
static unsigned int ch_counter=0;
  
int main(void)
{
  
  volatile unsigned int i;                  // Use volatile to prevent removal
                                            // by compiler optimization
  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  BCSCTL2 |= DIVS_3;                        // SMCLK/8
  
  SD16CTL = SD16REFON + SD16SSEL_1;         // 1.2V ref, SMCLK
  SD16INCTL0 = SD16INCH0;                   // Set channel A0+/-
  SD16CCTL0 |= SD16SNGL + SD16UNI + SD16IE; 
                                            // Single conv, 256OSR, unipolar, 
                                            // enable interrupt
  SD16INCTL0 |= SD16INTDLY_0;               // Interrupt on 4th sample 
  SD16AE = SD16AE0;                         // P1.0 A0+, A0- = VSS
  
  for (i = 0; i < 0x3600; i++);             // Delay for 1.2V ref startup
 
  while(1)
  {
    SD16CCTL0 |= SD16SC;                    // Set bit to start conversion
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SD16_VECTOR
__interrupt void SD16ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD16_VECTOR))) SD16ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (SD16IV)
  { case 2:                                 // SD16MEM Overflow
    break;
  
  case 4:                                   // SD16MEM0 IFG
        
    switch(ch_counter)
    {  
    case 0:
        ChA0results = SD16MEM0;             // Save CH0 results (clears IFG)
        SD16AE &= ~SD16AE0;                 // Disable external input A0+, A0
        SD16INCTL0 &= ~SD16INCH_0;          // Disable channel A0+/-
        ch_counter++;
        
        SD16INCTL0 |= SD16INCH_1;           // Enable channel A1+/-                          
        SD16AE |= SD16AE2;                  // Enable external input on A1+ 
        break;
        
    case 1:
        ChA1results = SD16MEM0;             // Save CH1 results (clears IFG)
        SD16AE &= ~SD16AE2;                 // Disable external input A1+, A1
        SD16INCTL0 &= ~SD16INCH_1;          // Disable channel A1+/-
        ch_counter++;

        SD16INCTL0 |= SD16INCH_2;           // Enable channel A2+/-                       
        SD16AE |= SD16AE4;                  // Enable external input on A2+
        break;
        
    case 2:
        ChA2results = SD16MEM0;             // Save CH2 results (clears IFG)
        ch_counter = 0;                     // Reset channel count (end of seq)
        SD16AE = SD16AE0;                   // Reset external input to A0+/-
        SD16INCTL0 = SD16INCH_0;            // Reset channel observed
        break;
    }
    
     __bic_SR_register_on_exit(LPM0_bits);  // Exit LPM0
    
  }
}
