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
;   MSP430P430 Demo - DMA0, Repeated Block To P5OUT, CCR2IFG Trigger
;
;   Description: DMA0 is used to transfer a string byte-by-byte as a repeating
;   block to P5OUT. Timer_A operates continuously with CCR2IFG
;   triggering DMA0. The effect is P5.1/5.2 toggling at different frequencies.
;   ACLK = n/a, MCLK = SMCLK = default DCO
;
;                MSP430FG439
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |             P5.1|--> LED
;            |             P5.2|-->
;
;   M. Buccini / M. Mitchell
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                  ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP1     bis.b   #006h,&P5DIR            ; P5.1/5.2 output
SetupDMA0   mov.w   #DMA0TSEL_1,&DMACTL0    ; CCR2 trigger
            mov.w   #testconst,&DMA0SA      ; Source block address
            mov.w   #P5OUT,&DMA0DA          ; Destination single address
            mov.w   #06h,&DMA0SZ            ; Block size
            mov.w   #DMADT_4+DMASRCINCR_3+DMASBDB+DMAEN,&DMA0CTL; Rpt, inc src
SetupTA     mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK/4, contmode
                                            ;
Mainloop    bis.w   #CPUOFF,SR              ;
            nop                             ; Required only for debugger
                                            ;
testconst   .byte  00h,06h,04h,06h,00h,02h
                                            ;
;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .end
