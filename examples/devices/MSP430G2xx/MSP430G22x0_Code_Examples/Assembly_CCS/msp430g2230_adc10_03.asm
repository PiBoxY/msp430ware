; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2012, Texas Instruments Incorporated
;  All rights reserved.
; 
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
; 
;  *  Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
; 
;  *  Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
; 
;  *  Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
; 
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
;  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
;  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
;  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; 
; ******************************************************************************
;  
;                        MSP430 CODE EXAMPLE DISCLAIMER
; 
;  MSP430 code examples are self-contained low-level programs that typically
;  demonstrate a single peripheral function or device feature in a highly
;  concise manner. For this the code may rely on the device's power-on default
;  register values and settings such as the clock configuration and care must
;  be taken when combining code from several examples to avoid potential side
;  effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
;  for an API functional library-approach to peripheral configuration.
; 
; --/COPYRIGHT--
;******************************************************************************
;  MSP430G2230 Demo - ADC10, Sample A10 Temp, Set P1.2 if Temp ++ ~2C
;
;  Description: Set ADC10 and the integrated temperature sensor to detect
;  temperature gradients. The temperature sensor output voltage is sampled
;  ~ every 120ms and compared with the defined delta values using an ISR.
;  (ADC10OSC/4)/64 determines sample time which needs to be greater than
;  30us for temperature sensor.
;  ADC10 is operated in repeat-single channel mode with the sample and
;  convert trigger sourced from Timer_A CCR1. The ADC10IFG at the end
;  of each converstion will trigger an ISR.
;  ACLK = VLO, MCLK = SMCLK = default DCO ~ 1.2MHz, ADC10CLK = ADC10OSC
;
;                MSP430G2230
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;        >---|P1.5/A5      P1.2|-->LED
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
;******************************************************************************

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

ADCDeltaOn  .set    3
;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x280,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

SetupADC    ; Input A10 (temperature sensor), sample Timer1, ADC10 clk/4, rpt mode
            mov.w   #INCH_10+SHS_1+ADC10DIV_3+CONSEQ_2,&ADC10CTL1   
            ; Sample every 64 ADC clocks, ref 2.5V, interrupt enabled, ADC10ON
            mov.w   #SREF_1+ADC10SHT_3+REF2_5V+ADC10IE+REFON+ADC10ON,&ADC10CTL0 
            bis.w   #GIE,SR
            mov.w   #30,&TA0CCR0            ; Delay to allow Ref to settle
            bis.w   #CCIE,&TA0CCTL0         ; Compare-mode interrupt enable
            mov.w   #TASSEL_2+MC_1,&TA0CTL  ; SMCLK, up mode, clear timer   
            bis.w   #LPM0,SR                ; LPM0 (CPU off), ISR forces exit
            bic.w   #CCIE,&TA0CCTL0         ; Disable timer interrupt
            bic.w   #GIE,SR                 ; Disable all interrupts
            bis.w   #ENC,&ADC10CTL0         ; Enable conversion
            mov.w   #OUTMOD_4,&TA0CCTL1     ; Toggle on EQU1 (TAR = 0)
            mov.w   #TASSEL_2+MC_2,&TA0CTL  ; SMCLK, continuous mode

ADC_Wait    bit.w   #ADC10IFG,&ADC10CTL0    ; Wait for conversion
            jnc     ADC_Wait

            mov.w   &ADC10MEM,R4            ; Store first conversion in R4
            add.w   #ADCDeltaOn,R4          ; Add offset for temperature change
            bic.b   #BIT2,&P1OUT            ; Initialize LED as off
            bis.b   #BIT2,&P1DIR            ; Set P1.2 as an output
            bis.w   #LPM0+GIE,SR            ; LPM0 (CPU off), ISR forces exit
            nop                             ; For debug purposes

;-------------------------------------------------------------------------------
TIMERA0_ISR
;-------------------------------------------------------------------------------
            clr.w   &TA0CTL                 ; Clear Timer_A control registers
            bic.w   #LPM0,0(SP)             ; Exit LPM0, interrupts enabled
            reti

;-------------------------------------------------------------------------------
ADC10_ISR
;-------------------------------------------------------------------------------
            NOP
            mov.w   &ADC10MEM,R5
            cmp     R4,&ADC10MEM
            jn      ISRless_thn
ISRgtr_thn  bis.b   #BIT2,&P1OUT            ; Set P1.2 LED on
            reti
ISRless_thn bic.b   #BIT2,&P1OUT            ; Clear P1.2 LED off
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   ADC10_VECTOR            ; ADC10 isr vector
            .short  ADC10_ISR
            .sect   TIMERA0_VECTOR          ; Timer_A0 Vector
            .short  TIMERA0_ISR
            .end
