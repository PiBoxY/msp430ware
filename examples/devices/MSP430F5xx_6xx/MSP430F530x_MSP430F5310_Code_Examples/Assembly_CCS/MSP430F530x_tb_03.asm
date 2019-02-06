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
;  MSP430F530x Demo - Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
;
;  Description: Toggle P1.0 using software and Timer_B overflow ISR.
;  In this example an ISR triggers when TB overflows.  TB overflow has the
;  lowest priority. Inside the TB overflow ISR P1.0 is toggled. Toggle rate is
;  approximatlely 7.822Hz = [1.045MHz/FFFFh]/2. Proper use of TBIV interrupt vector
;  generator is shown.
;  ACLK = n/a, MCLK = SMCLK = TBCLK = default DCO ~1.045MHz.
;
;           MSP430F530x
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |           P1.0|-->LED
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
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT

SetupP1     bis.b   #BIT0,&P1DIR            ; P1.0 output

; Configure TimerB
SetupTB     mov.w   #TBSSEL_2 | MC_2 | TBCLR | TBIE,&TBCTL
                                            ; SMCLK, contmode, clear TBR
                                            ; enable interrupt
            nop
            bis.w   #LPM0|GIE,SR            ; Enter LPM0, interrupts enabled
            nop                             ; Required only for debugger

;-------------------------------------------------------------------------------
TB_ISR;
;-------------------------------------------------------------------------------
            add.w   &TBIV,PC                ; Add Timer_B offset vector
            reti                            ; Vector 0 - no interrupt
            reti                            ; Vector 1 - CCR1 not used
            reti                            ; Vector 2 - CCR2 not used
            reti                            ; Vector 3 - CCR3 not used
            reti                            ; Vector 4 - CCR4 not used
            reti                            ; Vector 5 - CCR5 not used
            reti                            ; Vector 6 - CCR6 not used
TB_over     xor.b   #BIT0,&P1OUT            ; Vector 7 - overflow (Toggle P1.0)
            reti                            ; Return from overflow ISR

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   TIMER0_B1_VECTOR        ; TIMER0_B1_VECTOR
            .short  TB_ISR                  ;
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
