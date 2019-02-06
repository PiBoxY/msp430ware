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
//   MSP430F665x Demo - Software Toggle P1.1 with 12MHz DCO
//
//   Description: Toggle P1.1 by xor'ing P1.1 inside of a software loop.
//   ACLK is brought out on pin P1.0, SMCLK = MCLK on P3.4.
//   PMMCOREV = 1 to support up to 12MHz clock
//  
//   ACLK = REFO = 32kHz, MCLK = SMCLK = 12MHz
//
//                 MSP430F665x
//             -----------------
//         /|\|                 |
//          | |             P1.0|-->ACLK
//          --|RST              |
//            |             P3.4|-->SMCLK = MCLK
//            |                 |
//            |             P1.1|-->Port Pin
//   Note: 
//   In order to run the system at up to 12MHz, VCore must be set at 1.6V 
//   or higher. 
//
//   P. Thanigai
//   Texas Instruments Inc.
//   October 2012
//   Built with IAR Embedded Workbench Version: 5.40 & CCS V5.2
//******************************************************************************
#include <msp430.h>
#include <stdint.h>


#define PMM_STATUS_OK     0
#define PMM_STATUS_ERROR  1
#define _HAL_PMM_SVMLE  (SVMLE)
#define _HAL_PMM_SVSLE  (SVSLE)
#define _HAL_PMM_SVSFP  (SVSLFP)
#define _HAL_PMM_SVMFP  (SVMLFP)


uint16_t SetVCore (uint8_t level);
static uint16_t SetVCoreUp (uint8_t level);



int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT
  SetVCore(PMMCOREV_1);                     // Set VCore = 1.6V for 12MHz clock
  P1DIR |= BIT1;                            // P1.1 output

  P1DIR |= BIT0;                            // ACLK set out to pins
  P1SEL |= BIT0;                            
  P3DIR |= BIT4;                            // SMCLK set out to pins
  P3SEL |= BIT4;                            

  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
  UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (374 + 1) * 32768 = 12MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 6xx
  // UG for optimization.
  // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
  __delay_cycles(375000);
	
  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
	
  while(1)
  {
    P1OUT ^= BIT1;                          // Toggle P1.1
    __delay_cycles(600000);                 // Delay
  }
}

uint16_t SetVCore (uint8_t level)
{
  uint16_t actlevel;
  uint16_t status = 0;
  level &= PMMCOREV_3;                       // Set Mask for Max. level
  actlevel = (PMMCTL0 & PMMCOREV_3);         // Get actual VCore

  while (((level != actlevel) && (status == 0)) || (level < actlevel))		// step by step increase or decrease
  {
    if (level > actlevel)
      status = SetVCoreUp(++actlevel);
  }
  return status;
}

static uint16_t SetVCoreUp (uint8_t level)
  {
    uint16_t PMMRIE_backup,SVSMHCTL_backup;

    // Open PMM registers for write access
    PMMCTL0_H = 0xA5;

    // Disable dedicated Interrupts to prevent that needed flags will be cleared
    PMMRIE_backup = PMMRIE;
    PMMRIE &= ~(SVSMHDLYIE | SVSMLDLYIE | SVMLVLRIE | SVMHVLRIE | SVMHVLRPE);
    SVSMHCTL_backup = SVSMHCTL;
    PMMIFG &= ~(SVMHIFG | SVSMHDLYIFG);
    // Set SVM highside to new level and check if a VCore increase is possible
    SVSMHCTL = SVMHE | SVSHE | (SVSMHRRL0 * level);    
    // Wait until SVM highside is settled
    while ((PMMIFG & SVSMHDLYIFG) == 0);  
    // Check if a VCore increase is possible
    if ((PMMIFG & SVMHIFG) == SVMHIFG){       //-> Vcc is to low for a Vcore increase
      // recover the previous settings
      PMMIFG &= ~SVSMHDLYIFG;
      SVSMHCTL = SVSMHCTL_backup;
      // Wait until SVM highside is settled
      while ((PMMIFG & SVSMHDLYIFG) == 0);
      // Clear all Flags
      PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG);
      // backup PMM-Interrupt-Register
      PMMRIE = PMMRIE_backup;
 
      // Lock PMM registers for write access
      PMMCTL0_H = 0x00;
      return PMM_STATUS_ERROR;            // return: voltage not set
    }
    // Set also SVS highside to new level	    //-> Vcc is high enough for a Vcore increase
    SVSMHCTL |= (SVSHRVL0 * level);
    // Set SVM low side to new level
    SVSMLCTL = SVMLE | (SVSMLRRL0 * level);
    // Wait until SVM low side is settled
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    // Clear already set flags
    PMMIFG &= ~(SVMLVLRIFG | SVMLIFG);
    // Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;
    // Wait until new level reached
    if (PMMIFG & SVMLIFG)
      while ((PMMIFG & SVMLVLRIFG) == 0);
    // Set also SVS/SVM low side to new level
    PMMIFG &= ~SVSMLDLYIFG;
    SVSMLCTL |= SVSLE | (SVSLRVL0 * level);
    // wait for lowside delay flags
    while ((PMMIFG & SVSMLDLYIFG) == 0);

    // Disable SVS/SVM Low
    // Disable full-performance mode to save energy
    SVSMLCTL &= ~(_HAL_PMM_SVSLE + _HAL_PMM_SVMLE + _HAL_PMM_SVSFP + _HAL_PMM_SVMFP);
    SVSMHCTL &= ~(_HAL_PMM_SVSFP + _HAL_PMM_SVMFP);
  
    // Clear all Flags
    PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG);
    // backup PMM-Interrupt-Register
    PMMRIE = PMMRIE_backup;

    // Lock PMM registers for write access
    PMMCTL0_H = 0x00;
    return PMM_STATUS_OK;                               // return: OK
  }

