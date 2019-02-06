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
;  MSP430x24x Demo - Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
;
;  Description: Toggle P1.0 using software and Timer_B overflow ISR.
;  In this example an ISR triggers when TB overflows.  TB overflow has the 
;  lowest priority. Inside the TB overflow ISR P1.0 is toggled. Toggle rate is 
;  approximatlely ~ [1.04MHz/FFFFh]/2. Proper use of TBIV interrupt vector 
;  generator is shown.
;  ACLK = n/a, MCLK = SMCLK = TBCLK = default DCO ~1.045MHz.
;
;           MSP430F249
;         ---------------
;     /|\|            XIN|-
;      | |               |
;      --|RST        XOUT|-
;        |               |
;        |           P1.0|-->LED
;
;  JL Bile
;  Texas Instruments Inc.
;  May 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0500h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP1     bis.b   #01h, &P1DIR            ; Set P1.0 to output direction
            mov.w   #TBSSEL_2 + MC_2 + TBIE, &TBCTL ;SMCLK, contmode, interrupt
                                            ;
Mainloop    bis.w   #CPUOFF+GIE,SR          ; CPU off, interrupts enabled
            nop                             ; Required for debugger
                                            ;
;-------------------------------------------------------------------------------
TBX_ISR;    Common ISR for overflow
;-------------------------------------------------------------------------------
            add.w   &TBIV,PC                ; Add Timer_B offset vector
            reti                            ;
            reti                            ; TBCCR1 not used
            reti                            ; TBCCR2 not used
            reti                            ;
            reti                            ;
            reti                            ;
            reti                            ;
TB_over     xor.b   #001h,&P1OUT            ; Toggle P1.0
            reti                            ; Return from overflow ISR
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".reset"            ; POR, ext. Reset
            .short  RESET                   ;
            .sect   ".int28"          ; Timer_BX Vector
            .short  TBX_ISR                 ;
            .end
