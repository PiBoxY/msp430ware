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
;*******************************************************************************
;  MSP430F54xA Demo - Timer_A3, PWM TA1.1-2, Up Mode, DCO SMCLK
;
;  Description: This program generates two PWM outputs on P2.2,P2.3 using
;  Timer1_A configured for up mode. The value in CCR0, 512-1, defines the PWM
;  period and the values in CCR1 and CCR2 the PWM duty cycles. Using ~1.045MHz
;  SMCLK as TACLK, the timer period is ~500us with a 75% duty cycle on P2.2
;  and 25% on P2.3.
;  ACLK = n/a, SMCLK = MCLK = TACLK = default DCO ~1.045MHz.
;
;                MSP430F5438A
;            -------------------
;        /|\|                   |
;         | |                   |
;         --|RST                |
;           |                   |
;           |         P2.2/TA1.1|--> CCR1 - 75% PWM
;           |         P2.3/TA1.2|--> CCR2 - 25% PWM
;
;   D. Dang
;   Texas Instruments Inc.
;   December 2009
;   Built with CCS Version: 4.0.2 
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
RESET       mov.w   #0x5C00,SP              ; Initialize stackpointer
            mov.w   #WDTPW + WDTHOLD,&WDTCTL; Stop WDT    
            bis.b   #BIT2 + BIT3,&P2DIR     ; P2.2 and P2.3 output
            bis.b   #BIT2 + BIT3,&P2SEL     ; P2.2 and P2.3 options select
            mov.w   #512-1,&TA1CCR0         ; PWM Period
            mov.w   #OUTMOD_7,&TA1CCTL1     ; CCR1 reset/set
            mov.w   #384,&TA1CCR1           ; CCR1 PWM duty cycle
            mov.w   #OUTMOD_7,&TA1CCTL2     ; CCR2 reset/set
            mov.w   #128,&TA1CCR2           ; CCR2 PWM duty cycle
            mov.w   #TASSEL_2 + MC_1 + TACLR,&TA1CTL
                                            ; SMCLK, up mode, clear TAR
            bis.w   #LPM0,SR                ; Enter LPM0
            nop                             ; For debugger
  
;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
