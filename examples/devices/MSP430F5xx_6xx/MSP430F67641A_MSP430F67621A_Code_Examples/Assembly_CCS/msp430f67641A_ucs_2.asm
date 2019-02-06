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
;  MSP430F67641A Demo - Software Toggle P1.0 with 8MHz DCO
;
;  Description: Toggle P1.0 by xor'ing P1.0 inside of a software loop.
;  ACLK is rought out on pin PJ.3, SMCLK is brought out on PJ.0, and MCLK
;  is brought out on pin PJ.1.
;  ACLK = REFO = 32kHz, MCLK = SMCLK = 8MHz
;
;              MSP430F67641A
;             ---------------
;         /|\|               |
;          | |           PJ.3|-->ACLK
;          --|RST        PJ.1|-->MCLK
;            |           PJ.0|-->SMCLK
;            |               |
;            |           P1.0|-->LED
;
;  E. Chen
;  Texas Instruments Inc.
;  January 2015
;  Built with CCS Version: 5.5.0
;*******************************************************************************
            .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .global _main 
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x5C00,SP              ; Initialize stackpointer
            mov.w   #WDTPW + WDTHOLD,&WDTCTL; Stop WDT
            bis.b   #BIT0,&P1DIR            ; P1.0 output
            bis.w   #BIT0+BIT1+BIT3,&PJDIR  ; ACLK, MCLK, SMCLK set out to pins
            bis.w   #BIT0+BIT1+BIT3,&PJSEL  ; PJ.0,1,3 for debugging purposes

            ; Initialize clocks
            bis.w   #SELREF_2,&UCSCTL3      ; Set DCO FLL reference = REFO
            bis.w   #SELA_2,&UCSCTL4        ; Set ACLK = REFO

            bis.w   #SCG0,SR                ; Disable the FLL control loop
            clr.w   &UCSCTL0                ; Set lowest possible DCOx, MODx
            mov.w   #DCORSEL_5,&UCSCTL1     ; Select range for 16 MHz operation
            mov.w   #FLLD_1 + 243,&UCSCTL2  ; Set DCO multiplier for 8 MHz
                                            ; (N + 1) * FLLRef = Fdco
                                            ; (243 + 1) * 32768 = 8MHz
                                            ; Set FLL Div = fDCOCLK/2
            bic.w   #SCG0,SR                ; Enable the FLL control loop

  ; Worst-case settling time for the DCO when the DCO range bits have been
  ; changed is n x 32 x 32 x f_FLL_reference. See UCS chapter in 5xx UG
  ; for optimization.
  ; 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
            mov.w   #0xF422,R15
            nop
            clr.w   R14
delay_L1    add.w   #0xFFFF,R15
            addc.w  #0xFFFF,R14
            jc      delay_L1

            ; Loop until XT1,XT2 & DCO fault flag is cleared
do_while    bic.w   #XT2OFFG + XT1LFOFFG + DCOFFG,&UCSCTL7
                                            ; Clear XT2,XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jc      do_while

while_loop  xor.b   #BIT0,&P1OUT            ; Flash the LED
            mov.w   #50000,R4               ; Initialize loop counter R4=50,000
delay_loop  dec.w   R4                      ; Decrement loop counter
            jne     delay_loop              ; Loop if loop counter > 0
            jmp     while_loop              ; Infinite while loop
            nop                             ;

;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
