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
;   MSP430F23x0 Demo - Comp_A+, Detect Threshold, Set P1.0 if P2.4 > 0.25*Vcc
;
;   Description: Use Comparator_A+ to detect a voltage threshold.
;   Using an external potentiometer, an unknown voltage is applied to P2.4.
;   Comparator_A+ compares the unknown voltage to an internal reference
;   voltage, in this example 0.25*VCC. If the unknown voltage is higher
;   than 0.25*VCC, P1.0 is set, if not, P1.0 is reset.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~1.2MHz
;
;              MSP430F23x0
;             ---------------
;        /|\ |            XIN|-
;         |  |               |
;         ---|RST        XOUT|-
;         |  |               |
;         R<-|P2.4/CA1   P1.0|-->LED
;         |  |               |
;         ---|VSS            |
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
RESET       mov.w   #450h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP1     bis.b   #001h,&P1DIR            ; P1.0 = output
            mov.b   #CARSEL+CAREF0+CAON,&CACTL1  ; 0.25*Vcc = -Comp, on
            mov.b   #P2CA4,&CACTL2          ; P2.4/CA1 = +Comp
                                            ;
Mainloop    bit.b   #CAOUT,&CACTL2          ; Test Comparator_A+ output
            jz      OFF                     ; jmp--> if CAOUT reset
                                            ;
ON          bis.b   #001h,&P1OUT            ; P1.0 = 1
            jmp     Mainloop                ;
OFF         bic.b   #001h,&P1OUT            ; P1.0 = 0
            jmp     Mainloop                ;
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".reset"            ; MSP430 RESET Vector
            .short	RESET                   ;
            .end
