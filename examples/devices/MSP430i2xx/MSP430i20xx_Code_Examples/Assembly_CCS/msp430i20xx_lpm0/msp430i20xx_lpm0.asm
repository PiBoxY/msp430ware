; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2013, Texas Instruments Incorporated
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
;  MSP430i20xx Demo - Enters LPM0 with ACLK = 32kHz, SMCLK = MCLK = 1MHz
;
;  Description: This program operates MSP430 normally in LPM0, pulsing P1.4
;   ~ 1 second intervals. WDT ISR used to blink LED. To measure LPM0 current
;    on target board, remove JP1 and JP2 and measure current across JP1.
;
;  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO/16 = 1.024MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;               MSP430i20xx
;             -----------------
;         /|\|                |
;          | |                |
;          --|RST             |
;            |                |
;            |            P1.4|-->LED
;
;   T. Witt
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio v5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

            .ref    init                    ; Reference external function

RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
Cal         call    #init                   ; Calibrate peripherals
StopWDT     mov.w   #WDT_ADLY_1000,&WDTCTL  ; WDT interval timer, ACLK
            bis.b   #WDTIE,IE1              ; Enable WDT interrupt
            mov.b   #0xFF,&P1OUT            ; Configure GPIO
            mov.b   #0xFF,&P1DIR            ; All output, all low
            mov.b   #0xFF,&P2OUT
            mov.b   #0xFF,&P2DIR
            bis.b   #DIVM__16|DIVS__16,&CSCTL1 ; MCLK = DCO/16, SMCLK = DCO/16
            bis.w   #LPM0|GIE,SR            ; Enter LPM0 with Interrupts
Mainloop    nop
            jmp     Mainloop                ; Again

;-------------------------------------------------------------------------------
WDT_ISR;    ISR for WDT
;-------------------------------------------------------------------------------
            xor.b   #BIT4,&P1OUT            ; Toggle P1.4
            reti                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int10"                ; WDT Interrupt
            .short  WDT_ISR
            .end
