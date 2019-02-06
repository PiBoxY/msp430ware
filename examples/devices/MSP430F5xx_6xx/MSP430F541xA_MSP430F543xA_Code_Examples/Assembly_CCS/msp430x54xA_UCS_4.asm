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
;  MSP430F54xA Demo - FLL+, Runs Internal DCO at 2.45MHz with LFXT1 as Ref
;
;  Description: This program demonstrates setting the internal DCO to run at
;  2.45MHz with auto-calibration by the FLL+ circuitry.It uses LFXT1 as FLL
;  reference.
;  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = (74+1) x REFO = 2457600Hz	
;
;                MSP430F5438A
;             -----------------
;        /|\ |              XIN|-
;         |  |                 | 32kHz
;         ---|RST          XOUT|-
;            |                 |
;            |            P11.1|--> MCLK = 2.45MHz
;            |                 |
;            |            P11.0|--> ACLK = 32kHz
;            |                 |
;
;   D. Dang
;   Texas Instruments Inc.
;   December 2009
;   Built with CCS Version: 4.0.2 
;******************************************************************************

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
            bic.b   #BIT0,&P1OUT            ; Clear P1.0
            bis.b   #BIT0,&P1DIR            ; P1.0 output
            bis.b   #BIT0 + BIT1,&P11DIR    ; P11.0,1 output
            bis.b   #BIT0 + BIT1,&P11SEL    ; P11.0,1 output ACLK, MCLK

            ; Initialize LFXT1
            bis.b   #0x03,&P7SEL            ; Select XT1
            bic.w   #XT1OFF,&UCSCTL6        ; XT1 On
            bis.w   #XCAP_3,&UCSCTL6        ; Internal load cap

            ; Loop until XT1 fault flag is cleared
do_while1   bic.w   #XT1LFOFFG,&UCSCTL7     ; Clear XT1 fault flags
            bit.w   #XT1LFOFFG,&UCSCTL7     ; Test XT1 fault flag
            jc      do_while1

            ; Initialize DCO to 2.45MHz
            bis.w   #SCG0,SR                ; Disable the FLL control loop
            clr.w   &UCSCTL0                ; Set lowest possible DCOx, MODx
            mov.w   #DCORSEL_3,&UCSCTL1     ; 4.9 MHz nominal DCO
            mov.w   #FLLD_1 + 74,&UCSCTL2   ; Set DCO Multiplier for 2.45MHz
                                            ; (N + 1) * FLLRef = Fdco
                                            ; (74 + 1) * 32768 = 2.45MHz
                                            ; Set FLL Div = fDCOCLK/2
            bic.w   #SCG0,SR                ; Enable the FLL control loop

  ; Worst-case settling time for the DCO when the DCO range bits have been
  ; changed is n x 32 x 32 x f_FLL_reference. See UCS chapter in 5xx UG
  ; for optimization.
  ; 32 x 32 x 2.45 MHz / 32,768 Hz = 76563 = MCLK cycles for DCO to settle
            nop
            mov.w   #0x63AF,R15
delay_L1    add.w   #0xFFFF,R15
            jc      delay_L1

            ; Loop until XT1,XT2 & DCO stabilizes
do_while2   bic.w   #XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG,&UCSCTL7
                                            ; Clear XT2,XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jc      do_while2

while_loop  xor.b   #BIT0,&P1OUT            ; Flash the LED
            mov.w   #50000,R4               ; Initialize loop counter R4=50,000
delay_loop  dec.w   R4                      ; Decrement loop counter
            jne     delay_loop              ; Loop if loop counter > 0
            jmp     while_loop              ; Infinite while loop

;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
            
            
