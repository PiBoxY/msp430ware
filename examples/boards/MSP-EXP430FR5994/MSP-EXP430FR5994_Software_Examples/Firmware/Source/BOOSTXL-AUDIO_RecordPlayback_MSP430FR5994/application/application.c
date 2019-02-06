/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
//******************************************************************************
//!  TI-Design Signal Processing with LEA on MSP430FR5994
//!
//!  William Goh
//!  Texas Instruments Inc.
//!  April 2016
//******************************************************************************

#include <driverlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "application.h"

uint16_t dacBufferPlaybackPointer = 0;

#pragma PERSISTENT(applicationMode)
applicationMode_t applicationMode = DEFAULT;

void runApplication(void)
{
    while(1)
    {
		// Disable button interrupt
		GPIO_disableInterrupt(PUSHBUTTON1_PORT, PUSHBUTTON1_PIN);
		GPIO_disableInterrupt(PUSHBUTTON2_PORT, PUSHBUTTON2_PIN);

    	switch(applicationMode)
        {
            case RECORD:
                runRecord();
                break;
            case PLAYBACK:
                runPlayback();
                break;
            default: break;
        }

        // Toggle app mode
		applicationMode = DEFAULT;

    	// Set a Switch debounce to 500ms
        __delay_cycles(0.5 * __SYSTEM_FREQUENCY_MHZ__);

        // Clear the interrupt flag
        GPIO_clearInterrupt(PUSHBUTTON1_PORT, PUSHBUTTON1_PIN);

        // Re-enable the interrupt
        GPIO_enableInterrupt(PUSHBUTTON1_PORT, PUSHBUTTON1_PIN);

        // Clear the interrupt flag
        GPIO_clearInterrupt(PUSHBUTTON2_PORT, PUSHBUTTON2_PIN);

        // Re-enable the interrupt
        GPIO_enableInterrupt(PUSHBUTTON2_PORT, PUSHBUTTON2_PIN);

        // Enter low power mode
		__bis_SR_register(LPM0_bits + GIE);
    }
}

//******************************************************************************
// Port 5 interrupt service routine
//      This ISR handles the push button to switch the application
//      mode.
//******************************************************************************
#pragma vector=PORT5_VECTOR
__interrupt void port5IsrHandler(void)
{
    switch (__even_in_range(P5IV, P5IV_P5IFG7))
    {
        case P5IV_NONE: break;
        case P5IV_P5IFG0: break;
        case P5IV_P5IFG1: break;
        case P5IV_P5IFG2: break;
        case P5IV_P5IFG3: break;
        case P5IV_P5IFG4: break;
        case P5IV_P5IFG5:
        	// Toggle record mode
			applicationMode = PLAYBACK;

			//exit LPM mode on exit
			__bic_SR_register_on_exit(LPM0_bits);
			break;
        case P5IV_P5IFG6:
            // Toggle record mode
            applicationMode = RECORD;

			//exit LPM mode on exit
            __bic_SR_register_on_exit(LPM0_bits);
			break;
        case P5IV_P5IFG7: break;
        default: break;
   }
}

//******************************************************************************
// DMA interrupt service routine
//******************************************************************************
#pragma vector=DMA_VECTOR
__interrupt void dmaIsrHandler(void)
{
    switch (__even_in_range(DMAIV, DMAIV_DMA2IFG))
    {
        case DMAIV_DMA0IFG: break;
        case DMAIV_DMA1IFG:
            // Disable the dma transfer
            DMA_disableTransfers(DMA_CHANNEL_1);

            // Disable DMA channel 1 interrupt
			DMA_disableInterrupt(DMA_CHANNEL_1);

			 // Enable DMA channel 2 interrupt
			DMA_enableInterrupt(DMA_CHANNEL_2);

			 // Enable DMA channel 2 transfers
            DMA_enableTransfers(DMA_CHANNEL_2);
            break;
        case DMAIV_DMA2IFG:
        	// Disable the dma transfer
			DMA_disableTransfers(DMA_CHANNEL_2);

			// Disable DMA channel 2 interrupt
			DMA_disableInterrupt(DMA_CHANNEL_2);

        	// Clear LED_1
			P1OUT &= ~BIT0;

		    //shutdown audio collect
			Audio_stopCollect(&gAudioConfig);
		    Audio_shutdownCollect(&gAudioConfig);

		    // Start Cpu on exit
			__bic_SR_register_on_exit(LPM4_bits);
            break;
        default: break;
   }
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
    {
        case ADC12IV_NONE:        break;    // Vector  0:  No interrupt
        case ADC12IV_ADC12OVIFG:  break;    // Vector  2:  ADC12MEMx Overflow
        case ADC12IV_ADC12TOVIFG: break;    // Vector  4:  Conversion time overflow
        case ADC12IV_ADC12HIIFG:  break;    // Vector  6:  ADC12HI
        case ADC12IV_ADC12LOIFG:  break;    // Vector  8:  ADC12LO
        case ADC12IV_ADC12INIFG:  break;    // Vector 10:  ADC12IN
        case ADC12IV_ADC12IFG0:   break;    // Vector 12:  ADC12MEM0 Interrupt
        case ADC12IV_ADC12IFG1:   break;    // Vector 14:  ADC12MEM1
        case ADC12IV_ADC12IFG2:   break;    // Vector 16:  ADC12MEM2
        case ADC12IV_ADC12IFG3:   break;    // Vector 18:  ADC12MEM3
        case ADC12IV_ADC12IFG4:   break;    // Vector 20:  ADC12MEM4
        case ADC12IV_ADC12IFG5:   break;    // Vector 22:  ADC12MEM5
        case ADC12IV_ADC12IFG6:   break;    // Vector 24:  ADC12MEM6
        case ADC12IV_ADC12IFG7:   break;    // Vector 26:  ADC12MEM7
        case ADC12IV_ADC12IFG8:   break;    // Vector 28:  ADC12MEM8
        case ADC12IV_ADC12IFG9:   break;    // Vector 30:  ADC12MEM9
        case ADC12IV_ADC12IFG10:  break;    // Vector 32:  ADC12MEM10
        case ADC12IV_ADC12IFG11:  break;    // Vector 34:  ADC12MEM11
        case ADC12IV_ADC12IFG12:  break;    // Vector 36:  ADC12MEM12
        case ADC12IV_ADC12IFG13:  break;    // Vector 38:  ADC12MEM13
        case ADC12IV_ADC12IFG14:  break;    // Vector 40:  ADC12MEM14
        case ADC12IV_ADC12IFG15:  break;    // Vector 42:  ADC12MEM15
        case ADC12IV_ADC12IFG16:  break;    // Vector 44:  ADC12MEM16
        case ADC12IV_ADC12IFG17:  break;    // Vector 46:  ADC12MEM17
        case ADC12IV_ADC12IFG18:  break;    // Vector 48:  ADC12MEM18
        case ADC12IV_ADC12IFG19:  break;    // Vector 50:  ADC12MEM19
        case ADC12IV_ADC12IFG20:  break;    // Vector 52:  ADC12MEM20
        case ADC12IV_ADC12IFG21:  break;    // Vector 54:  ADC12MEM21
        case ADC12IV_ADC12IFG22:  break;    // Vector 56:  ADC12MEM22
        case ADC12IV_ADC12IFG23:  break;    // Vector 58:  ADC12MEM23
        case ADC12IV_ADC12IFG24:  break;    // Vector 60:  ADC12MEM24
        case ADC12IV_ADC12IFG25:  break;    // Vector 62:  ADC12MEM25
        case ADC12IV_ADC12IFG26:  break;    // Vector 64:  ADC12MEM26
        case ADC12IV_ADC12IFG27:  break;    // Vector 66:  ADC12MEM27
        case ADC12IV_ADC12IFG28:  break;    // Vector 68:  ADC12MEM28
        case ADC12IV_ADC12IFG29:  break;    // Vector 70:  ADC12MEM29
        case ADC12IV_ADC12IFG30:  break;    // Vector 72:  ADC12MEM30
        case ADC12IV_ADC12IFG31:  break;    // Vector 74:  ADC12MEM31
        case ADC12IV_ADC12RDYIFG: break;    // Vector 76:  ADC12RDY
        default: break;
    }
}
