/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
#include "TI_DRV_IEC60730.h"

// The following section contains the interrupt service routines (ISR) for all
// unused interrupts for DRV91670 Sensorless BLDC firmware.
extern uint16_t gInterruptCounter;

// Contains Watchdog Timer ISR
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void){

	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter&BITF){
		gInterruptCounter += TEST_WDT_INTERRUPT;
		SFRIE1 &= ~WDTIE;
		SFRIFG1 &= ~WDTIFG;

		#if ENABLED_WDT
				WDTCTL = WDTPW + WDTCNTCL + WDTIS1;
		#else
				WDTCTL = WDTPW + WDTHOLD;
		#endif
	}
}

// Contains PORT4 ISR
#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void){
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter&BITF){
		gInterruptCounter += TEST_PORT4_INTERRUPT;
		P4IE &=  ~BIT7;
		P4IFG &= ~BIT7;
	}
}

// Contains DMA ISR
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR (void){
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter&BITF){
		gInterruptCounter += TEST_DMA_INTERRUPT;
		DMA0CTL &= ~(DMAIE + DMAIFG);
	}
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void){
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter&BITF){
		gInterruptCounter += TEST_USCIB0_INTERRUPT;
		UCB0IE &= ~ UCTXIE;
	}
}


#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void){
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter&BITF){
		gInterruptCounter += TEST_PORT2_INTERRUPT;
		P2IE &=  ~BIT7;
		P2IFG &= ~BIT7;
	}
}





















