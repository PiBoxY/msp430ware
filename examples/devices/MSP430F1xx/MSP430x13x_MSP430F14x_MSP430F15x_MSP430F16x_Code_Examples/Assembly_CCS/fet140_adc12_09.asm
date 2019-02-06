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
;   MSP-FET430P140 Demo - ADC12, Sequence of Conversions (non-repeated)
;
;   Description: This program will show how to convert a non-repeated sequence
;                of channels.
;
;   This example shows how to perform A/D conversions on a sequence of channels.
;   A single sequence of conversions is performed - one conversion each on
;   channels A0, A1, A2, and A3. Each conversion uses AVcc and AVss for the
;   references. The conversion results are stored in ADC12MEM0, ADC12MEM1,
;   ADC12MEM2, and ADC12MEM3 respectively and are moved to R5, R6, R7, and R8
;   respectively after the sequence is complete. Test by applying voltages to
;   pins A0, A1, A2, and A3, then setting and running to a break point at
;   "jmp Mainloop."  To view the conversion results, open a register window in
;   debugger and view the contents of R5, R6, R7, and R8.
;
;   Note that a sequence has no restrictions on which channels are converted.
;   For example, a valid sequence could be A0, A3, A2, A4, A2, A1, A0, and A7.
;   See the MSP430x1xx User's Guide for instructions on using the ADC12.
;
;   *Note*  This example only functions on MSP production silicon, not PMS
;   pre-production silicon.  Production silicon will be noted on the chip as
;   "M430F149" whereas pre-production silicon will be marked "P430F149."
;
;
;                MSP430F149
;            -----------------
;           |                 |
;   Vin0 -->|P6.0/A0          |
;   Vin1 -->|P6.1/A1          |
;   Vin2 -->|P6.2/A2          |
;   Vin3 -->|P6.3/A3          |
;           |                 |
;
;
;   M. Mitchell / G. Morton
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
RESET       mov     #0A00h,SP               ; Initialize stackpointer
StopWDT     mov     #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog
            bis.b   #BIT0+BIT1+BIT2+BIT3,&P6SEL ; Enable A/D channel inputs
                                            ;
SetupADC12  mov     #ADC12ON+MSC+SHT0_2,&ADC12CTL0  ; Turn on ADC12, .set MSC
            mov     #SHP+CONSEQ_1,&ADC12CTL1; Use samp. timer, single sequence
            bis.b   #INCH_0,&ADC12MCTL0     ; AVcc=ref+, channel=A0
            bis.b   #INCH_1,&ADC12MCTL1     ; AVcc=ref+, channel=A1
            bis.b   #INCH_2,&ADC12MCTL2     ; AVcc=ref+, channel=A2
            bis.b   #INCH_3+EOS,&ADC12MCTL3 ; AVcc=ref+, channel=A3, .end seq.
                                            ;
            mov     #BIT3,&ADC12IE          ; Enable ADC12IFG.0 for ADC12MEM0
            bis     #ENC,&ADC12CTL0         ; Enable conversions
                                            ;
Mainloop    bis     #ADC12SC,&ADC12CTL0     ; Start conversions
            bis     #CPUOFF+GIE,SR          ; Wait in LPM0 for seq to complete
                                            ; Enable interrupts
            nop                             ; Only Required for debug
            jmp     Mainloop                ; SET BREAKPOINT HERE
                                            ;
;------------------------------------------------------------------------------
ADC12ISR    ; Interrupt Service Routine for ADC12
;------------------------------------------------------------------------------
            bic     #CPUOFF,0(SP)
            mov     &ADC12MEM0,R5           ; Move A0 result
            mov     &ADC12MEM1,R6           ; Move A1 result
            mov     &ADC12MEM2,R7           ; Move A2 result
            mov     &ADC12MEM3,R8           ; Move A3 result, IFG is reset
            reti                            ;
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int07"                ; ADC12 Interrupt Vector
            .short  ADC12ISR                ;
            .end
