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
;   MSP430x11x1 Demo - Encode RC5 IR Remote Control, 32kHz ACLK
;
;   Description: TX an RC5 packet from IRData. The packet is TXed every eight
;   seconds.  Power on command is sent first, followed by Channel + in an
;   endless loop. Timer_A CCR2 and CCR0 in upmode are used to generate 40kHz
;   modulation. CCR0_ISR generates 25us ISR and is used for timing of RC5 bit
;   length periods. The WDT_ISR is used for the eight second wake-up interval.
;   P1.0 is set at start of IRTX and cleared at the end.
;   The Set_DCO subroutine will calibrate the  DCOCLK to ~2Mhz.
;   ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = TACLK = target DCO
;   ;* External watch crystal installed on XIN XOUT is required for ACLK *//	
;
;		   MSP430F1121
;               -----------------
;           /|\|              XIN|-
;            | |                 | 32k
;            --|RST          XOUT|-
;              |                 |
;              |             P1.0|-->LED
;              |             P1.2|-->IR LED			
;
;
;   S1 S2 C  A4 A3 A2 A1 A0 C5 C4 C3 C2 C1 C0
;
;   2 Start, C and 12-bits of data are received MSB first
;
;   Bit pattern as sent at MSP430
;
;                 1.78ms
;                 ----     ----  ----
;                 |  |     |  |  |  |
;               --+  ---+---  +---  +--
;                 ^  0  ^ 1   ^  1  ^
;
;   CPU Registers Used
IRData  .equ     R4
IRBit   .equ     R5
IRCnt   .equ     R6
;
Delta       .equ    488                     ; Delta = (target DCO)/(32768/8)
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
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupBC     bis.b   #DIVA_3,&BCSCTL1        ; ACLK = LFXT1/8
            call    #Set_DCO                ; Set DCO clock
SetupP1     bic.b   #004h,&P1OUT            ; P1.2 = 0
            bis.b   #004h,&P1DIR            ; P1.2 output
            bis.b   #01h,&P1DIR             ; P1.0 = Output
SetupWDT    mov.w   #WDT_ADLY_1000,&WDTCTL  ; WDT 1000ms, ACLK, interval timer
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
            eint                            ;													

Mainloop    mov.w   #000Ch,IRData           ; RC5 TV1 Power on command
            call    #IRTX_Ready             ;
Chan_UP     bis.w   #LPM3+GIE,SR            ; Enter LPM3, w/ interrupts
;            mov.w   #080Ch,IRData           ; RC5 TV1 Power off command
            mov.w   #0020h,IRData           ; RC5 TV1 Chan+ command
            call    #IRTX_Ready             ;
            jmp     Chan_UP                 ;
                                            ;
;-------------------------------------------------------------------------------
IRTX_Ready;  Subroutine
;-------------------------------------------------------------------------------
IRTX_RC5    bis.b   #01h,&P1OUT             ; LED on
SetupC0     mov.w   #CCIE,&CCTL0            ; Interrupt
            mov.w   #50-1,&CCR0             ; 40MHz Period @ 2MHz
SetupC1     mov.w   #OUTMOD_7,&CCTL1        ; CCR1 Reset\Set
            mov.w   #25,&CCR1               ; CCR1 Duty Cycle	
SetupTA     mov.w   #TASSEL_2+MC_1,&TACTL   ; SMCLK, upmode
            bis.w   #03000h,IRData          ; Start bits
            rlc.w   IRData                  ;
            rlc.w   IRData                  ;
            mov.w   #14,IRBit               ; 2 Start, C + 12-bits data
                                            ;
IRTX_Bit    rlc.w   IRData                  ; C = Databit
            jnc     IRTX_0                  ; Jump --> C = 0
                                            ;
IRTX_1      bic.b   #004h,&P1SEL            ; IR 40kHz off
            bic.b   #004h,&P1OUT            ; IR off - safe
            call    #D_889us                ;
            bis.b   #004h,&P1SEL            ; IR 40kHz on
            call    #D_889us                ;
            jmp     IRTX_Tst                ;
                                            ;
IRTX_0      bis.b   #004h,&P1SEL            ; IR 40kHz on
            call    #D_889us                ;
            bic.b   #004h,&P1SEL            ; IR 40kHz off
            bic.b   #004h,&P1OUT            ; IR off - safe
            call    #D_889us                ;
IRTX_Tst    dec.w   IRBit                   ;
            jnz     IRTX_Bit                ;
                                            ;
            bic.b   #004h,&P1SEL            ; IR 40kHz off
            bic.b   #004h,&P1OUT            ; IR off - safe
            clr.w   &TACTL                  ; Stop Timer_A
            bic.b   #01h,&P1OUT             ; LED off
            ret                             ;
                                            ;
D_889us     add.w   #35,IRCnt               ;
D2          tst.w   IRCnt                   ;
            jnz     D2                      ;
            ret                             ;
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
TA0_ISR;    Decrement 25us counters
;-------------------------------------------------------------------------------
            dec.w   IRCnt                   ;
            reti                            ;		

;-------------------------------------------------------------------------------
WDT_ISR;    Exit LPMX
;-------------------------------------------------------------------------------
            mov.w   #GIE,0(SP)              ; Exit LPMx with enabled interrupt
            reti                            ;		
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int10"                ; WDT Vector
            .short  WDT_ISR                 ;
            .sect   ".int09"                ; Timer_A0 Vector
            .short  TA0_ISR                 ;
            .end
