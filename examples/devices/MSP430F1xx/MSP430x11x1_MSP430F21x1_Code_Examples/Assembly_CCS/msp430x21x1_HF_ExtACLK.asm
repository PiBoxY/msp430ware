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
;  MSP430x21x1 Demo - Basic Clock, MCLK Sourced from HF Digital source
;
;  Description: Proper selection of an external HF Digital Clk for MCLK is
;  shown by first polling the OSC fault until XTAL is stable - only then is
;  MCLK sourced by LFXT1.  MCLK/10 is on P1.1 driven by a software loop taking
;  exactly 10 CPU cycles.
;  ACLK = MCLK = LFXT1 = HF Digital Clk
;  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//
;
;               MSP430F21x1
;            -----------------
;        /|\|              XIN|<--HF Ext ACLK (0.4-16 MHz)
;         | |                 |
;         --|RST          XOUT|-
;           |                 |
;           |             P1.1|-->MCLK/10 = HFXTAL/10
;           |             P2.0|-->ACLK = HF Clk
;
;
;  H. Grewal / A. Dannenberg
;  Texas Instruments Inc.
;  June 2005
;  Built with Code Composer Essentials Version: 1.0
;*****************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupBC     bis.b   #XTS,&BCSCTL1           ; LFXT1 = HF XTAL
            bis.b   #LFXT1S_3,&BCSCTL3      ; Digital Clk
            bis.b   #001h,&P2DIR            ; P2.0 = output direction
            bis.b   #001h,&P2SEL            ; P2.0 = ACLK function
            bis.b   #002h,&P1DIR            ; P1.1 = output direction
SetupOsc    bic.b   #OFIFG,&IFG1            ; Clear OSC fault flag
            mov.w   #0FFh,R15               ; R15 = Delay
SetupOsc1   dec.w   R15                     ; Additional delay to ensure start
            jnz     SetupOsc1               ;
            bit.b   #OFIFG,&IFG1            ; OSC fault flag set?
            jnz     SetupOsc                ; OSC Fault, clear flag again
            bis.b   #SELM_3,&BCSCTL2        ; MCLK = LFXT1

                                            ;
Mainloop    bis.b   #002h,&P1OUT            ; P1.1 = 1
            bic.b   #002h,&P1OUT            ; P1.1 = 0
            jmp     Mainloop                ; Repeat
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect ".reset"                  ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
