 /* --COPYRIGHT--,BSD_EX
  * Copyright (c) 2015, Texas Instruments Incorporated
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
 //  MSP430FR60xx Demo - Software Port Interrupt Service on P1.1 from LPM4
 //
 //  Description: A Hi/Lo transition on P1.1 will trigger P1ISR the first time.
 //  On hitting the P1ISR, device exits LPM4 mode and executes section of code in
 //  main() which includes toggling an LED and the P1.1 IES bit which switches
 //  between Lo/Hi and Hi/Lo trigger transitions to alternatively enter the P1ISR.
 //  ACLK = n/a, MCLK = SMCLK = default DCO
 //
 //
 //               MSP430FR6047
 //            -----------------
 //        /|\|                 |
 //         | |                 |
 //         --|RST              |
 //     /|\   |                 |
 //      --o--|P1.1         P1.0|-->LED
 //     \|/
 //
 //   Evan Wakefield
 //   Texas Instruments Inc.
 //   October 2016
 //   Built with IAR Embedded Workbench V6.50 & Code Composer Studio V6.2
 //******************************************************************************

 #include <msp430.h>

 int main(void)
 {
   WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

     // Configure GPIO
     P1OUT = BIT1;                           // Pull-up resistor on P1.1
     P1REN = BIT1;                           // Select pull-up mode for P1.1
     P1DIR = 0xFF ^ BIT1;                    // Set all but P1.1 to output direction
     P1IES = BIT1;                           // P1.1 Hi/Lo edge
     P1IFG = 0;                              // Clear all P1 interrupt flags
     P1IE = BIT1;                            // P1.1 interrupt enabled

     P2OUT = 0;
     P2DIR = 0xFF;

     P3OUT = 0;
     P3DIR = 0xFF;

     P4OUT = 0;
     P4DIR = 0xFF;

     P5OUT = 0;
     P5DIR = 0xFF;

     P6OUT = 0;
     P6DIR = 0xFF;

     P7OUT = 0;
     P7DIR = 0xFF;

     P8OUT = 0;
     P8DIR = 0xFF;

     P9OUT = 0;
     P9DIR = 0xFF;

     PJOUT = 0;
     PJDIR = 0xFFFF;

     // Disable the GPIO power-on default high-impedance mode to activate
     // previously configured port settings
     PM5CTL0 &= ~LOCKLPM5;

     while(1)
     {
         __bis_SR_register(LPM4_bits | GIE); // Enter LPM4 w/interrupt
         __no_operation();                   // For debugger
         P1OUT ^= BIT0;                      // P1.0 = toggle
     }
 }

 // Port 1 interrupt service routine
 #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
 #pragma vector=PORT1_VECTOR
 __interrupt void port1_isr_handler(void)
 #elif defined(__GNUC__)
 void __attribute__ ((interrupt(PORT1_VECTOR))) port1_isr_handler (void)
 #else
 #error Compiler not supported!
 #endif
 {
     switch(__even_in_range(P1IV, P1IV__P1IFG7))
     {
         case P1IV__NONE:    break;          // Vector  0:  No interrupt
         case P1IV__P1IFG0:  break;          // Vector  2:  P1.0 interrupt flag
         case P1IV__P1IFG1:                  // Vector  4:  P1.1 interrupt flag
             P1IES ^= BIT1;                  // Toggle interrupt edge
             P1OUT ^= BIT1;                  // Toggle between pull-up vs pull-down R
             __bic_SR_register_on_exit(LPM4_bits); // Exit LPM4
             break;
         case P1IV__P1IFG2:  break;          // Vector  6:  P1.2 interrupt flag
         case P1IV__P1IFG3:  break;          // Vector  8:  P1.3 interrupt flag
         case P1IV__P1IFG4:  break;          // Vector  10:  P1.4 interrupt flag
         case P1IV__P1IFG5:  break;          // Vector  12:  P1.5 interrupt flag
         case P1IV__P1IFG6:  break;          // Vector  14:  P1.6 interrupt flag
         case P1IV__P1IFG7:  break;          // Vector  16:  P1.7 interrupt flag
         default: break;
     }
 }
