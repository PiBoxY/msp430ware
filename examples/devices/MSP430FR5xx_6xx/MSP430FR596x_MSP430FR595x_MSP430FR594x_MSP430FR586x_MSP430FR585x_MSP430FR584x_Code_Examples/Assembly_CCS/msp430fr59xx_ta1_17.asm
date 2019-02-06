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
;  MSP430FR59x Demo - Timer1_A3, PWM TA1.1-2, Up Mode, 32KHz ACLK
;
;  Description: This program generates two PWM outputs on P1.2,P1.3 using
;  Timer1_A configured for up mode. The value in CCR0, 100-1, defines the PWM
;  period and the values in CCR1 and CCR2 the PWM duty cycles. Using 32768Hz
;  ACLK as TACLK, the timer period is ~3ms with a 75% duty cycle on P1.2
;  and 25% on P1.3.
;  ACLK = TACLK =32768Hz, SMCLK = MCLK = DCO / 2 = 8MHz / 2 = 4MHz
;
;          MSP430FR5969
;         ---------------
;     /|\|            XIN|-
;      | |               |  32KHz Crystal
;      --|RST        XOUT|-
;        |               |
;        |     P1.2/TA0.1|--> CCR1 - 75% PWM
;        |     P1.3/TA0.2|--> CCR2 - 25% PWM
;
;   E. Chen
;   Texas Instruments Inc.
;   October 2013
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
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupGPIO   bis.b   #BIT2+BIT3,&P1DIR       ; P1.2 and P1.3 output
            bis.b   #BIT2+BIT3,&P1SEL0      ; P1.2 and P1.3 options select
            bis.w   #BIT4+BIT5,&PJSEL0

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

            mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #DCOFSEL_6,&CSCTL1      ; Set DCO to 8MHz
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2 ; Set ACLK = XT1; MCLK = DCO
            mov.w   #DIVA__1+DIVS__2+DIVM__2,&CSCTL3 ; Set all dividers
            bic.w   #LFXTOFF,&CSCTL4

OSCFlag     bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            clr.b   &CSCTL0_H               ; Lock CS registers

            mov.w   #100-1,&TA1CCR0         ; PWM Period
            mov.w   #OUTMOD_7,&TA1CCTL1     ; CCR1 reset/set
            mov.w   #75,&TA1CCR1            ; CCR1 PWM duty cycle
            mov.w   #OUTMOD_7,&TA1CCTL2     ; CCR2 reset/set
            mov.w   #25,&TA1CCR2            ; CCR2 PWM duty cycle
            mov.w   #TASSEL__ACLK+MC__UP+TACLR,&TA1CTL ; ACLK, up mode, clear TAR

            bis.w   #LPM3,SR                ; Enter LPM3
            nop                             ; For debugger

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
