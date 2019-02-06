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
;  MSP430G2230 Demo - ADC10, Sample A5, 1.5V, TA1 Trig, Set P1.2 if > 0.5V
;
;  Description: A5 is sampled every ACLK/1024 with reference to 1.5V.
;  Timer_A is run in upmode and TA1 is used to automatically trigger
;  ADC10 conversion, TA0 defines the period. Internal oscillator times sample
;  (16x) and conversion (13x). Inside ADC10_ISR if A5 > 0.5Vcc, P1.2 is set,
;  else reset. Normal mode is LPM3.
;  ACLK = VLO (~10kHz), MCLK = SMCLK = default DCO ~ 1.2MHz, ADC10CLK = ADC10OSC
;
;               MSP430G2230
;            -----------------
;        /|\|              XIN|-
;         | |                 |
;         --|RST          XOUT|-
;           |                 |
;       >---|P1.5/A5     P1.2 |--> LED
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

;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x280,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

            ; Input A5, sample/hold Timer_A.OUT1, rpt single channel mode
            mov.w   #INCH_5+SHS_1+CONSEQ_2,&ADC10CTL1   
            ; Set ref, sample every 16 ADC clks, enable ref, ADC on, enable int
            mov.w   #SREF_1+ADC10SHT_2+REFON+ADC10ON+ADC10IE,&ADC10CTL0 
            mov.w   #30,&TACCR0             ; Delay to allow Ref to settle
            bis.w   #CCIE,&TACCTL0          ; Compare-mode interrupt enable
            mov.w   #TASSEL_2+MC_1+TACLR,&TACTL ; SMCLK, up mode, clear timer
            bis.w   #LPM0+GIE,SR            ; LPM0 (CPU off), ISR forces exit
            bic.w   #CCIE,&TACCTL0          ; Disable timer interrupt
            bic.w   #GIE,SR                 ; Disable global interrupt

            bis.w   #ENC,&ADC10CTL0         ; ADC10 Enable
            bis.b   #BIT5,&ADC10AE0         ; P1.5 ADC option select
            bic.b   #BIT2,&P1OUT            ; Initialize LED as off
            bis.b   #BIT2,&P1DIR            ; Set P1.2 to output direction
            mov.w   #1024-1,&TA0CCR0        ; PWM Period
            mov.w   #OUTMOD_3,TA0CCTL1      ; TA0CCR1 output mode: set/reset
            mov.w   #1023,&TA0CCR1          ; TA0CCR1 PWM Duty Cycle
            mov.w   #TASSEL_1+MC_1,&TA0CTL  ; ACLK, up mode
            bis.w   #LPM0+GIE,SR            ; LPM0 (CPU off), ISR forces exit

;-------------------------------------------------------------------------------
ADC10_ISR
;-------------------------------------------------------------------------------
            cmp.w   #0x155,&ADC10MEM        ; Compare with 0.5V
            jn      less_thn                ; Jump if the ADC result < 0.5V
gtr_thn     bis.b   #BIT2,&P1OUT            ; Set P1.2 LED on
            reti
less_thn    bic.b   #BIT2,&P1OUT            ; Clear P1.2 LED off
            reti

;-------------------------------------------------------------------------------
TIMERA0_ISR
;-------------------------------------------------------------------------------
            clr.w   &TACTL                  ; Clear Timer_A control registers
            bic.w   #LPM0,0(SP)             ; Exit LPM0, interrupts enabled
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
