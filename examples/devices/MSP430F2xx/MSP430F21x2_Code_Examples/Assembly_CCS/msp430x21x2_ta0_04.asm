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
;  MSP430F21x2 Demo - Timer0_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
;
;  Description: Toggle P1.0 using software and the Timer0_A1 overflow ISR.
;  In this example an ISR triggers when TA overflows. Inside the ISR P1.0
;  is toggled. Toggle rate is exactly 0.5Hz.
;  Proper use of the TA0IV interrupt vector generator is demonstrated.
;  ACLK = TA0CLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO ~1.2MHz
;  /* An external watch crystal on XIN XOUT is required for ACLK */
;
;              MSP430F21x2
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
;         --|RST          XOUT|-
;           |                 |
;           |             P1.0|-->LED
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
RESET       mov.w   #025Fh,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP1     bis.b   #001h,&P1DIR            ; P1.0 output
SetupTA     mov.w   #TASSEL_1+MC_2+TAIE,&TA0CTL  ; ACLK, contmode interrupt
                                            ;
Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3, interrupts enabled
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
TAX_ISR;    Common ISR for overflow
;-------------------------------------------------------------------------------
            add.w   &TA0IV,PC                ; Add Timer_A offset vector
            reti
            reti                            ; TA0CCR1 not used
            reti                            ; TA0CCR2 not used
            reti
            reti
TA_over     xor.b   #001h,&P1OUT            ; Toggle P1.0
            reti                            ; Return from overflow ISR
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vector
;-------------------------------------------------------------------------------
            .sect	".reset"				; MSP430 RESET Vector
            .short	RESET                   ;
            .sect	".int08"        		; Timer_AX Vector
            .short	TAX_ISR                 ;
            .end            
