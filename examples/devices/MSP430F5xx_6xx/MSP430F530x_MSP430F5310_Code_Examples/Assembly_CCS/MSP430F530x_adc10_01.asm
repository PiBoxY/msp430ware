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
;  MSP430F530x Demo - ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
;
;  Description: A single sample is made on A0 with reference to AVcc.
;  Software sets ADC10SC to start sample and conversion - ADC10SC
;  automatically cleared at EOC. ADC10 internal oscillator times sample (16x)
;  and conversion. In Mainloop MSP430 waits in LPM0 to save power until ADC10
;  conversion complete, ADC10_ISR will force exit from LPM0 in Mainloop on
;  reti. If A0 > 0.5*AVcc, P1.0 set, else reset.
;
;                MSP430F530x
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;     Vin -->|P6.0/A0      P1.0|-->LED
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
StopWDT     mov.w   #WDTPW | WDTHOLD,&WDTCTL; Stop WDT

SetupADC10  bis.w   #ADC10SHT_2 | ADC10ON,&ADC10CTL0 ; S&H=16 ADC clks, ADC10 on
            bis.w   #ADC10SHP,&ADC10CTL1    ; ADCCLK = MODOSC; sampling timer   
            bis.w   #ADC10RES,&ADC10CTL2    ; 10-bit conversion results
            bis.w   #ADC10INCH_0,&ADC10MCTL0; A0 ADC input select; Vref=AVcc
            bis.w   #ADC10IE0,&ADC10IE      ; Enable ADC conv complete interrupt

SetupP6     bis.b   #BIT0,&P6SEL            ; P6.0 ADC option select
SetupP1     bis.b   #BIT0,&P1DIR            ; P1.0 output
            
Mainloop    bis.w   #ADC10ENC|ADC10SC,&ADC10CTL0 ; Start sampling/conversion
            nop
            bis.w   #LPM0 | GIE,SR          ; LPM0, ADC10_ISR will force exit
            nop                             ; used for debugging
            jmp     Mainloop

;-------------------------------------------------------------------------------
ADC10_ISR
;-------------------------------------------------------------------------------
            add.w   &ADC10IV,PC             ; Add offset to PC
            reti                            ; Vector 0:  No interrupt
            reti                            ; Vector 2:  ADC overflow
            reti                            ; Vector 4:  ADC timing overflow
            reti                            ; Vector 6:  ADC10HI
            reti                            ; Vector 8:  ADC10LO
            reti                            ; Vector 10: ADC10IN
            jmp     ADC10IFG0_HND           ; Vector 12: ADC10IFG3

ADC10IFG0_HND
            cmp.w   #0x1FF,&ADC10MEM0       ; ADC10MEM = A0 >= 0.5AVcc? 
            jnc     less_than  
grtr_or_eql bis.b   #BIT0,&P1OUT            ; yes -> P1.0 = 1  
            jmp     exit_isr                
less_than   bic.b   #BIT0,&P1OUT            ; no -> P1.0 = 0
exit_isr    bic.w   #LPM0,0(SP)             ; Exit active CPU 
            reti                                    
;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ADC10_VECTOR            ; ADC10 ISR vector
            .short  ADC10_ISR
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
