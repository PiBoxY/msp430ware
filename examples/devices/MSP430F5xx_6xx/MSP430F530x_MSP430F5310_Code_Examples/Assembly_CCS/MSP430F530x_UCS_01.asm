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
;  MSP430F530x Demo - Software Toggle P1.1 at Default DCO
;
;  Description: Toggle P1.0 by xor'ing P1.1 inside of a software loop.
;  ACLK is brought out on pin P1.0, SMCLK is brought out on P2.2, and MCLK
;  is brought out on pin P4.7.
;  ACLK = REFO = 32.768kHz, MCLK = SMCLK = Default 1MHz
;
;                 MSP430F530x
;             -----------------
;         /|\|                 |
;          | |             P1.0|-->ACLK
;          --|RST          P4.7|-->MCLK
;            |             P2.2|-->SMCLK
;            |                 |
;            |             P1.1|-->Port Pin
;
;  K. Chen
;  Texas Instruments Inc.
;  March 2012
;  Built with CCS Version: 5.2
;*******************************************************************************

 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
;-------------------------------------------------------------------------------
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
            mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            
            mov.w   #0x02D52,&PMAPPWD       ; Enable Write-access to modify port
                                            ; mapping registers
            mov.b   #PM_MCLK,&P4MAP7        ; 
            mov.w   #0,&PMAPPWD             ; Disable Write-Access to modify 
                                            ; port mapping registers
                                            
            bis.b   #BIT1,&P1DIR            ; P1.1 output
            
            bis.b   #BIT0,&P1DIR            ; ACLK set out to pins
            bis.b   #BIT0,&P1SEL            ;
            bis.b   #BIT2,&P2DIR            ; SMCLK set out to pins
            bis.b   #BIT2,&P2SEL            ;
            bis.b   #BIT7,&P4DIR            ; MCLK set out to pins
            bis.b   #BIT7,&P4SEL            ;            

while_loop  xor.b   #BIT1,&P1OUT            ; Flash the LED
            mov.w   #60000,R4               ; Initialize loop counter
                                            ; R4 = 60,000
delay_loop  dec.w   R4                      ; Decrement loop counter
            jne     delay_loop              ; Loop if loop counter > 0        
            jmp     while_loop              ; Infinite while loop
            nop                             ; To workaround CPU40
            
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end