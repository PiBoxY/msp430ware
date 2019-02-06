; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2013, Texas Instruments Incorporated
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
;  MSP430i20xx Demo - PMM Module, Compare DVCC to 2.85V, VMON ISR
;
;  Description: Setup VMON to compare DVCC to internally generated
;    2.85V. If DVCC drifts below 2.85V, clear the LED. If DVCC drifts
;    above 2.85V, turn the LED on.
;
;  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;               MSP430i20xx
;             -----------------
;         /|\|                |
;          | |                |
;          --|RST             |
;            |            P1.4|--> LED
;            |                |
;
;   T. Witt
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio v5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

            .ref    init                    ; Reference external function

RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
Cal         call    #init                   ; Calibrate peripherals
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #BIT4,&P1DIR            ; LED Output
            bic.b   #BIT4,&P1OUT            ; LED Off
            bis.b   #VMONLVL_3,&VCMONCTL    ; Compare DVCC to 2.85V
            bis.b   #VMONIE,&VCMONCTL       ; Enable VMON Interrupt
            mov.w   #10,R15                 ; VMON requires 0.5us settling time
Settle      dec.w   R15
            jnz     Settle
            bis.w   #GIE,SR

Mainloop    bis.b   #BIT4,&P1OUT            ; DVCC > 2.85V, LED On
            jmp     Mainloop

;-------------------------------------------------------------------------------
VMON_ISR    ; ISR for Voltage Monitor
;-------------------------------------------------------------------------------
            bic.b   #BIT4,&P1OUT            ; DVCC < 2.85V, LED Off
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int11"                ; Voltage Monitor ISR
            .short  VMON_ISR
            .end
