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
;   MSP-FET430P430 Demo - ADC12, Sample A0, set P5.1 if A0 > 0.5*AVcc
;
;   Description: A single sample is made on A0 with reference to AVcc.
;   Software sets ADC12SC to start sample and conversion - ADC12SC
;   automatically cleared at EOC. ADC12 internal oscillator times sample (16x)
;   and conversion. In Mainloop MSP430 waits in LPM0 to save power until ADC12
;   conversion complete, ADC12_ISR will force exit from LPM0 in Mainloop on
;   reti. If A0 > 0.5*AVcc P5.1 set, else reset.
;   ACLK = n/a, MCLK = SMCLK = default DCO, ADC12CLK = ADC12OSC
;
;
;                MSP430FG439
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;     Vin -->|P6.0/A0      P5.1|--> LED
;
;   M. Buccini / M. Mitchell
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                  ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupADC12  mov.w   #SHT0_2+ADC12ON,&ADC12CTL0     ; Sampling time, ADC12 on
            mov.w   #SHP,&ADC12CTL1         ; Use sampling timer
            mov.w   #01h,&ADC12IE           ; Enable interrupt
            bis.w   #ENC,&ADC12CTL0         ;
            bis.b   #01h,&P6SEL             ; P6.0 ADC option select
            bis.b   #02h,&P5DIR             ; P5.1 output
                                            ;
Mainloop    bis.w   #ADC12SC,&ADC12CTL0     ; Start sampling/conversion
            bis.w   #CPUOFF+GIE,SR          ; LPM0, ADC12_ISR will force exit
            jmp     Mainloop                ; Again
                                            ;
;------------------------------------------------------------------------------
ADC12_ISR;  Exit any LPMx on reti
;------------------------------------------------------------------------------
            bic.b   #02h,&P5OUT             ; P5.1 = 0
            cmp.w   #07FFh,&ADC12MEM0       ; ADC12MEM = A0 > 0.5AVcc?
            jlo     ADC12_ISR_1             ;
            bis.b   #02h,&P5OUT             ; P5.1 = 1
ADC12_ISR_1 bic.w   #CPUOFF,0(SP)           ; Exit any LPM0 on reti
            reti                            ;		
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .sect   ".int07"                ; ADC12 Vector
            .short  ADC12_ISR               ;
            .end
