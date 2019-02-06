; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2014, Texas Instruments Incorporated
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
; MSP430FR231x Demo - eCOMP Toggle from LPM3; eCOMP in ultra low power mode,
;                     Vcompare is compared against internal VREF.
;
; Description: Use eCOMP and internal VREF to determine if input 'Vcompare'
;    is high or low.  When Vcompare exceeds 1.47V(VREF*63/64), CPOUT goes high
;     and when Vcompare is less than 1.47V(VREF*63/64) then CPOUT goes low.
;
;                MSP430FR2311
;             ------------------
;         /|\|                  |
;          | |                  |
;          --|RST        P1.1/C1|<--Vcompare
;            |                  |
;            |         P2.0/COUT|----> 'high'(Vcompare>1.47V); 'low'(Vcompare<1.47V)
;            |                  |
;
;   Darren Lu
;   Texas Instruments Inc.
;   July 2015
;   Built with Code Composer Studio v6.1
;******************************************************************************
            .cdecls C,LIST,"msp430.h"  ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

RESET       mov.w   #__STACK_END,SP         ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetPin      bis.b   #BIT1,&P1SEL0           ; Select eCOMP input function on P1.1/C1
            bis.b   #BIT1,&P1SEL1
            bis.b   #BIT0,&P2DIR
            bis.b   #BIT0,&P2SEL1           ; Select CPOUT function on P2.0/COUT
            bic.w   #LOCKLPM5,&PM5CTL0      ; Unlock I/O pins

SetREF      mov.b   #PMMPW_H,&PMMCTL0_H      ; Unlock the PMM registers
            bis.w   #INTREFEN,&PMMCTL2       ; Enable internal reference
PollREF     bit.w   #REFGENRDY,&PMMCTL2      ; Poll till internal reference settles
            jz      PollREF

SetCOMP     mov.w   #CPPSEL0+CPNSEL1+CPNSEL2+CPPEN+CPNEN,&CPCTL0
                                            ; Select C1 as input for V+ terminal
                                            ; Select DAC as input for V- terminal
                                            ; Enable eCOMP input
            bis.b   #CPDACREFS+CPDACEN,&CPDACCTL; Select on-chip VREF and enable DAC
            bis.b   #0x3f,&CPDACDATA        ; CPDACBUF1=On-chip VREF *63/64
            bis.w   #CPEN+CPMSEL,&CPCTL1    ; Turn on eCOMP, in low power mode

            nop
            bis.w   #LPM3,SR                ; Enter LPM3
            nop

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   RESET_VECTOR            ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
