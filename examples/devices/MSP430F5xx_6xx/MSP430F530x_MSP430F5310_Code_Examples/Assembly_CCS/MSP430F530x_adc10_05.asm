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
;  MSP430F530x Demo - ADC10, Sample A11, Lo_Batt, Clear P1.0 if AVcc < 2.3V
;
;  Description: A single sample is made on A11 (AVcc/2) with reference to
;  internal 1.5V Vref. Software sets ADC10SC to start sample and conversion
;  - ADC10SC automatically cleared at EOC. ADC10 internal oscillator times
;  sample (16x) and conversion. ADC10BUSY flag is polled for EOC. If A11
;  (AVcc/2) < 0311h (1.15V) indicating AVcc is less 2.3V, P1.0 reset indicating
;  a lo_Batt condition, else set.
;  ACLK = n/a, MCLK = SMCLK = default DCO ~1.2MHz, ADC10CLK = ADC10OSC
;
;                MSP430F530x
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |A11 (AVcc/2) P1.0|-->LED
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

; Confirgure ADC10 - Pulse sa,ple mode; ADC10SC trigger
SetupADC10  mov.w   #ADC10SHT_2 | ADC10ON,&ADC10CTL0 ; 16 ADC10CLKs; ADC ON
            mov.w   #ADC10SHP | ADC10CONSEQ_0,&ADC10CTL1
                                            ; s/w trig, single ch/conv 
            mov.w   #ADC10RES,&ADC10CTL2    ; 10-bit conversion results
            mov.w   #ADC10SREF_1 | ADC10INCH_11,&ADC10MCTL0 ; AVcc/2

while_loop  and.w   #REFGENBUSY,&REFCTL0    ; If ref generator busy, WAIT       
            jnz     while_loop              ;
            bis.w   #REFVSEL_0 | REFON,&REFCTL0 ; Select Internal ref = 1.5V.
                                                ; Internal reference on

; Configure TA0 to provide delay for reference settling ~75us            
            mov.w   #80,&TA0CCR0            ; Delay to allow Ref to settle
            bis.w   #CCIE,&TA0CCTL0         ; Compare-mode interrupt.
            mov.w   #MC_1 | TASSEL_2,&TA0CTL; up mode, TACLK=SMCLK
            nop
            bis.w   #LPM0 | GIE,SR          ; Enter LPM0, TA0_ISR will force
            nop                             ; exit
            bic.w   #CCIE,&TA0CCTL0         ; Disable timer interrupt

SetupP1     bis.b   #BIT0,&P1DIR            ; P1.0 output
                                            
Mainloop    bis.w   #ADC10ENC | ADC10SC,&ADC10CTL0
                                            ; Sampling and conversion start
L1          bit.w   #ADC10BUSY,&ADC10CTL1   ; ADC10BUSY?
            jnz     L1                      ;
            cmp.w   #0311h,&ADC10MEM0       ; ADC10MEM0 = A11 (AVcc/2) > 1.15V? 
            jnc     lo_Batt                 ;
            bis.b   #BIT0,&P1OUT            ; Set P1.0 LED on
            jmp     Mainloop                ; Again
lo_Batt     bic.b   #BIT0,&P1OUT            ; Clear P1.0 LED off
            jmp     Mainloop                ; Again
                                            ;
;-------------------------------------------------------------------------------
TA0_ISR;    ISR for TACCR0
;-------------------------------------------------------------------------------
            clr.w   &TA0CTL                 ; Clear Timer_A control registers
            bic.w   #LPM0,0(SP)             ; Exit LPM0 on reti
            reti                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   "int53"                 ; Timer_A0 Vector
            .short  TA0_ISR                 ;
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
