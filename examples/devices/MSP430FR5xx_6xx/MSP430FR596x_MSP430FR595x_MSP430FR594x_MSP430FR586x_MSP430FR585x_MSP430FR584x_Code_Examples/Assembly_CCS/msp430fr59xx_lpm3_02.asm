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
;   MSP430FR59xx Demo - Enters LPM3 with ACLK = VLO
;
;   Description: Configures WDT and ACLK to be sourced from VLO and enters LPM3.
;   Note: On the FET board P1.0 drives an LED that can show high power numbers
;   when turned ON. Measure current with LED jumper JP3 disconnected.
;   LED jumper disconnected.
;   ACLK = VLOCLK = ~9.4KHz, MCLK = SMCLK = 1MHz
;
;          MSP430FR5969
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |               |
;        |          P1.0 |---> Disconnect JP9 for power meas.
;
;   Tyler Witt/ P. Thanigai
;   Texas Instruments Inc.
;   August 2012
;   Built with Code Composer Studio V5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
            mov.w   #WDTPW+WDTTMSEL+WDTSSEL_1+WDTIS_5,&WDTCTL  ; ACLK, 1s interrupts
            mov.w   #WDTIE,&SFRIE1          ; enable WDT interrupt
SetupGPIO   clr.b   &P1OUT
            mov.b   #0xFF,&P1DIR

            clr.b   &P2OUT
            mov.b   #0xFF,&P2DIR

            clr.b   &P3OUT
            mov.b   #0xFF,&P3DIR

            clr.b   &P4OUT
            mov.b   #0xFF,&P4DIR

            clr.w   &PJOUT
            mov.w   #0xFFFF,&PJDIR

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

            mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #DCOFSEL_0,&CSCTL1      ; Set DCO to 1MHz
            mov.w   #SELA__VLOCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2
            mov.w   #DIVA__1+DIVS__1+DIVM__1,&CSCTL3  ; Set all dividers to 1
            bis.w   #LFXTOFF+HFXTOFF,&CSCTL4
            clr.b   &CSCTL0_H               ; Lock CS registers

Mainloop    bis.b   #BIT0,&P1DIR            ; Turn on LED
            nop                             ; 
            bis.w   #LPM3+GIE,SR            ; Go into LPM3 w/ interrupts
            nop

;-------------------------------------------------------------------------------
WDT_ISR;    ISR for WDT
;-------------------------------------------------------------------------------
            xor.b   #BIT0,&P1OUT            ; Toggle P1.0
            reti                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   WDT_VECTOR              ; WDT Interrupt
            .short  WDT_ISR
            .end
