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
;  MSP430G2230 Demo - ADC10, DTC Sample A5 32x, AVcc, TA0 Trig, DCO
;
;  Description; A5 is sampled in 32x burst using DTC 16 times per second
;  (ACLK/2048) with reference to AVcc. Activity is interrupt driven.
;  Timer_A in upmode uses TA0 toggle to drive ADC10 conversion. Sample burst
;  is automatically triggered by TA0 rising edge every 2048 ACLK cycles.
;  ADC10_ISR will exit from LPM3 mode and return CPU active. Internal ADC10OSC
;  times sample (16x) and conversion (13x). DTC transfers conversion code to
;  RAM 200h - 240h. In the Mainloop P1.2 is toggled. Normal Mode is LPM3.
;  * An external watch crystal on XIN XOUT is required for ACLK *//
;
;               MSP430G2230
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
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

            ; Input A5, sample/hold source Timer_A.OUT0, rpt single channel mode
            mov.w   #INCH_5+SHS_2+CONSEQ_2,&ADC10CTL1   
            ; Sample every 16 ADC clocks, ADC on, enable interrupt
            mov.w   #ADC10SHT_2+MSC+ADC10ON+ADC10IE,&ADC10CTL0 
            mov.b   #0x20,&ADC10DTC1        ; 32 conversions
            bic.b   #BIT2,&P1OUT            ; Initialize LED as off
            bis.b   #BIT2,&P1DIR            ; Set P1.2 to output direction
            bis.b   #BIT5,&ADC10AE0         ; P1.5 ADC option select
            mov.w   #1024-1,&TA0CCR0        ; PWM Period
            mov.w   #OUTMOD_4,&TA0CCTL0     ; TA0CCR0 toggle
            mov.w   #TASSEL_1+MC_1,&TA0CTL  ; ACLK, up mode

loop        bic.w   #ENC,&ADC10CTL0         ; clear ENC
busy_test   bit     #BUSY,&ADC10CTL1        ; ADC10 core inactive?
            jnz     busy_test
            mov.w   #0x200,&ADC10SA         ; Set start address
            bis.w   #ENC,&ADC10CTL0         ; set ENC
            bis.w   #LPM0+GIE,SR            ; LPM0 (CPU off), ISR forces exit
            xor.b   #BIT2,&P1OUT            ; Toggle P1.2 using exclusive-OR
            jmp     loop

;-------------------------------------------------------------------------------
ADC10_ISR
;-------------------------------------------------------------------------------
            bic.w   #LPM0,0(SP)             ; Exit active CPU
            reti                            ; Vector 0:  No interrupt

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   ADC10_VECTOR            ; ADC10 isr vector
            .short  ADC10_ISR
            .end
