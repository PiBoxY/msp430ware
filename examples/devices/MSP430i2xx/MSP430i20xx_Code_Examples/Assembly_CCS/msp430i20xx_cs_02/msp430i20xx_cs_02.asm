; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2013, Texas Instruments Incorporated
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
;  MSP430i20xx Demo - Basic Clock, MCLK = DCO/2, SMCLK = DCO/4, Internal Resistor
;
;  Description: Buffer MCLK(DCO/2) on P1.0 and SMCLK(DCO/4) on P1.1
;
;  ACLK = 32kHz, MCLK = DCO/2 = 8.192MHz, SMCLK = DCO/4 = 4.096MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;               MSP430i20xx
;             -----------------
;         /|\|                |
;          | |                |
;          --|RST             |
;            |                |
;            |            P1.0|-->MCLK  = DCO/2
;            |            P1.1|-->SMCLK = DCO/4
;
;   T. Witt
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio v5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

            .ref    init                    ; Reference external function

RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
Cal         call    #init                   ; Calibrate peripherals
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            clr.b   &CSCTL0                 ; Internal resistor
            bis.b   #DIVM__2|DIVS__4,&CSCTL1; MCLK = DCO/2, SMCLK = DCO/4
            bis.b   #BIT0|BIT1,&P1DIR       ; P1.0, P1.1 output
            bis.b   #BIT0|BIT1,&P1SEL1      ; MCLK, SMCLK Pin Function
            bic.b   #BIT0|BIT1,&P1SEL0

Mainloop    nop                             ; Do nothing, let clocks run
            jmp     Mainloop                ; Again
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .end
