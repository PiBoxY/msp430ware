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
;   MSP430F20xx Demo - USICNT Used as a One-Shot Timer Function, DCO SMCLK
;
;   Description: USI conunter is used as a one-shot timer. The USI
;   conunter ISR toggles P1.0 using software. As coded ACLK is pre-divided by
;   128 and the USICNT loaded with 32, for a total divided of 4096 or
;   12-bits. USICNT must be reloaded after each interrupt which also clears
;   interrupt flag.
;   ACLK = VLO, MCLK = SMCLK = default DCO
;
;         MSP430F20xx
;              -----------------
;          /|\|              XIN|-
;           | |                 |
;           --|RST          XOUT|-
;             |                 |
;             |             P1.0|-->LED
;
;   P.Thanigai
;   Texas Instruments Inc.
;   May 2007
;   Built with Code Composer Essentials Version: 2.0
;*******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP1     bis.b   #01h,&P1DIR             ; P1.1 output direction

SetupBC     bis.b   #LFXT1S_2,&BCSCTL1      ; ACLK = VL0
SetupUSI    bis.b   #USIMST,&USICTL0        ; SPI master
            bis.b   #USIIE,&USICTL1         ; Counter interrupt, flag remains
            mov.b   #USIDIV_7+USISSEL_1,&USICKCTL    ; /128 ACLK
            bic.b   #USISWRST,&USICTL0      ; Enable USI
            mov.b   #08h,&USICNT            ; init-load counter
                                            ;
Mainloop    bis.w   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
            jmp     Mainloop                ; Again
                                            ;
;-------------------------------------------------------------------------------
USI_ISR  ;
;-------------------------------------------------------------------------------
            xor.b   #01h,&P1OUT             ; Toggle P1.0
            bis.b   #01Fh,&USICNT           ; re-load counter
            reti                            ;
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int04"                ; USI Vector
            .short  USI_ISR                 ;        
            .end
