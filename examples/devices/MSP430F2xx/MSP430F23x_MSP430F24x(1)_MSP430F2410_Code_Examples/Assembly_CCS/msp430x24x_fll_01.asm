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
;  MSP430x24x Demo - Basic Clock, Implement Auto RSEL SW FLL
;
;  Description: Set DCO clock to (Delta)*(4096) using software FLL. DCO clock
;  is output on P1.4 as SMCLK.  DCO clock, which is the selected SMCLK source
;  for timer_A is integrated over LFXT1/8 (4096) until SMCLK is equal to Delta
;  CCR2 captures ACLK.  To use Set_DCO Timer_A must be operating in continous
;  mode.  Watch crystal for ACLK is required for this example.  Delta must be
;  kept in a range that allows possible DCO speeds.  Minimum Delta must ensure
;  that Set_DCO loop can complete within capture interval. Maximum delta can be 
;  calculated by f(DCOx7) / 4096.  f(DCOx7) can be found in device specific
;  datasheet. ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = target DCO
;  //* External watch crystal installed on XIN XOUT is required for ACLK *//
;
;           MSP430F49
;         ---------------
;     /|\|            XIN|-
;      | |               | 32kHz
;      --|RST        XOUT|-
;        |               |
;        |           P1.4|--> SMLCK = target DCO
;
;  JL Bile
;  Texas Instruments Inc.
;  May 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
;Delta       .equ     2930   ; target DCO = DELTA*(4096) = 12MHz
;Delta       .equ     900    ; target DCO = Delta*(4096) = 3686400
Delta       .equ    600    ; target DCO = Delta*(4096) = 2457600
;Delta       .equ    250    ; target DCO = Delta*(4096) = 1024000
;Delta       .equ    078    ; target DCO = Delta*(4096) =  319488
;Delta       .equ    070    ; target DCO = Delta*(4096) =  286720
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0500h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            push.w  #0ffffh                 ; LFXT1 startup delay
OscDelay    dec.w   0(SP)                   ; Do calc on the stack
            jnz     OscDelay                ;
            incd.w  SP                      ; Correct SP
SetupP1     bis.b   #010h,&P1DIR            ; P1.4 output
            bis.b   #010h,&P1SEL            ; P1.4 SMCLK output
            call    #Set_DCO                ; Set DCO clock
                                            ;
Mainloop    bis.w   #CPUOFF+GIE,SR          ; CPU is not required
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
Set_DCO;    Subroutine: Sets DCO to selected frequency based on Delta.
;           R13, R14, and R15 are used, ACLK= 32768/8 Timer_A clocked by DCOCLK
;-------------------------------------------------------------------------------
            bis.b   #DIVA_3,&BCSCTL1        ; ACLK = LFXT1CLK/8
            clr.w   R15                     ;
Setup_CC2   mov.w   #CM_1+CCIS_1+CAP,&TACCTL2  ; CAP, ACLK
Setup_TA    mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK, Continous Mode
Test_DCO    bit.w   #CCIFG,&TACCTL2         ; Test capture flag
            jz      Test_DCO                ;
            bic.w   #CCIFG,&TACCTL2         ; Clear capture flag
                                            ;
AdjDCO      mov.w   &TACCR2,R14             ; R14 = captured SMCLK
            sub.w   R15,R14                 ; R14 = capture difference
            mov.w   &TACCR2,R15             ; R15 = captured SMCLK
            cmp.w   #Delta,R14              ; Delta = SMCLK/(32768/4)
            jlo     IncDCO                  ;
            jeq     DoneDCO                 ;
DecDCO      dec.b   &DCOCTL                 ; Slow DCO with DCO and MOD
            jc      Test_DCO                ; Slower?
            bit.b   #00fh,&BCSCTL1          ; Can RSELx be decremented
            jnc     Test_DCO                ; No
            dec.b   &BCSCTL1                ; Decrement RSEL.x
            jmp     Test_DCO                ;
IncDCO      inc.b   &DCOCTL                 ; Speed DCO with DCO and MOD
            jnc     Test_DCO                ; Faster?
            mov.b   &BCSCTL1,R13            ; Can RSEL.x be increased?
            and.b   #00fh,R13               ;
            cmp.b   #00fh,R13               ;
            jz      Test_DCO                ; No
            inc.b   &BCSCTL1                ; Increment RSEL.x
            jmp     Test_DCO                ;
DoneDCO     clr.w   &TACCTL2                ; Stop TACCR2
            clr.w   &TACTL                  ; Stop Timer_A
            ret                             ; Return from subroutine
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	  ".reset"            ; POR, ext. Reset
            .short	RESET
            .end
