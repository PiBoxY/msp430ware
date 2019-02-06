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
;  MSP430F530x Demo - FLL|, Runs Internal DCO at 2.45MHz
;
;  Description: This program demonstrates setting the internal DCO to run at
;  2.45MHz with auto-calibration by the FLL| circuitry. It uses LFXT1 as FLL
;  reference.
;  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = (74+1) x REFO = 2457600Hz
;
;                MSP430F530x
;             -----------------
;         /|\|                 |
;          | |             P1.0|-->ACLK
;          --|RST          P4.7|-->MCLK
;            |                 |
;            |                 |
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
            mov.w   #WDTPW | WDTHOLD,&WDTCTL; Stop WDT

            mov.w   #0x02D52,&PMAPPWD       ; Enable Write-access to modify port
                                            ; mapping registers
            mov.b   #PM_MCLK,&P4MAP7        ;
            mov.w   #0,&PMAPPWD             ; Disable Write-Access to modify
                                            ; port mapping registers

            bis.b   #BIT0,&P1DIR            ; ACLK set out to pins
            bis.b   #BIT0,&P1SEL            ;
            bis.b   #BIT7,&P4DIR            ; MCLK set out to pins
            bis.b   #BIT7,&P4SEL            ;

; Initialize LFXT1
            bis.b   #BIT4|BIT5,&P5SEL       ; Select XT1
            bic.w   #XT1OFF,&UCSCTL6        ; XT1 On
            bis.w   #XCAP_3,&UCSCTL6        ; Internal load cap

; Loop until XT1 fault flag is cleared
do_while1   bic.w   #XT1LFOFFG,&UCSCTL7     ; Clear XT1 fault flags
            bit.w   #XT1LFOFFG,&UCSCTL7     ; Test XT1 fault flag
            jc      do_while1

; Initialize DCO to 2.45MHz
            bis.w   #SCG0,SR                ; Disable the FLL control loop
            clr.w   &UCSCTL0                ; Set lowest possible DCOx, MODx
            mov.w   #DCORSEL_3,&UCSCTL1     ; Set RSELx for DCO = 4.9 MHz
            mov.w   #FLLD_1 | 74,&UCSCTL2   ; Set DCO Multiplier for 2.45MHz
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

; Loop until XT1,XT2 & DCO fault flag is cleared
do_while2   bic.w   #XT2OFFG | XT1LFOFFG  | DCOFFG,&UCSCTL7
                                            ; Clear XT2,XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jc      do_while2

while_loop  jmp     while_loop              ; Loop in place
            nop                             ; To workaround CPU40

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
