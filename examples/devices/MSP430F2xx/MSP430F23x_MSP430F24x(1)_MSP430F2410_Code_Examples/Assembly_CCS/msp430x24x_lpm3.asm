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
;   MSP430x24x Demo - Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK
;
;   Description: This program operates MSP430 normally in LPM3, pulsing P1.0
;   at 4 second intervals. WDT ISR used to wake-up system. All I/O configured
;   as low outputs to eliminate floating inputs. Current consumption does
;   increase when LED is powered on P1.0. Demo for measuring LPM3 current.
;   ACLK = LFXT1/4 = 32768/4, MCLK = SMCLK = default DCO ~1.045MHz
;   //* External watch crystal on XIN XOUT is required for ACLK *//
;
;                MSP430F249
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |             P1.0|-->LED
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
SetupWDT    mov.w   #WDT_ADLY_1000,&WDTCTL  ; WDT 1s*4 interval timer
SetupBC     bis.b   #DIVA_1,&BCSCTL1        ; ACLK/4
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
SetupP1     mov.b   #0FFh,&P1DIR            ; All P1.x outputs
            clr.b   &P1OUT                  ; All P1.x reset
SetupP2     mov.b   #0FFh,&P2DIR            ; All P2.x outputs
            clr.b   &P2OUT                  ; All P2.x reset
SetupP3     mov.b   #0FFh,&P3DIR            ; All P3.x outputs
            clr.b   &P3OUT                  ; All P3.x reset
SetupP4     mov.b   #0FFh,&P4DIR            ; All P4.x outputs
            clr.b   &P4OUT                  ; All P4.x reset
SetupP5     mov.b   #0FFh,&P5DIR            ; All P4.x outputs
            clr.b   &P5OUT 
SetupP6     mov.b   #0FFh,&P6DIR            ; All P4.x outputs
            clr.b   &P6OUT  
            
Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3, enable interrupts
            bis.b   #001h,&P1OUT            ; Set P1.0
            push.w  #20000                  ; Delay to TOS
Delay       dec.w   0(SP)                   ; Decrement TOS
            jnz     Delay                   ; Delay over?
            incd.w  SP                      ; Clean stack
            bic.b   #001h,&P1OUT            ; Reset P1.0
            jmp     Mainloop                ; Again
                                            ;
;-------------------------------------------------------------------------------
WDT_ISR  ;  Exit LPM3 on reti
;-------------------------------------------------------------------------------
            bic.w   #LPM3,0(SP)             ; Clear LPM3 from TOS
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".int26"              ; WDT Vector
            .short	WDT_ISR
            .sect	".reset"            ; POR, ext. Reset
            .short    RESET
            .end
