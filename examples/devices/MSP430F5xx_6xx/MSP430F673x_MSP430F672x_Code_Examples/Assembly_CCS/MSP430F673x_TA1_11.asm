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
;  MSP430F673x Demo - Timer1_A1, Toggle P8.4/TA1.0, Up Mode, ACLK = REFO
;
;  Description: Toggle P8.4 using hardware TA1.0 output. Timer1_A is configured
;  for up mode with CCR0 defining period, TA1.0 also output on P8.4. In this
;  example, CCR0 is loaded with 10-1 and TA1.0 will toggle P8.4 at TACLK/10.
;  Thus the output frequency on P8.4 will be the TACLK/20. No CPU or software
;  resources required. Normal operating mode is LPM3.
;  As coded with TACLK = ACLK = REFO, P8.4 output frequency = 32KHz/20 = 1.6kHz.
;  ACLK = TACLK = 32kHz, MCLK = default DCO ~1.045MHz
;
;              MSP430F673x
;            ---------------
;        /|\|               |
;         | |               |
;         --|RST            |
;           |               |
;           |     P8.4/TA1.0|--> ACLK/20
;
;  M. Swanson
;  Texas Instruments Inc.
;  December 2011
;  Built with CCS Version: 5.1.0
;*******************************************************************************
            .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x63FE,SP              ; Initialize stackpointer
            mov.w   #WDTPW + WDTHOLD,&WDTCTL; Stop WDT    
            bis.b   #BIT4,&P8DIR            ; P3.1 output
            bis.b   #BIT4,&P8SEL            ; P3.1 option select
            mov.w   #OUTMOD_4,&TA1CCTL0     ; CCR0 toggle mode
            mov.w   #10-1,&TA1CCR0
            mov.w   #TASSEL_1 + MC_1 + TACLR,&TA1CTL
                                            ; ACLK, upmode, clear TAR
            bis.w   #LPM3,SR                ; Enter LPM3
            nop                             ; For debugger

;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
