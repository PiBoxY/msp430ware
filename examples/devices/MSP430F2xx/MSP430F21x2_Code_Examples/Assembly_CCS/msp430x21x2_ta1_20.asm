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
;   MSP430F21x2 Demo - Timer1_A, PWM TA1_1, Up/Down Mode, 32kHz ACLK
;
;   Description: This program generates one PWM outputs on P3.7 using
;   Timer1_A configured for up/down mode. The value in TA1CCR0, 128, defines the
;   PWM period/2 and the values in TA1CCR1 and TA1CCR2 the PWM duty cycles. Using
;   32kHz ACLK as TA1CLK, the timer period is 7.8ms with a 75% duty cycle on
;   P3.7. Normal operating mode is LPM3.
;   ACLK = TA1CLK = LFXT1 = 32768Hz, MCLK = default DCO ~1.2MHz
;   //* External watch crystal on XIN XOUT is required for ACLK *//
;
;                MSP430F21x2
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |       P3.7/TA1_1|--> TAC1CCR1 - 75% PWM
;            |                 |
;
;  JL Bile
;  Texas Instruments Inc.
;  May 2008
;  Built with Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
 			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #025Fh,SP         		; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP3     bis.b   #080h,&P3DIR            ; P3.7 output
            bis.b   #080h,&P3SEL            ; P3.7 TA1_2 otion
SetupC0     mov.w   #128,&TA1CCR0           ; PWM Period/2
SetupC1     mov.w   #OUTMOD_6,&TA1CCTL1     ; TA1CCR1 toggle/set
            mov.w   #32,&TA1CCR1            ; TA1CCR1 PWM Duty Cycle
SetupTA     mov.w   #TASSEL_1+MC_3,&TA1CTL   ; ACLK, updown mode
                                            ;
Mainloop    bis.w   #LPM3,SR                ; Enter LPM3
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".reset"				; MSP430 RESET Vector
            .short	RESET                   ;
            .end
