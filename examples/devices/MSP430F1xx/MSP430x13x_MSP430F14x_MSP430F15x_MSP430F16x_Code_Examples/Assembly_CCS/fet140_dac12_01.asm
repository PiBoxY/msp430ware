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
;   MSP-FET430P140 Demo - DAC12_0, Output 1.0V on DAC0
;
;   Description: Using DAC12_0 and 2.5V ADC12REF reference with a gain of 1,
;   ouput 1V on P6.6.
;   ;* MSP430F169 Device Required *//
;
;                MSP430F169
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |        DAC0/P6.6|--> 1V
;            |                 |
;
;   M. Buccini / L. Westlund
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
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupADC12  mov.w   #REF2_5V+REFON,&ADC12CTL0 ; Internal 2.5V ref on
            mov.w   #13600,&TACCR0          ; Delay to allow Ref to settle
            bis.w   #CCIE,&TACCTL0          ; Compare-mode interrupt.
            mov.w   #TACLR+MC_1+TASSEL_2,&TACTL; up mode, SMCLK
            bis.w   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
            bic.w   #CCIE,&TACCTL0          ; Disable timer interrupt
            dint                            ; Disable Interrupts
SetupDAC120 mov.w   #DAC12IR+DAC12AMP_5+DAC12ENC,&DAC12_0CTL  ; Int ref gain 1
            mov.w   #0666h,&DAC12_0DAT      ; 1.0V
                                            ;
Mainloop    bis.w   #CPUOFF,SR              ; Enter LPM0
            nop                             ; Required only for debugger
                                            ;
;------------------------------------------------------------------------------
TAX_ISR;    Common ISR for CCR0-4 and overflow
;------------------------------------------------------------------------------
            add.w   &TAIV,PC                ; Add TA interrupt offset to PC
            jmp     TACCR0_ISR              ; CCR0
            reti                            ; CCR1
            reti                            ; CCR2
            reti                            ; CCR3
            reti                            ; CCR4
TA_over     reti                            ; Timer_A overflow

TACCR0_ISR  clr     &TACTL                  ; clear Timer_A control registers
            bic     #LPM0,0(SP)             ; Exit LPMx, interrupts enabled
            reti                            ;
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
;           Interrupt Vectors Used MSP430x15x/16x
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int06"                ; Timer_A0 Vector
            .short  TAX_ISR                 ;
            .end
