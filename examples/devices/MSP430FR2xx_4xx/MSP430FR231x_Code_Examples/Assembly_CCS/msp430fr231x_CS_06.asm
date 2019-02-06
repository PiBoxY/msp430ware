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
;  MSP430FR231x Demo - Configure MCLK for 4MHz and XT1HF sourcing MCLK.
;
;  Description: MCLK = XT1HF = 4MHz,
;               SMCLK = MCLK/2 = 2MHz.
;               Toggle LED to indicate that the program is running.
;
;           MSP430FR2311
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |          P1.2 |---> LED
;        |               |
;        |          P1.0 |---> SMCLK = 2MHz
;        |               |
;
;
;   Darren Lu
;   Texas Instruments Inc.
;   July 2015
;   Built with Code Composer Studio v6.1
;******************************************************************************

;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"  ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                   ; Make stack linker segment ?known?
;-------------------------------------------------------------------------------
            .global _main
            .text                            ; Assemble to Flash memory
            .retain                          ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

            bis.w   #XTS,&CSCTL6            ; Select XT1 HF mode
            mov.w   #SELMS__XT1CLK+SELA__REFOCLK,&CSCTL4
                                            ; Set MCLK = XT1CLK = 4MHz

XT1on       bis.b   #BIT6+BIT7,&P2SEL1      ; P2.6~P2.7: crystal pins
XT1chk      bic.w   #XT1OFFG+DCOFFG,&CSCTL7 ; Clear XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscilator fault flag
            jnz     XT1chk                  ; If set, attempt to clear again
                                            ; If clear, continue

            bis.w   #DIVM_0+DIVS_1,&CSCTL5  ; MCLK = XT1CLK = 4MHZ
                                            ; SMCLK = MCLK/2 = 2MHz
            bis.b   #BIT0+BIT2,&P1DIR       ; set SMCLK and LED pin as output
            bis.b   #BIT0,&P1SEL1           ; set SMCLK pin as second function

            ; Disable the GPIO power-on default high-impedance mode
            ; to activate previously configured port settings
            bic.w   #LOCKLPM5,&PM5CTL0

Mainloop    xor.b   #BIT2,&P1OUT            ; toggle P1.2
            mov.w   #65000, R13
delay1      dec     R13
            jnz     delay1
            jmp     Mainloop                ; Loop forever
            nop
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   RESET_VECTOR            ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
