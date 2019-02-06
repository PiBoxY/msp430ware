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
;  MSP430FR69xx Demo - ADC12, Using an External Reference
;
;  Description: This example shows how to use an external positive reference for
;  the ADC12.The external reference is applied to the VeREF+ pin. AVss is used
;  for the negative reference. A single conversion is performed on channel A0.
;  The conversion results are stored in ADC12MEM0. Test by applying a voltage
;  to channel A0, then setting and running to a break point at the
;  "__no_operation()" instruction. To view the conversion results, open an
;  SFR window in debugger and view the contents of ADC12MEM0 or from the
;  variable ADCvar.
;  NOTE: VeREF+ range: 1.4V (min) to AVCC (max)
;        VeREF- range: 0V (min) to 1.2V (max)
;        Differential ref voltage range: 1.4V(min) to AVCC(max)
;           (see datasheet for device specific information)
;
;                MSP430FR6989
;             -------------------
;         /|\|                   |
;          | |                   |
;          --|RST                |
;            |                   |
;     Vin -->|P1.0/A0            |
;            |                   |
;     REF -->|P1.1/VREF+/VeREF+  |
;            |                   |
;
;   E. Chen
;   Texas Instruments Inc.
;   April 2014
;   Built with Code Composer Studio V6.0
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
ADCvar      .set    R5                      ; ADC conversion result
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupGPIO   bis.b   #BIT0,&P1SEL1           ; Enable A/D channel A0
            bis.b   #BIT0,&P1SEL0           ;
UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

SetupADC12  mov.w   #ADC12SHT0_2+ADC12ON,&ADC12CTL0 ; Turn on ADC12, set sampling time
            mov.w   #ADC12SHP,&ADC12CTL1    ; Use sampling timer
            mov.w   #ADC12VRSEL_4,&ADC12MCTL0 ; Vr+ = VeREF+ (ext) and Vr-=AVss
            bis.w   #ADC12ENC,&ADC12CTL0    ; Enable conversions
                                            ;
Mainloop    bis.w   #ADC12SC,ADC12CTL0      ; Start conversion-software trigger
ADCResult   bit.w   #BIT0,&ADC12IFGR0       ;
            jeq     ADCResult               ;
            mov.w   &ADC12MEM0,ADCvar      ; Read conversion result
            nop                             ; for debug
            jmp     Mainloop                ; Again
            nop

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
