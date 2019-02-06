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
;  MSP430G2x55 Demo - Timer0_A3, Toggle P1.1/TA0_0, Up/Down Mode, DCO SMCLK
;
;  Description: Toggle P1.1 using hardware TA0_0 output. Timer0_A3 is configured
;  for up/down mode with TA0CCR0 defining period, TA0_0 also output on P1.1. In
;  this example, TA0CCR0 is loaded with 250 and TA0_0 will toggle P1.1 at
;  TA0CLK/2*250. Thus the output frequency on P1.1 will be the TA0CLK/1000.
;  No CPU or software resources required.
;  ACLK = n/a, SMCLK = MCLK = TA0CLK = default DCO ~1.2MHz
;  As coded with TA0CLK = SMCLK, P1.1 output frequency is ~1200000/1000
;
;           MSP430G2x55
;         -----------------
;     /|\|              XIN|-
;      | |                 |
;      --|RST          XOUT|-
;        |                 |
;        |       P1.1/TA0_0|--> SMCLK/1000
;
;  Xiaodong Li
;  Texas Instruments Inc.
;  March 2013
;  Built with Code Composer Studio v5.3
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Program reset
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section
            .retainrefs                     ; Additionally retain any sections
                                            ; that have references to current
                                            ; section
            .global RESET                   ; Define entry point
;-------------------------------------------------------------------------------
RESET       mov.w   #025Fh,SP                 ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP1     bis.b   #002h,&P1DIR            ; P1.1 output
            bis.b   #002h,&P1SEL            ; P1.1 option slect
SetupC0     mov.w   #OUTMOD_4,&TA0CCTL0      ; TA0CCR0 toggle mode
            mov.w   #250,&TA0CCR0            ;
SetupTA     mov.w   #TASSEL_2+MC_3,&TA0CTL   ; SMCLK, updown mode
                                            ;
Mainloop    bis.w   #CPUOFF,SR              ; CPU off
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
;            Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect    ".reset"                ; MSP430 RESET Vector
            .short    RESET                   ;
            .end

