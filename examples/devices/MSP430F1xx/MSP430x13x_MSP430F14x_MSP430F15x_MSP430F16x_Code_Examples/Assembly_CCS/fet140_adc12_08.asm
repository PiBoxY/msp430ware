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
;   MSP-FET430P140 Demo - ADC12, Using 10 External Channels for Conversion
;
;   Description: This program will show how to obtain 10 external channels for
;                conversion.
;
;   This example shows how to perform A/D conversion on up to 10 external
;   channels by showing that channels A8 and A9 can be used for conversion
;   of external signals when not using these channels as external reference 
;   inputs. A single sequence of conversions is performed - one
;   conversion on A8 and then one conversion on A9. Each conversion uses AVcc
;   and AVss for the references. The conversion results are stored in ADC12MEM0
;   and ADC12MEM1 respectively and are moved to R5 and R6 respectively after
;   the sequence is complete. Test by applying voltages to pins VeREF+ for A8
;   and VREF/VeREF- for A9, then setting and running to a break point at
;   "jmp Mainloop."  To view the conversion results, open a register window in
;   debugger and view the contents of R5 and R6.
;
;   *NOTE*  When using channels A8 and A9 for external signals, internal
;   references must be used for the conversions. Refer to figure 17-1 in the
;   MSP430x1xx Family User's Guide.
;
;   *Note*  This example only functions on MSP production silicon, not PMS
;   pre-production silicon.  Production silicon will be noted on the chip as
;   "M430F149" whereas pre-production silicon will be marked "P430F149."
;
;
;
;               MSP430F149
;            -----------------
;           |                 |
;   Vin1 -->|VeREF+           |
;   Vin2 -->|VREF-/VeREF-     |
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
                                            ;
SetupADC12  mov     #ADC12ON+MSC+SHT0_15,&ADC12CTL0
                                            ; Turn on ADC12, set MSC bit
            mov     #SHP+CONSEQ_1,&ADC12CTL1; Use samp. timer, single sequence
            bis.b   #INCH_8,&ADC12MCTL0     ; AVcc=ref+, channel=A8
            bis.b   #INCH_9+EOS,&ADC12MCTL1 ; AVcc=ref+, channel=A9, .end seq.
                                            ;
            mov     #BIT1,&ADC12IE          ; Enable ADC12IFG.1 for ADC12MEM1
            bis     #ENC,&ADC12CTL0         ; Enable conversions
                                            ;
Mainloop    bis     #ADC12SC,&ADC12CTL0     ; Start conversions
            bis     #CPUOFF+GIE,SR          ; Hold in LPM0 for seq to complete
                                            ; Enable interrupts
            nop                             ; Only required for debug.
            jmp     Mainloop                ; SET BREAKPOINT HERE
                                            ;
;------------------------------------------------------------------------------
ADC12ISR    ; Interrupt Service Routine for ADC12
;------------------------------------------------------------------------------
            mov     &ADC12MEM0,R5           ; Move A8 result
            mov     &ADC12MEM1,R6           ; Move A9 result
            bic     #CPUOFF,0(SP)
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
