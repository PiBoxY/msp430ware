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
;   MSP430x47xx Demo - FLL+, MCLK Configured to Operate from XT2 HF XTAL
;
;   Description: Proper selection of an external HF XTAL for MCLK is
;   demonstrated using HF XT2 OSC.  OFIFG is polled until the HF XTAL
;   is stable - only then is MCLK sourced by XT2.  MCLK is buffered on P1.1.
;   //** HF XTAL NOT INSTALLED ON FET **//
;
;                MSP430x47xx
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |                 |
;            |            XT2IN|-
;            |                 | HF XTAL (455kHz - 8MHz)
;            |           XT2OUT|-
;            |                 |
;            |                 |
;            |        P1.1/MCLK|-->MCLK = HF XTAL
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
			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #900h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #OSCOFF+SCG0,SR         ; Disable LFXT1 xtal osc & FLL loop
SetupHF     bic.b   #XT2OFF,&FLL_CTL1       ; Clear bit = high freq xtal on
ClearFlag   bic.b   #OFIFG,&IFG1            ; Clear osc fault flag
            mov     #0F000h,R15             ; Move delay time to register 15
HF_Wait     dec     R15                     ; Delay for xtal to start, FLL lock
            jnz     HF_Wait		    ; Loop if delay not finished

            bit.b   #OFIFG,&IFG1            ; Test osc fault flag
            jnz     ClearFlag               ; If not loop again
                                            ;
            bis.b   #SELM1,&FLL_CTL1        ; MCLK = XT2
                                            ;
SetupP1     bis.b   #BIT1,&P1DIR            ; P1.1 output direction
            bis.b   #BIT1,&P1SEL            ; P1.1 option select

Mainloop    jmp     Mainloop                ;
                                            ;
;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect	".reset"            	; MSP430 RESET Vector
            .short  RESET                   ;
            .end
     
