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
; Comparator in Non Compensated Mode - An input signal on A0 is compared with 
; an input signal on A2.  The signal present on A2 uses the input divider of 
; the module. The Results of the Comparator will be present on P1.3
;
;                                     MSP430 L092
;                                   -----------------
;                               /|\|              XIN|-
;                                | |                 |
;                                --|RST          XOUT|-
;                                  |             P1.4|<--Comparator Input A0 
;           Comparator Input A2--> | P1.0        P1.3|-->Comparator Output
;
;  D.Dang/D.Archbold/D.Szmulewicz
;  Texas Instruments Inc.
;  Built with CCS version 4.2.0
;******************************************************************************

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .global main
            .text                           ; Assemble to memory
;-------------------------------------------------------------------------------
main        mov.w   #WDTPW + WDTHOLD,&WDTCTL ; Stop WDT  
             bis.w     #0x3000,&SYSCNF          ; debug issue
  
  ;************************ 
  ; Setup CCS               
  ; MCLK = SMCLK = HFCLK/1  
  ;************************ 

  
            mov.w    #CCSKEY,&CCSCTL0       ; Unlock CCS

check       mov.w    #0,&CCSCTL7            ; Clear HF & LF OSC fault flags
            mov.w    #0,&SFRIFG1            ; Clear OFIFG
            bit.w    #OFIFG,&SFRIFG1        ; Oscillator Flag(s)?
            jnz      check             
            
            mov.w   #DIVM_0 + DIVS_0,&CCSCTL5 ; Set the Dividers for  MCLK, and SMCLK to 1
            bis.w      #SELM_0 + SELS_0,&CCSCTL4 ; Select HFCLK as the source for MCLK, and SMCLK                 
            bis.b   #0xFF,&CCSCTL0_H        ;  Lock CCS  

  
  ;************************ 
  ; Setup Port 1            
  ; Output P1.3             
  ;************************ 
                   
            bis.b   #BIT3,&P1DIR
            bis.b   #BIT3,&P1SEL0
            bis.b   #BIT3,&P1SEL1
  
  ;************************ 
  ; Configure APOOL         
  ;************************ 
  
            mov.w   #0,&APIFG               ; Clear all APIFG 
            mov.w   #CLKSEL_2,&APCNF         
            mov.w   #0,&APIFG  

            mov.w   #APPSEL_0 + APNSEL_2,&APCTL ; A0=P vs A2=N 
            bis.w   #A2DIV_2,&APVDIV        ; A2 divide by 8
            mov.w   #CTEN,&APOMR   
            bis.w   #CMPON,&APCNF               ;If A2/8 < A0, P1.3 = 1, else P1.3 = 0
            nop
 
loop        jmp      loop  

            .end

