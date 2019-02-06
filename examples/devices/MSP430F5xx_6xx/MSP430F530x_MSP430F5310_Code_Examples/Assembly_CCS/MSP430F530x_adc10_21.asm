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
;  MSP430F530x Demo - ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO
;
;  Description; A1 is sampled in every 0.5s using the TA0.0 trigger in single 
;  ch/conversion mode. Window comparator is used to generate interrupts to
;  indicate when the input voltage goes above the High_Threshold or below the 
;  Low_Threshold or is inbetween the high and low thresholds. TimerB0 is used 
;  as an interval timer used to control the LED at P1.0 to toggle slow/fast 
;  or turn off according to the ADC10 Hi/Lo/IN interupts.
;  ADC10 is configured in pulse sample mode and is triggered by the TA0.0
;
;               MSP430F530x
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
;         --|RST          XOUT|-
;           |                 |
;       >---|P6.1/A1     P1.0 |--> LED 
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

High_Threshold       .equ     818
Low_Threshold        .equ     409
SlowToggle_Period    .equ     20000-1
FastToggle_Period    .equ     1000-1

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

; Configure ADC10
SetupADC10  mov.w   #ADC10SHT_2 | ADC10ON,&ADC10CTL0 ; ADC10ON
            mov.w   #ADC10SHP | ADC10SHS_1 | ADC10CONSEQ_2,&ADC10CTL1
                                            ; rpt single ch; TA0.1 trigger            
            mov.w   #ADC10RES,&ADC10CTL2    ; 10-bit conversion results
            mov.w   #ADC10INCH_1 | ADC10SREF_1,&ADC10MCTL0 ; Vref+, A1
            mov.w   #High_Threshold,&ADC10HI; Window Comparator Hi-threshold
            mov.w   #Low_Threshold,&ADC10LO ; Window Comparator Lo-threshold
            
; Configure internal reference
while_loop  and.w   #REFGENBUSY,&REFCTL0    ; If ref generator busy, WAIT       
            jnz     while_loop              ; 
            bis.w   #REFVSEL_2 | REFON,&REFCTL0
                                            ; Internal Reference ON; Vref=2.5V            
SetupP1     bis.b   #BIT0,&P1DIR            ; Set P1.0 output direction
            bic.b   #BIT0,&P1OUT            ; Clear P1.0
            
; Configure TB0 period-timer
            mov.w   #CCIE,&TB0CCTL0         ; CCR0 interrupt enabled
            mov.w   #TBSSEL_1 | TBCLR,&TB0CTL ; ACLK, clear TAR

; Configure ADC10 timer trigger TA0.1          
            mov.w   #16348-1,&TA0CCR0       ; PWM Period 
            mov.w   #8192-1,&TA0CCR1        ; TA0.1 PWM
            mov.w   #OUTMOD_3,&TA0CCTL1     ; TACCR1 set/reset; enable interrupt
            mov.w   #TASSEL_1 | MC_1| TACLR,&TA0CTL ; ACLK, up mode
            
            bis.w   #ADC10HIIE | ADC10LOIE | ADC10INIE,&ADC10IE; Interrupt
                                                               ; enable
            bis.w   #ADC10ENC,&ADC10CTL0    ; Enable conversion
            nop
            bis.w   #LPM3 | GIE,SR          ; Enter LPM3 w/ interrupts
            nop
;-------------------------------------------------------------------------------
ADC10_ISR;
;-------------------------------------------------------------------------------
            add.w   &ADC10IV,PC             ; Add offset to PC
            reti                            ; Vector 0:  No interrupt
            reti                            ; Vector 2:  ADC overflow
            reti                            ; Vector 4:  ADC timing overflow
            jmp     ADC10HI_HND             ; Vector 6:  ADC10HI
            jmp     ADC10LO_HND             ; Vector 8:  ADC10LO
            jmp     ADC10IN_HND             ; Vector 10: ADC10IN               
            reti                            ; Vector 12: ADC10IFG3

ADC10HI_HND
            bic.w   #ADC10HIIFG,&ADC10IFG   ; Clear interrupt flag 
            bic.w   #MC_1,&TB0CTL           ; Turn off Timer
            mov.w   #FastToggle_Period,&TB0CCR0
                                            ; Set Timer Period for fast LED
                                            ; toggle
            bis.w   #MC_1,&TB0CTL           ; 
            reti
            
ADC10LO_HND 
            bic.w   #ADC10LOIFG,&ADC10IFG   ; Clear interrupt flag  
            bic.w   #MC_1,&TB0CTL           ; Turn off Timer
            bic.b   #BIT0,&P1OUT      
            reti

ADC10IN_HND            
            bic.w   #ADC10INIFG,&ADC10IFG   ; Clear interrupt flag 
            bic.w   #MC_1,&TB0CTL           ; Turn off Timer
            mov.w   #SlowToggle_Period,&TB0CCR0 
                                            ; Set Timer Period for slow LED
                                            ; toggle
            bis.w   #MC_1,&TB0CTL           ; 
            reti            
;-------------------------------------------------------------------------------
TB0_ISR
;-------------------------------------------------------------------------------
            xor.b   #BIT0,&P1OUT            ; Toggle LED P1.0
            reti 
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ADC10_VECTOR            ; ADC10 Vector
            .short  ADC10_ISR
            .sect   TIMER0_B0_VECTOR        ; Timer0_B0 Vector
            .short  TB0_ISR
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
