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
;*****************************************************************************
;   MSP430x21x1 Demo - 21x1 Pull-up resistors demo
;
;   Description: This program demonstrates how to use the internal pull-ups
;   on the 21x1 device. P1.2 is configured for using the pull-up. P1.0
;   is configured as output to use the LED as an indicator. The P1.2 interrupt
;   is enabled and the device is put into LPM4. When P1.2 goes low, and
;   interrupt is generated and the P1_ISR executes. The ISR implements a
;   software debounce delay, toggles P1.0 and returns.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~1.16MHz
;
;
;               MSP430F21x1
;            -----------------
;       /|\ |              XIN|-
;        |  |                 |
;        ---|RST          XOUT|-
;           |                 |
;           |             P1.2|---- \----
;           |                 |         |
;           |                 |        GND
;
;   H. Grewal / A. Dannenberg
;   Texas Instruments, Inc
;   July 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ;
;------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize 'x1121 stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP1     bis.b   #001h,&P1DIR            ; P1.0 output
            mov.b   #04h,&P1REN             ; Enable resistor on P1.2
            mov.b   #04h,&P1OUT             ; Select pull-up
            mov.b   #04h,&P1IE              ; Enable interrupt
            clr.b   &P1IFG                  ; Clear IFG
            mov.w   #LPM4+GIE,SR            ; Enable interrupts, LPM4
;-----------------------------------------------------------------------------
P1_ISR;     Toggle P1.0 Output
;-----------------------------------------------------------------------------
Wait        mov.w   #01FFFh,R15             ; Delay to R15
L1          dec.w   R15                     ; Decrement R15
            jnz     L1                      ; Delay over?
            xor.b   #001h,&P1OUT            ; P1.0 = toggle
            clr.b   &P1IFG                  ; IFG Cleared
            reti                            ; Return from ISR

;-----------------------------------------------------------------------------
;           Interrupt Vectors Used MSP430
;-----------------------------------------------------------------------------
            .sect ".reset"                  ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect ".int02"                  ; P1.x Vector
            .short  P1_ISR                  ;
            .end
