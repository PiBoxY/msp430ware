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
;  MSP430F5438 Demo - Timer_A3, Toggle P1.0;P2.1-3, Cont. Mode ISR, 32kHz ACLK
;
;  Description: Use Timer1_A CCRx units and overflow to generate four
;  independent timing intervals. For demonstration, CCR0, CCR1 and CCR2
;  output units are optionally selected with port pins P2.1, P2.2 and P2.3
;  in toggle mode. As such, these pins will toggle when respective CCRx
;  registers match the TAR counter. Interrupts are also enabled with all
;  CCRx units, software loads offset to next interval only - as long as
;  the interval offset is added to CCRx, toggle rate is generated in
;  hardware. Timer1_A overflow ISR is used to toggle P1.0 with software.
;  Proper use of the TAIV interrupt vector generator is demonstrated.
;  ACLK = TACLK = 32kHz, MCLK = SMCLK = default DCO ~1.045MHz
;
;  As coded and with TACLK = 32768Hz, toggle rates are:
;  P2.1= CCR0 = 32768/(2*4) = 4096Hz
;  P2.2= CCR1 = 32768/(2*16) = 1024Hz
;  P2.3= CCR2 = 32768/(2*100) = 163.84Hz
;  P1.0= overflow = 32768/(2*65536) = 0.25Hz
;
;               MSP430F5438
;            -------------------
;        /|\|                   |
;         | |                   |
;         --|RST                |
;           |                   |
;           |         P2.1/TA1.0|--> CCR0
;           |         P2.2/TA1.1|--> CCR1
;           |         P2.3/TA1.2|--> CCR2
;           |               P1.0|--> Overflow/software
;           |               P1.0|--> LED
;
;   M. Morales
;   Texas Instruments Inc.
;   September 2008
;   Built with Code Composer Essentials v3.x
;******************************************************************************

    .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .global _main 
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x5C00,SP              ; Initialize stackpointer
            mov.w   #WDTPW + WDTHOLD,&WDTCTL; Stop WDT    
            bis.b   #BIT1 + BIT2 + BIT3,&P2SEL 
                                            ; P1.1 - P1.3 option select
            bis.b   #BIT1 + BIT2 + BIT3,&P2DIR 
                                            ; P2.1 - P2.3 outputs
            bis.b   #BIT0,&P1DIR            ; P1.0 - Outputs
            mov.w   #OUTMOD_4 + CCIE,&TA1CCTL0 ; CCR0 toggle, interrupt enabled                                            
            mov.w   #OUTMOD_4 + CCIE,&TA1CCTL1 ; CCR1 toggle, interrupt enabled
            mov.w   #OUTMOD_4 + CCIE,&TA1CCTL2 ; CCR2 toggle, interrupt enabled
            mov.w   #TASSEL_1 + MC_2 + TACLR + TAIE,&TA1CTL
                                            ; ACLK, contmode, clear TAR,
                                            ; interrupt enabled
            bis.w   #LPM3 + GIE,SR          ; Enter LPM3, enable interrupts 
            nop                             ; For debugger
            
;-------------------------------------------------------------------------------
TIMER1_A0_ISR                               ; Timer1 A0 interrupt service routine
;-------------------------------------------------------------------------------           
            add.w   #0x04,&TA1CCR0          ; Add Offset to CCR0
            nop                             ; Set breakpoint here 
            reti                            ; Return from interrupt 

;-------------------------------------------------------------------------------
TIMER1_A1_ISR                         ; Timer_A3 Interrupt Vector (TAIV) handler
;-------------------------------------------------------------------------------
            add.w   &TA1IV,PC               ; Add offset to jump handler
            reti                            ; Vector 0:  No interrupt 
            jmp     TA1CCR1_HND             ; Vector 2:  TA1CCR1 
            jmp     TA1CCR2_HND             ; Vector 4:  TA1CCR2
            reti                            ; Vector 6:  Reserved
            reti                            ; Vector 8:  Reserved
            reti                            ; Vector 10: Reserved
            reti                            ; Vector 12: Reserved 

TA1IFG_HND  xor.b  #BIT0,&P1OUT             ; Vector 14: TAIG, Toggle LED            
            reti                            ; Return from interrupt 

TA1CCR1_HND add.w  #16,&TA1CCR1             ; Add Offset to TA1CCR1
            reti

TA1CCR2_HND add.w  #100,&TA1CCR2            ; Add Offset to TA1CCR2
            reti

;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int49"            
            .short  TIMER1_A0_ISR
            .sect   ".int48"            
            .short  TIMER1_A1_ISR
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end

