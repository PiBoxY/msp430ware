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
;   MSP-FET430P440 Demo - Timer_B, PWM TB1-6 Up Mode, DCO SMCLK
;
;   Description: This program outputs six PWM signals on P2.2,3 and P3.4-7
;   using Timer_B configured for up mode. The value in CCR0 defines the PWM
;   period and the values in CCR1-6 the PWM duty cycles. Using SMCLK as TBCLK,
;   the timer period is ~488us.
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   ;* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;                MSP430F449
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |         P2.2/TB1|--> CCR1 - 75% PWM
;            |         P2.3/TB2|--> CCR2 - 25% PWM
;            |         P3.4/TB3|--> CCR3 - 12.5% PWM
;            |         P3.5/TB4|--> CCR4 - 6.25% PWM
;            |         P3.6/TB5|--> CCR5 - 3.125% PWM
;            |         P3.7/TB6|--> CCR6 - 1.5625% PWM
;
;   M. Buccini / A. Dannenberg
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                  			; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
SetupPx     bis.b   #0ch,&P2DIR             ; P2.2-P2.3 output
            bis.b   #0ch,&P2SEL             ; P2.2-P2.3 TB1-6 options
            bis.b   #0F0h,&P3DIR            ; P3.4-P3.7 output
            bis.b   #0F0h,&P3SEL            ; P3.4-P3.7 TB1-6 options
SetupC0     mov.w   #512-1,&TBCCR0          ; PWM Period
SetupC1     mov.w   #OUTMOD_7,&TBCCTL1      ; CCR1 reset/set
            mov.w   #384,&TBCCR1            ; CCR1 PWM Duty Cycle	
SetupC2     mov.w   #OUTMOD_7,&TBCCTL2      ; CCR2 reset/set
            mov.w   #192,&TBCCR2            ; CCR2 PWM duty cycle	
SetupC3     mov.w   #OUTMOD_7,&TBCCTL3      ; CCR3 reset/set
            mov.w   #96,&TBCCR3             ; CCR3 PWM duty cycle	
SetupC4     mov.w   #OUTMOD_7,&TBCCTL4      ; CCR4 reset/set
            mov.w   #48,&TBCCR4             ; CCR4 PWM duty cycle	
SetupC5     mov.w   #OUTMOD_7,&TBCCTL5      ; CCR5 reset/set
            mov.w   #24,&TBCCR5             ; CCR5 PWM duty cycle	
SetupC6     mov.w   #OUTMOD_7,&TBCCTL6      ; CCR6 reset/set
            mov.w   #12,&TBCCR6             ; CCR6 PWM duty cycle	
SetupTB     mov.w   #TBSSEL_2+MC_1,&TBCTL   ; SMCLK, upmode
                                            ;					
Mainloop    bis.w   #CPUOFF,SR              ; CPU off
            nop                             ; Required only for debugger
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .end
