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
;  MSP430FR231x Demo - ADC, Sample A2/A1/A0, internal 1.5V Ref.
;
;  Description: This example works on Sequence-of-Channels Mode.
;  A2/A1/A0 is sampled 16ADCclks with reference to 1.5V.
;  Internal oscillator times sample (16x) and conversion(13x).
;  Inside ADC_ISR A2/A1/A0 sample value put into array ADC_Result[3].
;  ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
;  
;
;                MSP430FR2311
;             -----------------
;         /|\|                 |
;          | |                 |
;          --|RST              |
;            |                 |
;        >---|P1.2/A2          |
;        >---|P1.1/A1          |
;        >---|P1.0/A0          |
;
;
;   Darren Lu
;   Texas Instruments Inc.
;   July 2015
;   Built with Code Composer Studio v6.1
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                                  ; Make stack linker segment ?known?

            .global _main
            .text                                           ; Assemble to Flash memory
            .retain                                         ; Ensure current section gets linked
            .retainrefs
_main

RESET       mov.w   #__STACK_END,SP                         ; Initialize stack pointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL                  ; Stop WDT

SetupP1     bis.b   #BIT0|BIT1|BIT2,&P1SEL0                 ; ADC A0~2 pin
            bis.b   #BIT0|BIT1|BIT2,&P1SEL1
            bic.w   #LOCKLPM5,PM5CTL0                       ; Unlock I/O pins

SetupADC    bis.w   #ADCSHT_2|ADCMSC|ADCON,&ADCCTL0         ; ADC on
            bis.w   #ADCSHP|ADCCONSEQ_1|ADCSSEL_1,&ADCCTL1  ; ADC clock ACLK, sampling timer, s/w trig.,single sequence
            bic.w   #ADCRES,&ADCCTL2                        ; 8-bit conversion results
            bis.w   #ADCINCH_2|ADCSREF_1,&ADCMCTL0          ; A0~2(EoS), Vref=1.5V
            bis.w   #ADCIE0,&ADCIE                          ; Enable ADC conv

SetupREF    mov.b   #PMMPW_H,&PMMCTL0_H                     ; Unlock the PMM registers
            bis.w   #INTREFEN,&PMMCTL2                      ; Enable internal reference
            mov.w   #200,R15                                ; Delay ~400 cycles for reference settling
L1          dec.w   R15                                     ; Decrement R15
            jnz     L1                                      ; Delay over?

Mainloop    mov.w   #2,R8
ADCBusy     mov.w   &ADCCTL1,R15
            and.w   #ADCBUSY,R15                            ; Wait if ADC core is active
            tst.w   R15
            jnz     ADCBusy
            bis.w   #ADCENC|ADCSC,&ADCCTL0                  ; Sampling and conversion start
            nop
            bis.w   #CPUOFF|GIE,SR                          ; Enter LPM0 with interrupt
            nop
            mov.w   #2500,R15
L2          dec.w   R15
            jnz     L2
            nop
            jmp     Mainloop

;-------------------------------------------------------------------------------
ADC_ISR;  ADC interrupt service routine
;-------------------------------------------------------------------------------
            add.w   &ADCIV,PC                               ; add offset to PC
            reti                                            ; No Interrupt
            reti                                            ; Conversion result overflow
            reti                                            ; Conversion time overflow
            reti                                            ; ADHI
            reti                                            ; ADLO
            reti                                            ; ADIN
            jmp     ADMEM
            reti
ADMEM       cmp.w   #1,R8
            jeq     Result_1
            jl      Result_2
            jmp     Result_0
Result_0    mov.w   &ADCMEM0,R5
            jmp     L3
Result_1    mov.w   &ADCMEM0,R6
            jmp     L3
Result_2    mov.w   &ADCMEM0,R7
            jmp     L3
L3          tst.w   R8
            jnz     Continue
            bic.w   #CPUOFF,0(SP)                           ; Exit LPM0 on reti
            reti
Continue    sub.w   #1,R8
            reti

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
           .sect   RESET_VECTOR                             ; MSP430 RESET Vector
           .short  RESET                           
           .sect   ADC_VECTOR                               ; ADC Vector
           .short  ADC_ISR                           
           .end
