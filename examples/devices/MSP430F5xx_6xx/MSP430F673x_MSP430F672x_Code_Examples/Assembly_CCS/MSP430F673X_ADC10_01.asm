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
;  MSP430F673x Demo - ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
;
;  Description: A single sample is made on A0 with reference to AVcc.
;  Software sets ADC10SC to start sample and conversion - ADC10SC
;  automatically cleared at EOC. ADC10 internal oscillator times sample (16x)
;  and conversion. In main loop MSP430 waits in LPM0 to save power until ADC10
;  conversion complete, ADC10_ISR will force exit from LPM0 in main loop on
;  the return from the interrupt. If A0 > 0.5*AVcc, P1.0 set, else reset.
;
;                MSP430F673x
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;        >---|P1.2/A0      P1.0|-->LED
;
;  M. Swanson
;  Texas Instruments Inc.
;  December 2011
;  Built with CCS Version: 5.1.0
;*******************************************************************************
            .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
ADC_Result  .equ    R4
;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x5C00,SP              ; Initialize stackpointer
            mov.w   #WDTPW + WDTHOLD,&WDTCTL; Stop WDT

            bis.b   #BIT0,&P1DIR            ; P1.0 output
            bic.b   #BIT0,&P1OUT            ; Clear P1.0
            bis.b   #BIT2,&P1SEL            ; Set P1.2 to non-IO
            dint                            ; Disable interrupts; Port Map cnfg
            mov.w   #PMAPKEY,&PMAPKEYID     ; Enable access Port Mapping regs
            mov.b   #PM_ANALOG,&P1MAP2      ; Enable A0
            clr.w   &PMAPKEYID              ; Disable access Port Mapping regs
            eint                            ; Re-enable all interrupts

            bis.w   #ADC10SHT_2 + ADC10ON,&ADC10CTL0
                                            ; S&H=16 ADC clks, Enable ADC10
            bis.w   #ADC10SHP,&ADC10CTL1    ; ADCCLK = MODOSC; sampling timer
            bis.w   #ADC10RES,&ADC10CTL2    ; 10-bit conversion results
            mov.w   #ADC10INCH_0,&ADC10MCTL0; A0 ADC input select; Vref=AVCC
            bis.w   #ADC10IE0,&ADC10IE      ; Enable ADC conv complete interrupt

while_loop  mov.w   #5000,R4                ; Initialize loop counter R4 = 50,000
delay_loop  dec.w   R4                      ; Decrement loop counter
            jne     delay_loop              ; Loop if loop counter > 0

            bis.w   #ADC10ENC + ADC10SC,&ADC10CTL0
                                            ; Sampling and conversion start
            bis.w   #LPM0 + GIE,SR          ; Enter LPM0 w/ interrupt
            nop

            cmp.w   #0x01FF,&ADC10MEM0
            jn      turn_off_LED
turn_on_LED bis.b   #BIT0,&P1OUT            ; Yes -> set P1.0 (LED)
            jmp     while_loop
turn_off_LED
            bic.b   #BIT0,&P1OUT            ; No  -> reset P1.0 (LED)
            jmp     while_loop              ; Infinite while loop
            nop                             ; For debugger

;-------------------------------------------------------------------------------
ADC10_ISR     ; ADC10 isr
;-------------------------------------------------------------------------------
            add     &ADC10IV,PC
            reti                            ; No interrupts
            reti                            ; ADC10OVIFG
            reti                            ; ADC10TOVIFG
            reti                            ; ADC10HIIFG
            reti                            ; ADC10LOIFG
            reti                            ; ADC10INIFG
            jmp     ADC10IFG_HND            ; ADC10IFG
            reti
ADC10IFG_HND
            mov.w   &ADC10MEM0,ADC_Result   ; Store ADC10 channel 0 result
            bic.w   #LPM0,0(SP)             ; Exit LPM0
            reti                            ;

;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int57"
            .short  ADC10_ISR
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end

