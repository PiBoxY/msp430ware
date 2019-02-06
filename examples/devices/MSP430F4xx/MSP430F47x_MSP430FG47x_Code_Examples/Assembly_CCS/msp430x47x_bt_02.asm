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
;   MSP430x479 Demo - Basic Timer, Toggle P4.6 Inside ISR, 32kHz ACLK
;
;   Description: This program toggles P4.6 by xor'ing P4.6 inside of
;   a basic timer ISR. ACLK provides basic timer clock source.
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   //* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;                MSP430x479
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |             P4.6|-->LED
;
;   P.Thanigai
;   Texas Instruments Inc.
;   September 2008
;   Built with Code Composer Essentials Version: 3.0
;******************************************************************************
 .cdecls C,LIST, "msp430.h" 
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
SetupBT     mov.b   #BTDIV+BT_fCLK2_DIV16,&BTCTL   ; ACLK/(256*16)
            bis.b   #BTIE,&IE2              ; Enable Basic Timer interrupt
SetupP4     bis.b   #040h,&P4DIR            ; P4.6 output
                                            ;						
Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3, enable interrupts
            nop                             ; Required only for debugger
                                            ;
;------------------------------------------------------------------------------
BT_ISR;     Toggle P4.6
;------------------------------------------------------------------------------
            xor.b   #040h,&P4OUT            ; Toggle P4.6
            reti                            ;		
                                            ;
;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect	".reset"            ; RESET Vector
            .short  RESET               ;
            .sect   ".int00"	        ; MSP430 Basic Timer Interrupt Vector
            .short  BT_ISR         
            .end
