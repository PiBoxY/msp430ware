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
;  MSP430FR413x Demo - Configure MCLK for 8MHz sourced from DCO.
;
;  Description: Default DCODIV is MCLK and SMCLK source.
;  By default, FR413x select XT1 as FLL reference.
;  If XT1 is present, the XIN and XOUT pin needs to configure.
;  If XT1 is absent, switch to select REFO as FLL reference automatically.
;  XT1 is considered to be absent in this example.
;  f(DCOCLK) = 2^FLLD * (FLLN+1) * (fFLLREFCLK / n).
;  FLLD = 0, FLLN =243, n=1, DIVM =1, f(DCOCLK) = 2^0 * (243+1)*32768Hz = 8MHz,
;  f(DCODIV) = (243+1)*32768Hz = 8MHz,
;  ACLK = default REFO ~32768Hz,  SMCLK = MCLK =f(DCODIV) = 8MHz.
;
;           MSP430FR4133
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |          P1.4 |---> MCLK = 8MHz
;        |          P8.0 |---> SMCLK = 8MHz
;        |          P8.1 |---> ACLK = 32768Hz
;
;
;   Wei Zhao
;   Texas Instruments Inc.
;   June 2014
;   Built with Code Composer Studio v6.0
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"  ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?
;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

            bis.w   #SCG0,SR                ; Disable FLL
            bis.w   #SELREF__REFOCLK,&CSCTL3; Set REFOCLK as FLL reference source
            mov.w   #0,&CSCTL0              ; clear DCO and MOD registers
            bic.w   #DCORSEL_7,&CSCTL1      ; Clear DCO frequency select bits first
            bis.w   #DCORSEL_3,&CSCTL1      ; Set DCOCLK = 8MHz, max in FR413x
            mov.w   #FLLD_0+243,&CSCTL2     ; DCODIV = 8MHz
            nop
            nop
            nop
            bic.w   #SCG0,SR                ; Enable FLL
Unlock      mov.w   &CSCTL7,R13
            and.w   #FLLUNLOCK0|FLLUNLOCK1,R13
            jnz     Unlock                  ; Check if FLL is locked

Go_on       mov.w   #SELMS__DCOCLKDIV+SELA__REFOCLK,&CSCTL4   ; set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                            ; default DCODIV as MCLK and SMCLK source

            bis.b   #BIT0+BIT4,&P1DIR       ; set ACLK/SMCLK/MCLK pin
            bis.b   #BIT4,&P1SEL0
            bis.b   #BIT0+BIT1,&P8DIR
            bis.b   #BIT0+BIT1,&P8SEL0

            ; Disable the GPIO power-on default high-impedance mode
            ; to activate previously configured port settings
            bic.w   #LOCKLPM5,PM5CTL0

Mainloop    xor.b   #BIT0,&P1OUT
            mov.w   #(65000), R13           ; Arbitrary delay to blink the LED
Delay1      sub.w   #1, R13
            jne     Delay1
            jmp     Mainloop                ; endless loop
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   RESET_VECTOR            ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
