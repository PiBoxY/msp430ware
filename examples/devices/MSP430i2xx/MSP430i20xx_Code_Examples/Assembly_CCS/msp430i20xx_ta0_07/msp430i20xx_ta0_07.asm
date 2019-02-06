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
;  MSP430i20xx Demo - Timer_A0, Toggle P1.4, CCR1 Cont. Mode ISR, DCO SMCLK/8
;
;  Description: Use Timer_A CCRx units and overflow to generate four
;  independent timing intervals. For demonstration, CCR1 and CCR2 output
;  units are optionally selected with port pins P1.5 and P1.6 in toggle
;  mode. As such, these pins will toggle when respective CCRx registers match
;  the TAR counter. Interrupts are also enabled with all CCRx units,
;  software loads offset to next interval only - as long as the interval offset
;  is added to CCRx, toggle rate is generated in hardware. Timer_A overflow ISR
;  is used to toggle P1.4 with software.
;
;  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;           MSP430i20xx
;         ----------------
;     /|\|                |
;      | |                |
;      --|RST   P1.6/TA0.2|--> CCR2
;        |      P1.5/TA0.1|--> CCR1
;        |            P1.4|--> LED (Overflow/Software)
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
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #BIT5|BIT6,&P1SEL1      ; P1.5, P1.6 CCRx Function
            bis.b   #BIT4|BIT5|BIT6,&P1DIR  ; P1.4-6 output
            mov.w   #OUTMOD_4|CCIE,&TA0CCTL1; CCR1 Toggle, Interrupt
            mov.w   #OUTMOD_4|CCIE,&TA0CCTL2; CCR2 Toggle, Interrupt
            mov.w   #TASSEL_2|MC_2|ID_3|TAIE,&TA0CTL
            ; SMCLK/8, Cont. Mode, Interrupt Enabled

Mainloop    bis.w   #LPM0|GIE,SR            ; Enter LPM0 w/ interrupts
            nop                             ; For debugger
                                            ;
;-------------------------------------------------------------------------------
TA0_ISR        ; Timer A0 General ISR
;-------------------------------------------------------------------------------
            add.w   &TA0IV,PC
            reti                            ; No Interrupt
            jmp     CCR1_                   ; CCR1 IFG
            jmp     CCR2_                   ; CCR2 IFG
            reti                            ; CCR3 IFG
            reti                            ; CCR4 IFG
            reti                            ; CCR5 IFG
            reti                            ; CCR6 IFG
Overflow    xor.b   #BIT4,&P1OUT            ; Overflow IFG, Toggle LED
            reti
CCR1_       add.w   #2000,&TA0CCR1          ; Add offset to CCR1
            reti
CCR2_       add.w   #5000,&TA0CCR2          ; Add offset to CCR2
            reti
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int05"                ; TimerA0 General Interrupt
            .short  TA0_ISR
            .end
