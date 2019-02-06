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
;   MSP430x261x Demo - ADC12, Extend Sampling Period with SHT Bits
;
;   This example shows how to extend the sampling time using the sampling
;   timer. In this example, the ADC12OSC is used to provide the sampling period
;   and the SHT0 bits are set to extend the sampling period to 4xADC12CLKx256.
;   A single conversion is performed on channel A0. The A/D conversion results
;   are stored in ADC12MEM0 and are moved to R5 upon completion of the
;   conversion. Test by setting and running to a break point at "jmp Mainloop."
;   To view the conversion results, open a register window in debugger and view
;   the contents of R5.
;
;                MSP430F241x
;                MSP430F261x
;              ---------------
;             |            XIN|-
;      Vin -->|P6.0/A0        | 32kHz
;             |           XOUT|-
;
;  JL Bile
;  Texas Instruments Inc.
;  June 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0850h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog
                                            ;
SetupADC12  mov.w   #ADC12ON+SHT0_15,&ADC12CTL0 ; Turn on ADC12, set SHT0
                                                ; for longer sampling
            mov.w   #SHP,&ADC12CTL1         ; Use sampling timer
            bis.w   #01h,&ADC12IE           ; Enable ADC12IFG.0 for ADC12MEM0
            bis.w   #ENC,&ADC12CTL0         ; Enable conversions
                                            ;
Mainloop    bis.w   #ADC12SC,&ADC12CTL0     ; Start conversions
            bis.w   #LPM0+GIE,SR            ; Wait for conversion completion
                                            ; Enable interrupts
            nop                             ; Only required for CSPY
            jmp     Mainloop                ; SET BREAKPOINT HERE
                                            ;
;-------------------------------------------------------------------------------
ADC12_ISR   ; Interrupt Service Routine for ADC12
;-------------------------------------------------------------------------------
            mov.w   &ADC12MEM0,R5           ; Move result, IFG is reset
            bic.w   #CPUOFF,0(SP)           ; Return active
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".int21"             ; ADC12 Vector
            .short	ADC12_ISR
            .sect	".reset"            ; POR, ext. Reset
            .short	RESET
            .end
