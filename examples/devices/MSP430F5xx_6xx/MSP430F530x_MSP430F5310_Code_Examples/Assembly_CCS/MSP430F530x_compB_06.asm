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
;  MSP430F530x Demo - COMPB and TIMERAx interaction (TA0.1, TA1.1)
;
;  Description: Use CompB to determine if input, Vcompare has a duty cycle
;  greater than 50%. When Vcompare exceeds Vcc*3/4 then TimerA0 captures the
;  time (TA0CCR1). When Vcompare is less than Vcc*1/4 then TimerA1 captures the
;  time (TA1CCR1) and resets the timers for TIMERA0 and TIMERA1. If TA0CCR1 is
;  greater than TA1CCR1/2, then turn on the LED. If TA0CCR1 is less than
;  TA1CCR1/2, then turn off the LED.
;
;   Clocks: ACLK = REFO; MCLK = SMCLK = 12MHz
;
;                 MSP430x530x
;             ------------------
;         /|\|                  |
;          | |                  |
;          --|RST           CB0 |<--Vcompare (200Hz<f<1Mhz) (vary dutycycle)
;            |                  |
;            |                  |
;            |              P1.0|-->LED ('ON' if dutycycle > 50%;
;                                        'OFF' if dutycycle < 50%)
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

; Setup UCS: ACLK = REFO; MCLK = SMCLK = 12MHz
            bis.w   #SELREF_2,&UCSCTL3      ; Set DCO FLL reference = REFO
            bis.w   #SELA_2,&UCSCTL4        ; Set MCLK = SMCLK = DCOCLK

            bis.w   #SCG0,SR                ; Disable the FLL control loop
            mov.w   #0,&UCSCTL0             ; Set lowest possible DCOx, MODx
            mov.w   #DCORSEL_5,&UCSCTL1     ; Select range for 24 MHz operation
            mov.w   #FLLD_1 | 374,&UCSCTL2  ; Set DCO multiplier for 12 MHz
                                            ; (N + 1) * FLLRef = Fdco
                                            ; (374 + 1) * 32768 = 12MHz
                                            ; Set FLL Div = fDCOCLK/2
            bic.w   #SCG0,SR                ; Enable the FLL control loop

; Worst-case settling time for the DCO when the DCO range bits have been
; changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
; UG for optimization.
; 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
            mov.w   #0x6E34,R15
            nop
            mov.w   #0x1,R14
delay_L1    add.w   #0xFFFF,R15
            addc.w  #0xFFFF,R14
            jc      delay_L1

; Loop until XT1,XT2 & DCO fault flag is cleared
do_while    bic.w   #XT2OFFG | XT1LFOFFG | DCOFFG,&UCSCTL7
                                            ; Clear XT2,XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jc      do_while

; Setup ComparatorB
            bis.w   #CBIPEN | CBIPSEL_0,&CBCTL0
                                            ; Enable V+, input channel CB0
            bis.w   #CBPWRMD_0,&CBCTL1      ; CBMRVS=0 => select VREF1 as
                                            ; ref when CBOUT is high and
                                            ; VREF0 when CBOUT is low.
                                            ; High-Speed Power mode
            bis.w   #CBRSEL,&CBCTL2         ; Ref is applied to -terminal
            bis.w   #CBRS_1|CBREF13,&CBCTL2 ; VREF1 is Vcc*1/4
            bis.w   #CBREF04|CBREF03,&CBCTL2; VREF0 is Vcc*3/4
            bis.w   #BIT0,&CBCTL3           ; Input Buffer Disable @P6.0/CB0
            bis.w   #CBON,&CBCTL1           ; Turn On ComparatorB

; delay for the reference to settle
            mov.w   #75,R4                  ; Initialize loop counter R4=75
delay_loop  dec.w   R4                      ; Decrement loop counter
            jnz     delay_loop              ; Loop if loop counter > 0

; Setup Timer_A0 and Timer_A1
; Internally CBOUT --> TA0CCTL1:CCIB (TA0.1)
;                  +-> TA1CCTL1:CCIB (TA1.1)
            mov.w   #TASSEL_2 | MC_1,&TA0CTL; SMCLK, upmode
            mov.w   #TASSEL_2 | MC_1,&TA1CTL; SMCLK, upmode
            mov.w   #0xFFFF,&TA0CCR0        ;
            mov.w   #0xFFFF,&TA1CCR0        ;
            mov.w   #CM_2|SCS|CAP|CCIS_1,&TA0CCTL1
                                            ; Capture Falling Edge
            mov.w   #CM_1|SCS|CAP|CCIS_1|CCIE,&TA1CCTL1
                                            ; Capture Rising Edge,
                                            ; Enable Interrupt

            bis.b   #BIT0,&P1DIR            ; Set P1.0/LED to output
            nop
Mainloop    bis.w   #LPM0|GIE,SR            ; Enter LPM0, interrupts enabled
            nop                             ; For Debug

;-------------------------------------------------------------------------------
TAX_ISR;    Common ISR for CCR1-4 and overflow
;-------------------------------------------------------------------------------
            add.w   &TA1IV,PC               ; Add Timer_A offset vector
            reti                            ; Vector 0 - no interrupt
            jmp     CCIIFG_1_HND            ; Vector 1 - CCR1
            reti                            ; Vector 2 - CCR2 not used
            reti                            ; Vector 3 - CCR3 not used
            reti                            ; Vector 4 - CCR4 not used
            reti                            ; Vector 5 - CCR5 not used
            reti                            ; Vector 6 - Reserved not used
TA0IFG_HND  nop                             ; Vector 7 - overflow
while_loop  jmp     while_loop              ; If input frequency < 200Hz,
                                            ; trap here
            reti                            ; Return from overflow ISR

CCIIFG_1_HND
            mov.w   #0,&TA0CCR0             ;
            mov.w   #0,&TA1CCR0             ;
            mov.w   #0xFFFF,&TA0CCR0        ;
            mov.w   #0xFFFF,&TA1CCR0        ; Halt and resart TimerA0 and A1
                                            ; counters
            mov.w   &TA1CCR1,R15            ; Compare On and off time of input
                                            ; signal
            rrum.w  #1,R15                  ;
            cmp.w   &TA0CCR1,R15            ;
            jc      LED_off                 ;
            bis.b   #BIT0,&P1OUT            ;
            reti                            ;
LED_off     bic.b   #BIT0,&P1OUT            ;
            reti                            ;

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   "int48"                 ; Timer_AX Vector
            .short  TAX_ISR                 ;
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
