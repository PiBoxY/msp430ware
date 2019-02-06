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
//   CC430x614x Demo - RTC_D, LPM3.5, & alarm
//
//   Description: The RTC_D module is used to set the time, start RTC operation,
//   and read the time from the respective RTC registers. Software will set the
//   original time to 11:59:55 am on Friday October 7, 2011. Then the RTC will
//   be activated through software, and an alarm will be created for the next 
//   minute (12:00:00 pm). The device will then enter LPM3.5 awaiting
//   the event interrupt. The device will flash the LED and then reset the RTC
//   to the time above upon being woken up by the event.
//
//   NOTE: This code example was created and tested using the EM430F6147RF900
//   Rev3.4 board. To ensure that LPM3.5 is entered and exited properly:
//
//   1. In debug mode:  enable  LPM3.5 debug option for the project and do not 
//      select "Release JTAG on Go” option in IAR and “Free Run” option in CCS.
// 
//   2. In stand-alone mode (JTAG disconnected, for accurate current measurements): 
//      remove JTAG after programming device and externally power board. A reset 
//      may be required after removing the JTAG. When device is in LPM3.5 ~1uA 
//      should be measured on JP2. 
//
//  //* An external watch crystal on XIN XOUT is required for ACLK *//	
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                CC430x614x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|--> LED
//
//   G. Larmore
//   Texas Instruments Inc.
//   June 2012
//   Built with IAR Embedded Workbench V5.40.1 & Code Composer Studio V5.2
//******************************************************************************

#include <msp430.h>

void Board_Init(void);
void Clock_Init(void);
void RTC_Init(void);
void EnterLPM35(void);

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Init board and clock
  Board_Init();
  Clock_Init();
  
  // If waking up from LPM3.5
  if(SYSRSTIV == SYSRSTIV_LPM5WU)
  {
    // If RTC time is not noon, blink LED fast
    if((RTCHOUR != 12)||(RTCMIN != 0)||(RTCSEC != 0))
    {
      while(1)                                  // continuous loop
      {
        P1OUT ^= BIT0;                          // XOR P1.0
        __delay_cycles(20000);                  // Delay
      }
    }
  }
  
  // Init RTC, blink LED, and enter LPM3.5
  RTC_Init();
  P1OUT |= BIT0;
  __delay_cycles(100000);
  P1OUT &= ~BIT0;
  EnterLPM35();

  // Code should not get here
  // Blink LED slowly if LPM3.5 not entered properly
  Board_Init();
  Clock_Init();
  while(1)
  {
    P1OUT ^= BIT0;                          // XOR P1.0
    __delay_cycles(200000);                 // Delay
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTCISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTCISR (void)
#else
#error Compiler not supported!
#endif
{
  RTCCTL01&= ~(RTCTEVIFG);                //Clear event interrupt
  RTCCTL01 &= ~(RTCTEVIE);                //Clear interrupt enable
  __bic_SR_register_on_exit(LPM4_bits);     // Exit LPM3.5
  __no_operation();
}

void Clock_Init(void)
{
  // Setup Clock
  P5SEL |= BIT0 + BIT1;                     // Port select XT1
  UCSCTL6 |= XCAP_3 + XT1OFF + XT2OFF;      // Internal load cap
                                            // XT1 and XT2 off
  // Loop until XT1 & DCO stabilizes - In this case loop until XT1 and DCO settle
  do
  {
	UCSCTL7 &= ~(XT1LFOFFG + DCOFFG);
                                            // Clear XT1,DCO fault flags
	SFRIFG1 &= ~OFIFG;                  // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive
                                            // strength
}

void Board_Init(void)
{
  PMMCTL0_H = PMMPW_H;                      // open PMM
  PM5CTL0 &= ~LOCKIO;                       // Clear LOCKBAK and enable ports
  PMMCTL0_H = 0x00;                         // close PMM

  // Port Configuration
  P1OUT = 0x00;P2OUT = 0x00;P3OUT = 0x00;P4OUT = 0x00;P5OUT = 0x00;PJOUT = 0x00;
  P1DIR = 0xFF;P2DIR = 0xFF;P3DIR = 0xFF;P4DIR = 0xFF;P5DIR = 0xFF;PJDIR = 0xFF;
}

void RTC_Init(void)
{
  // Configure RTC_D
  RTCCTL01&= ~(RTCTEVIFG);                //Clear event interrupt
  RTCCTL01 |= RTCTEVIE + RTCHOLD;         //RTC hold, enable RTC
                                          // event interrupt
  RTCYEAR = 2011;                         // Year = 2011
  RTCMON = 10;                            // Month = 10 = October
  RTCDAY = 07;                            // Day = 7 = 7th
  RTCDOW = 05;                            // Day of week = 5 = Friday
  RTCHOUR = 11;                           // Hour = 11
  RTCMIN = 59;                            // Minute = 59
  RTCSEC = 55;                            // Seconds = 55
  RTCCTL01|= RTCTEV_0;                      // Set RTCTEV for 1 minute alarm
  RTCCTL01 &= ~(RTCHOLD);                   // Start RTC calendar mode
}

void EnterLPM35(void)
{
  __enable_interrupt();
  PMMCTL0_H = PMMPW_H;                      // Open PMM Registers for write
  PMMCTL0_L |= PMMREGOFF;                   // and set PMMREGOFF
  __bis_SR_register(LPM4_bits);             // Enter LPM3.5 mode with interrupts
  __no_operation();                         // enabled
}
