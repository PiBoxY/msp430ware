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
;*******************************************************************************
;  MSP430FR231x Demo - Timer1_B3, PWM TB1.1-2, Up Mode, 32KHz ACLK
;
;  Description: This program generates two PWM outputs on P2.0,P2.1 using
;  Timer1_B configured for up mode. The value in CCR0, 100-1, defines the PWM
;  period and the values in CCR1 and CCR2 the PWM duty cycles. Using 32768Hz
;  ACLK as TBCLK, the timer period is ~3ms with a 75% duty cycle on P2.0
;  and 25% on P2.1.
;  ACLK = TBCLK =32768Hz, MCLK = SMCLK = default DCODIV ~1MHz
;
;
;          MSP430FR2311
;         ---------------
;     /|\|            XIN|-
;      | |               |  ~32768Hz
;      --|RST        XOUT|-
;        |               |
;        |     P2.0/TB1.1|--> CCR1 - 75% PWM
;        |     P2.1/TB1.2|--> CCR2 - 25% PWM
;
;  
;   Darren Lu
;   Texas Instruments Inc.
;   Oct. 2015
;   Built with Code Composer Studio v6.1
;******************************************************************************
            .cdecls C,LIST,"msp430.h"                 ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                                  ; Make stack linker segment ?known?
                
            .global _main
            .text                                           ; Assemble to Flash memory
            .retain                                         ; Ensure current section gets linked
            .retainrefs
_main
                
RESET       mov.w   #__STACK_END,SP                         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL                  ; Stop WDT
                
            bis.b   #BIT0+BIT1,&P2DIR                       ; P2.0 and P2.1 output                          
            bis.b   #BIT0+BIT1,&P2SEL0                      ; P2.0 and P2.1 options select 
            bic.w   #LOCKLPM5,&PM5CTL0                      ; Unlock I/O pin

XT1on       bis.b   #BIT6+BIT7,&P2SEL1                      ; P2.6~P2.7: crystal pins
XT1chk      bic.w   #XT1OFFG+DCOFFG,&CSCTL7                 ; Clear XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1                         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1                         ; Test oscilator fault flag
            jnz     XT1chk                                  ; If set, attempt to clear again
                                                            ; If clear, continue

            mov.w   #100-1,&TB1CCR0                         ; PWM period
            mov.w   #OUTMOD_7,&TB1CCTL1                     ; CCR1 reset/set
            mov.w   #75,&TB1CCR1                            ; CCR1 PWM duty cycle
            mov.w   #OUTMOD_7,&TB1CCTL2                     ; CCR2 reset/set
            mov.w   #25,&TB1CCR2                            ; CCR2 PWM duty cycle
            mov.w   #TBSSEL_1|MC_1|TBCLR,&TB1CTL            ; ACLK, up mode, clear TBR

Mainloop    bis.w   #LPM3,SR                                ; Enter LPM3
            nop                                             ; For debuger

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   RESET_VECTOR                            ; MSP430 RESET Vector
            .short  RESET                   
            .end
