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
;   MSP430F41x2 Demo - ADC10, DTC Sample A1-0 32x, AVcc, Repeat Seq, DCO
;
;   Description: Use DTC to sample A1/A0 repeat sequence 32x(64 total samples)
;   with reference to AVcc.  Software sets ADC10SC to trigger sample burst.
;   In Mainloop MSP430 waits in LPM0 to save power until ADC10 conversion
;   complete, ADC10_ISR will force exit from LPM0 in Mainloop on reti.
;   ADC10 internal oscillator times sample period (16x) and conversion (13x).
;   DTC transfers conversion code to RAM 200h - 280h.  ADC10(DTC) interrupt
;   will return system active. P5.1 set at start of conversion burst, reset
;   on completion.
;
;                MSP430F41x2
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;        >---|P7.5/A1      P5.1|-->LED
;        >---|P7.4/A0          |
;
;  P. Thanigai
;  Texas Instruments Inc.
;  January 2009
;  Built with CCE Version: 3.1   
;******************************************************************************
 .cdecls C,LIST, "msp430.h" 
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
  
            .global _main
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
_main
RESET       mov.w   #0400h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupADC10  mov.w   #INCH_1+CONSEQ_3,&ADC10CTL1 ; A1/A0, repeat multi channel
            mov.w   #ADC10SHT_2+MSC+ADC10ON+ADC10IE,&ADC10CTL0 ;
            bis.b   #03h,&ADC10AE0          ; P7.4,5 ADC option select
            mov.b   #040h,&ADC10DTC1        ; 64 conversions
SetupP1     bis.b   #002h,&P5DIR            ; P5.1 output
                                            ;
Mainloop    bis.b   #002h,&P5OUT            ; P5.1 = 1
busy_test   bit     #BUSY,&ADC10CTL1        ; ADC10 core inactive?
            jnz     busy_test               ;
            mov.w   #0200h,&ADC10SA         ; Data buffer start
            bis.w   #ENC+ADC10SC,&ADC10CTL0 ; Start sampling
            bis.w   #CPUOFF+GIE,SR          ; CPU off, Enable interrupts
            bic.b   #002h,&P5OUT            ; P5.1 = 0
            jmp     Mainloop                ; Again
                                            ;
;-------------------------------------------------------------------------------
ADC10_ISR;
;-------------------------------------------------------------------------------
            bic.w   #ENC,&ADC10CTL0         ; ADC10 disabled
            bic.w   #LPM0,0(SP)             ; Exit LPM0 on reti
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int07"            	; ADC10 Vector
            .short  ADC10_ISR
            .sect	".reset"	            ; POR, ext. Reset
            .short	RESET
            .end
