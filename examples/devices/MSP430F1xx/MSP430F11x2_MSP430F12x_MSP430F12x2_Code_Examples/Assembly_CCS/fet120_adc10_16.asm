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
;   MSP-FET430P120 Demo - ADC10, DTC Sample A0 -> TA1, AVcc, DCO
;
;   Description: Use DTC to sample A0 with reference to AVcc and directly
;   transfer code to CCR1. Timer_A has been configured for 10-bit PWM mode.
;   CCR1 duty cycle is automatically proportional to ADC10 A0. WDT_ISR used
;   as a period wakeup timer approximately 30ms based on default ~ 800khz
;   DCO/SMCLK clock source used in this example for the WDT clock source.
;   Timer_A also uses default DCO.
;   //* MSP430F12x2 or MSP430F11x2 Device Required *//
;
;                MSP430F1232
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;        >---|A0           P1.2|--> CCR1 - 0-1024 PWM
;
;   M. Buccini / M. Raju
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0300h,SP               ; Initialize stackpointer
SetupWDT    mov.w   #WDT_MDLY_32,&WDTCTL    ; WDT ~30ms interval timer
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
SetupADC10  mov.w   #ADC10SHT_2+ADC10ON,&ADC10CTL0 ;
            bis.b   #01h,&ADC10AE           ; P2.0 ADC option select
            mov.b   #001h,&ADC10DTC1        ; 1 conversion
SetupP1     bis.b   #004h,&P1DIR            ; P1.2 = output
            bis.b   #004h,&P1SEL            ; P1.2 = TA1 output
SetupC0     mov.w   #1024-1,&CCR0           ; PWM Period
SetupC1     mov.w   #OUTMOD_7,&CCTL1        ; CCR1 reset/set
            mov.w   #512,&CCR1              ; CCR1 PWM Duty Cycle	
SetupTA     mov.w   #TASSEL_2+MC_1,&TACTL   ; SMCLK, upmode
                                            ;
Mainloop    bis.b   #CPUOFF+GIE,SR          ; LPM0, WDT_ISR will force exit
            mov.w   #CCR1,&ADC10SA          ; Data trasfer location
            bis.w   #ENC+ADC10SC,&ADC10CTL0 ; Start sampling
            jmp     Mainloop                ; Again
                                            ;
;------------------------------------------------------------------------------
WDT_ISR;    Exit all LPMx modes, reti returns system active with GIE enabled
;------------------------------------------------------------------------------
            mov.w   #GIE,0(SP)              ; System active on TOS
            reti                            ;		
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int10"                ; WDT Vector
            .short  WDT_ISR                 ;
            .end
