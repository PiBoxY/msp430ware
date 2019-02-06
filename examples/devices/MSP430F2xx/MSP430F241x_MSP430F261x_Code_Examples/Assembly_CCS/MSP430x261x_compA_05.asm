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
;   MSP430x261x Demo - Comparator A, Input to CA0, Interrupt triggered
;
;   Description:  Normal operation is in LPM3.  When Vcompare crosses the ref
;   voltage, an interrupt is generated, and the ISR toggles the LED.
;   //* External watch crystal on XIN XOUT is required for ACLK *//
;
;             MSP430F261x/241x
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |         P2.3/CA0|<--Vcompare
;            |                 |
;            |             P1.0|-->I/O - LED
;
;  JL Bile
;  Texas Instruments Inc.
;  June 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
	.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0850h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

            mov.b   #CAON+CAREF_2+CARSEL, &CACTL1  ; Enable comp, ref = 0.5*Vcc
            mov.b   #P2CA0, &CACTL2         ; Pin to CA0

SetupP1     bis.b   #BIT0,&P1DIR            ; P1.0 = output direction
            bic.b   #BIT0,&P1OUT
            
SetupCAIRQ  bis.b   #CAIE,&CACTL1           ; Setup interrupt for Comparator

mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3, enable interrupts

;------------------------------------------------------------------------------
COMPA_ISR   ;     COMPARATOR A ISR - toggles LED
;------------------------------------------------------------------------------
            xor.b   #CAIES,&CACTL1          ; Toggles interrupt edge
            xor.b   #BIT0,&P1OUT            ; Toggle P1.0
            reti                            ;		

;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect	".reset"            ; POR, ext. Reset
            .short   RESET                   ;
            .sect	".int27"      ; Comparator A
            .short	COMPA_ISR
            .end
            
