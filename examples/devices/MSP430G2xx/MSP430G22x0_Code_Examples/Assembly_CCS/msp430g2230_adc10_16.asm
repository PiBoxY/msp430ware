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
;  MSP430G2230 Demo - ADC10, DTC Sample A5 -> TA1, AVcc, DCO
;
;  Description: Use DTC to sample A5 with reference to AVcc and directly
;  transfer code to TA0CCR1. Timer_A has been configured for 10-bit PWM mode.
;  TA0CCR1 duty cycle is automatically proportional to ADC10 A5. WDT_ISR used
;  as a period wakeup timer approximately 45ms based on default ~1.2MHz
;  DCO/SMCLK clock source used in this example for the WDT clock source.
;  Timer_A also uses default DCO.
;
;                MSP430G2230
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;        >---|P1.5/A5      P1.2|--> TA0CCR1 - 0-1024 PWM
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
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
RESET       mov.w   #0x280,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTTMSEL,&WDTCTL ; WDT ~45ms interval timer.
            bis.b   #WDTIE,&IE1             ;  Enable WDT interrupt
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

            mov.w   #ADC10SHT_2+ADC10ON,&ADC10CTL0 ; Sample every 16 ADC clks
            bis.w   #INCH_5,&ADC10CTL1      ; Input A5
            bis.b   #BIT5,&ADC10AE0         ; P1.5 ADC option select
            mov.b   #0x1,&ADC10DTC1         ; 1 conversion
            bic.b   #BIT2,&P1OUT            ; Initialize LED as off
            bis.b   #BIT2,&P1DIR            ; Set P1.2 to output direction
            bis.b   #BIT2,&P1SEL            ; P1.2 = TA1 output
            mov.w   #1024-1,&TA0CCR0        ; PWM Period
            mov.w   #OUTMOD_7,&TA0CCTL1     ; TA0CCR1 output mode reset/set
            mov.w   #512,&TA0CCR1           ; TA0CCR1 PWM Duty Cycle
            mov.w   #TASSEL_2+MC_1,&TA0CTL  ; SMCLK, up mode

loop        bis.w   #LPM0+GIE,SR            ; LPM0 (CPU off), ISR forces exit
            mov.w   #TA0CCR1,&ADC10SA
            bis.w   #ENC+ADC10SC,ADC10CTL0  ; Start sampling
            jmp     loop

;-------------------------------------------------------------------------------
WDT_ISR
;-------------------------------------------------------------------------------
            bic.w   #LPM0,0(SP)             ; Exit active CPU
            reti                            ; Vector 0:  No interrupt

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   WDT_VECTOR              ; Watchdog Vector
            .short  WDT_ISR
            .end
