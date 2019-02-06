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
;  MSP430G22x0 Demo - Basic Clock, Configure RST/NMI as NMI
;
;  Description: Configure RST/NMI as NMI, hi/lo edge. Toggle P1.2  inside of
;  NMI_ISR if NMI occurs. General enable interrupt in status register does
;  not need to be set for NMI. NMIIE does need to be reset, as NMI_ISR
;  automatically clears NMI enable to prevent unintentional stack overflow
;  that could result from, bounce or uncontrolled NMI's.
;  ACLK = VLO, MCLK = SMCLK = DCO ~ 800k
;
;               MSP430G22x0
;            -----------------
;        /|\|              XIN|-
;         | |                 |
;         --|RST/NMI      XOUT|-
;           |                 |
;           |             P1.2|-->LED
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
;******************************************************************************


 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x280,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD+WDTNMI+WDTNMIES,&WDTCTL  ; WDT off NMI hi/lo
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

            bis.b   #BIT2,&P1DIR            ; Set P1.2 to output direction
            bic.b   #BIT2,&P1OUT            ; Clear P1.2 LED off
            bis.b   #NMIIE,&IE1             ; Enable NMI

loop        bis.w   #LPM0,SR                ; Enter LPM0
            jmp     loop

;-------------------------------------------------------------------------------
NMI_ISR
;-------------------------------------------------------------------------------
            bis.b   #BIT2,&P1OUT            ; Set P1.2 LED on
            mov.w   #20000,R4               ; Initialize counter
delay       dec     R4
            jne     delay
            bic.b   #BIT2,P1OUT             ; Clear P1.2 LED off
            bic.w   #NMIIFG,&IFG1           ; Reclear NMI flag in case bounce
            bis.w   #NMIIE,&IE1             ; Enable NMI
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   NMI_VECTOR              ; NMI Vector
            .short  NMI_ISR
            .end
