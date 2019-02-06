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
;   MSP430x11x1 Demo - Timer_A, Output 40kHz Square Wave, Up Mode, DCO SMCLK
;
;   Description: Outputs a 40kHz square wave on P1.2 using Timer_A. The DCO
;   is first .set to 2.56MHz using "Set_DCO" software FLL. Timer_A is then
;   configured for up mode operation. CCR0 defines the period and CCR1 the
;   duty cycle which is 50%. CCR1 is output on P1.2. The 40kHz square
;   wave is generated in hardware and does not require any CPU resources.
;   MCLK = SMCLK = TACLK = DCOCLK = 2.56MHz
;   ;* An external watch crystal on XIN XOUT is required for ACLK *//	
;
Delta       .equ    625                     ; Delta = (target DCO)/(32768/8)
;
;                MSP430F1121
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |         P1.2/TA1|--> 40kHz
;
;   M. Buccini / Z. Albus
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;*******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize stackpointer
SetupWDT    mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupBC     bis.b   #DIVA_3,&BCSCTL1        ; ACLK = LFXT1CLK/8
            call    #Set_DCO                ; Set DCO clock
SetupP1     bis.b   #004h,&P1DIR            ; P1.2 output
            bis.b   #004h,&P1SEL            ; P1.2/TA1 port function
SetupC0     mov.w   #64-1,&CCR0             ; PWM Period
SetupC1     mov.w   #OUTMOD2+OUTMOD1+OUTMOD0,&CCTL1; CCR1 Reset\Set
            mov.w   #32,&CCR1               ; CCR1 PWM Duty Cycle	
SetupTA     mov.w   #TASSEL_2+MC_1+TACLR,&TACTL; SMCLK, Clear TAR, upmode
                                            ;				
Mainloop    bis.w   #CPUOFF,SR              ; CPU not required
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
Set_DCO;    Subroutine: Sets DCO to selected frequency based on Delta.
;           R14 and R15 are used, ACLK= 32768/8 Timer_A clocked by DCOCLK
;-------------------------------------------------------------------------------
            clr.w   R15                     ;							
Setup_CC2   mov.w   #CM_1+CCIS_1+CAP,&CCTL2 ; CAP, ACLK
Setup_TA    mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK, Continous Mode
Test_DCO    bit.w   #CCIFG,&CCTL2           ; Test capture flag
            jz      Test_DCO                ;
            bic.w   #CCIFG,&CCTL2           ; Clear capture flag
                                            ;
AdjDCO      mov.w   &CCR2,R14               ; R14 = captured SMCLK
            sub.w   R15,R14                 ; R14 = capture difference
            mov.w   &CCR2,R15               ; R15 = captured SMCLK
            cmp.w   #Delta,R14              ; Delta = SMCLK/(32768/8)
            jlo     IncDCO                  ;
            jeq     DoneDCO                 ;
DecDCO      dec.b   &DCOCTL                 ; Slow DCO with DCO and MOD
            jc      Test_DCO                ; Slower?
            cmp.b   #XT2OFF+DIVA_3,&BCSCTL1 ; Can RSELx be decremented
            jz      Test_DCO
            dec.b   &BCSCTL1                ; Decrement RSEL.x
            jmp     Test_DCO                ;
IncDCO      inc.b   &DCOCTL                 ; Speed DCO with DCO and MOD
            jnc     Test_DCO                ; Faster?
            cmp.b   #XT2OFF+DIVA_3+07h,&BCSCTL1; Can RSEL.x be increased?
            jz      Test_DCO
            inc.b   &BCSCTL1                ; Increment RSEL.x
            jmp     Test_DCO                ;
DoneDCO     clr.w   &CCTL2                  ; Stop CCR2
            clr.w   &TACTL                  ; Stop timer_A
            ret                             ; Return from subroutine
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end

