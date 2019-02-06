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
;   MSP-FET430P140 Demo - ADC12, Using the Internal Reference
;
;   Description: This program will show how to use the internal reference
;                for ADC12.
;
;   This example shows how to use the internal reference of the ADC12.
;   It uses the internal 2.5V reference and performs a single conversion
;   on channel A0. The conversion results are stored in ADC12MEM0 and are
;   moved to R5 after the conversion is complete. Test by applying a voltage
;   to channel A0, then setting and running to a break point at "jmp Mainloop".
;   To view the conversion results, open a register window in Debugger and view
;   the contents of R5.
;
;
;                MSP430F149
;              ---------------
;             |               |
;      Vin -->|P6.0/A0        |
;             |               |
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
            bis.b   #BIT0,&P6SEL            ; Enable A/D channel A0
                                            ;
SetupADC12  mov     #REFON+REF2_5V+ADC12ON+SHT0_2,&ADC12CTL0
                                            ; Turn on 2.5V ref, set samp time
            mov     #SHP,&ADC12CTL1         ; Use sampling timer
            mov.b   #SREF_1,&ADC12MCTL0     ; Vr+=Vref+
                                            ;
            mov     #03600h,R15             ; Delay for needed ref start-up.
L$1         dec     R15                     ; See datasheet for details.
            jnz     L$1                     ;
                                            ;
            bis     #ENC,&ADC12CTL0         ; Enable conversions
                                            ;
Mainloop    bis     #ADC12SC,&ADC12CTL0     ; Start conversions
testIFG     bit     #BIT0,&ADC12IFG         ; Conversion done?
            jz      testIFG                 ; No, test again
            mov     &ADC12MEM0, R5          ; Move result
            jmp     Mainloop                ; SET BREAKPOINT HERE
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
