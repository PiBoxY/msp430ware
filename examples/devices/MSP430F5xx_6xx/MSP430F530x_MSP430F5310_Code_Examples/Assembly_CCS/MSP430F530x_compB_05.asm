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
;  MSP430x530x Demo - COMPB Hysteresis, CBOUT Toggle in LPM4; High speed mode
;
;  Description: Use CompB and shared reference to determine if input 'Vcompare'
;  is high of low.  Shared reference is configured to generate hysteresis.
;  When Vcompare exceeds Vcc*3/4 CBOUT goes high and when Vcompare is less 
;  than Vcc*1/4 then CBOUT goes low.
;  Connect P1.6/CBOUT to P1.0 externally to see the LED toggle accordingly.
;
;                 MSP430x530x
;             ------------------                        
;         /|\|                  |                       
;          | |                  |                       
;          --|RST      P6.0/CB0 |<--Vcompare            
;            |                  |                       
;            |        P1.6/CBOUT|----> 'high'(Vcompare>Vcc*3/4);
;            |                  |  |   'low'(Vcompare<Vcc*1/4)
;            |            P1.0  |__| LED 'ON'(Vcompare>Vcc*3/4);
;            |                  |        'OFF'(Vcompare<Vcc*1/4)
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
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            
SetupPort   bis.b   #BIT6,&P1DIR            ; P1.6 output direction
            bis.b   #BIT6,&P1SEL            ; Select CBOUT function on
                                            ; P1.6/CBOUT
; Setup ComparatorB
            bis.w   #CBIPEN|CBIPSEL_0,&CBCTL0; Enable V+, input channel CB0
            bis.w   #CBPWRMD_0,&CBCTL1      ; CBMRVS=0 => select VREF1 as ref 
                                            ; when CBOUT is high and VREF0 when 
                                            ; CBOUT is low, High-Speed Power
                                            ; mode
            bis.w   #CBRSEL,&CBCTL2         ; VREF is applied to -terminal
            bis.w   #CBRS_1|CBREF13,&CBCTL2 ; VREF1 is Vcc*1/4
            bis.w   #CBREF04|CBREF03,&CBCTL2; VREF0 is Vcc*3/4
            
            bis.w   #BIT0,&CBCTL3           ; Input Buffer Disable @P6.0/CB0 
            bis.w   #CBON,&CBCTL1           ; Turn On ComparatorB
            
; Delay for reference settle = 75us
            mov.w   #0x1FF,R15
delay_L1    dec.w   R15
            jnz     delay_L1
            
            bis.w   #LPM4,SR                ; Enter LPM4
            nop

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
