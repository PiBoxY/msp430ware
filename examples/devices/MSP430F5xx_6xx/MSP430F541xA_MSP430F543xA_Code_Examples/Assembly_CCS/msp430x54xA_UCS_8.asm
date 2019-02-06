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
;  MSP430F54xA Demo - XT2 sources MCLK & SMCLK
;
;  Description: This program demonstrates using XT2 to source MCLK. XT1 is not
;  connected in this case.
;
;  By default, LFXT1 is requested by the following modules:
;     - FLL
;     - ACLK
;  If LFXT1 is NOT used and if the user does not change the source modules,
;  it causes the XT1xxOFIFG flag to be set because it is constantly looking
;  for LFXT1. OFIFG, global oscillator fault flag, will always be set if LFXT1
;  is set. Hence, it is important to ensure LFXT1 is no longer being sourced
;  if LFXT1 is NOT used.
;  MCLK = XT2
;  PMMCOREV = 2 to support up to 20MHz clock
;
;  NOTE: if the SMCLK/HF XTAL frequency exceeds 8MHz, VCore must be set
;  accordingly to support the system speed. Refer to MSP430x5xx Family User's Guide
;  Section 2.2 for more information.

;               MSP430F5438A
;             -----------------
;        /|\ |                 |
;         |  |                 |
;         ---|RST              |
;            |            XT2IN|-
;            |                 | HF XTAL (455kHz - 16MHz)
;            |           XT2OUT|-
;            |                 |
;            |            P11.1|--> MCLK = XT2
;            |                 |
;
;  	Note: 
;      	In order to run the system at up to 20MHz, VCore must be set at 1.8V 
;		or higher. This is done by invoking function SetVCore().
;
;   F. Chen
;   Texas Instruments Inc.
;   December 2012
;   Built with CCS Version: 5.2.1 
;******************************************************************************

    .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
	
count    .equ    R4 
;-------------------------------------------------------------------------------
            .global _main 
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x5C00,SP              ; Initialize stackpointer
            mov.w   #WDTPW + WDTHOLD,&WDTCTL; Stop WDT
            mov.w	#PMMCOREV_2,R12			; Set VCore to 1.8V to support up to 20MHz clock
            calla	#SetVCore				; call SetVCore subroutine from hal_pmm.c
            
            mov.b   #BIT1 + BIT2,&P11DIR    ; P11.1-2 to output direction
            bis.b   #BIT1 + BIT2,&P11SEL    ; P11.1-2 to output SMCLK,MCLK
            bis.b   #0x0C,&P5SEL            ; Port select XT2

            bic.w   #XT2OFF,&UCSCTL6        ; Clear XT2OFF bit
            bis.w   #SELREF_2,&UCSCTL3      ; FLLref = REFO
                                            ; Since LFXT1 is not used,
                                            ; sourcing FLL with LFXT1 can cause
                                            ; XT1OFFG flag to set
            bis.w   #SELA_2,&UCSCTL4        ; ACLK=REFO,SMCLK=DCO,MCLK=DCO

            ; Loop until XT1,XT2 & DCO stabilizes
do_while    bic.w   #XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG,&UCSCTL7
                                            ; Clear XT2,XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jc      do_while

            bic.w   #XT2DRIVE0,&UCSCTL6     ; Decrease XT2 Drive according to
                                            ; expected frequency
            bis.w  #SELS_5 + SELM_5,&UCSCTL4; SMCLK=MCLK=XT2

while_loop  jmp     while_loop

;-------------------------------------------------------------------------------
SetVCore   ; SetVCore function
           ; In order to support high speed MCLK , The voltage level of Vcore  must be sufficiently high
           ; The voltage level of Vcore must be increased by one step one time
;-------------------------------------------------------------------------------
            mov.b   #PMMPW_H, &PMMCTL0_H     ;Open PMM registers for write
   ;  Set SVS/SVM high side new level
            mov.w   R12,R15                 ; R12--->R15
			and.w   #0xff,R15
            swpb    R15                     ;exchange the high and low byte of R15
            add.w   R12,R15                 ;add src to dst src+dst--->dst
            add.w   #0x4400,R15             ;SVM high-side enable ,SVS high-side enable
            mov.w   R15,&SVSMHCTL            ;
   ;  Set SVM low side to new level
			mov.w   R12,R15
			add.w   #0x4400,R15
			mov.w   R15,&SVSMLCTL
   ; Wait till SVM is settled
do_while1   bit.w   #SVSMLDLYIFG,&PMMIFG     ; Test SVSMLDLYIFG
			jz     do_while1
   ; Clear already set flags
            bic.w   #SVMLIFG,&PMMIFG         ;clear SVM low-side interrupt flag
            bic.w   #SVMLVLRIFG,&PMMIFG      ;clear  SVM low-side voltage level reached interrupt flag
   ; Set VCore to new level
            mov.b   R12,&PMMCTL0_L
   ; Wait till new level reached
            bit.w   #SVMLIFG,&PMMIFG
            jz     low_set
do_while2   bit.w   #SVMLVLRIFG,&PMMIFG      ; Test SVMLvLrIFG
			jz     do_while2
    ;Set SVS/SVM low side to new level
low_set     mov.w   R12,R15
            and.w   #0xff,R15
            swpb    R15
            add.w   R15,R12
            add.w   #0x4400,R12
            mov.w   R12,&SVSMLCTL
	;Lock PMM registers for write access
			clr.b   &PMMCTL0_H
            RETA

;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
            
            
