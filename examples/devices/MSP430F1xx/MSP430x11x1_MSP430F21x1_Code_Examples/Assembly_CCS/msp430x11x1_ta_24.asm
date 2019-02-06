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
;   MSP430x11x1 Demo - Timer_A, Ultra-Low Pwr 1800Hz Detect, SMCLK +/- 1%
;
;   Description: Using Timer_A capture capability. At 128ms intervals,
;   CCR0 is enabled on P2.2 to detect presence or absence of 1800Hz. If
;   1800Hz is present, P1.0 is set, if not, reset. Software FLL is use to .set
;   DCO (SMCLK) to 1MHz. WDT is used for 128ms second wake-up to hunt for
;   tone. System runs normally in LPM3. Watch crystal is required for ACLK.
;   ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = target DCO = 921600kHz
;   ;* External watch crystal installed on XIN XOUT is required for ACLK *//	
;
;        1800Hz  = 512 SMCLK clocks
;        1814Hz  = 508 SMCLK clocks
;        1786Hz  = 516 SMCLK clock
;
;                 MSP430F1121
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;    Signal->|P2.2/CCR0    P1.0|-->LED
;            |                 |			
;
;
;           CPU registers used
Period  .equ     R15
OldCap  .equ     R14
;
;            User definitions
Delta       .equ    225                     ; Target DCO Clock/(32768/8)
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
RESET       mov.w   #300h,SP		        ; Initialize stackpointer
SetupBC     bis.b   #DIVA1+DIVA0,&BCSCTL1   ; ACLK=LFXT1CLK/8
SetupWDT    mov.w   #WDT_ADLY_16,&WDTCTL    ; WDT 16ms*8 Interval Timer
            bis.b   #WDTIE,&IE1             ; Enable WDT Interrupt
SetupP1     mov.b   #0FFh,&P1DIR            ; All P1.x Outputs
            clr.b   &P1OUT                  ; All P1.x Reset
SetupP2     mov.b   #0FBh,&P2DIR            ; P2.2 Input / All P2.x Output
            bis.b   #004h,&P2SEL            ; P2.2 CCR0 input
            clr.b   &P2OUT                  ; All P2.x Reset
            call    #Set_DCO                ; St DCO to target frequency
                                            ;								
Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3 w/ interrupts
            call    #Get_CCR0               ; Capture CCR0 on P2.2
            cmp.w   #516,Period             ;
            jhs     OFF                     ;
            cmp.w   #508,Period             ;
            jlo     OFF                     ;
                                            ;
ON          bis.b   #001h,&P1OUT            ; P1.0 = 1
            jmp     Mainloop                ;
OFF         bic.b   #001h,&P1OUT            ; P1.0 = 0
            jmp     Mainloop                ;
                                            ;
;-------------------------------------------------------------------------------
Get_CCR0; Subroutine to get CCR0 Capture
;-------------------------------------------------------------------------------
            clr.w   Period                  ;
            clr.w   OldCap                  ;
            mov.w   #TASSEL1+TACLR,&TACTL   ; SMCLK, TAR cleared
SetupC0     mov.w   #CCIS0+CM0+CAP+CCIE+SCS,&CCTL0; CCI0B, CAP, interrupt
            bis.w   #MC1,&TACTL             ; Start timer in Ccntinous mode
Test_1      cmp.w   #2048,&TAR              ; Timeout
            jlo     Test_1                  ; jump if 02048 > &TAR
            clr.w   &TACTL                  ; Stop Timer_A
            clr.w   &CCTL0                  ; Stop CCTL0
            ret                             ;
                                            ;
;-------------------------------------------------------------------------------
Set_DCO;    Subroutine: Sets DCO to selected frequency based on Delta.
;           R14 and R15 are used, ACLK= 32768/8 Timer_A clocked by DCOCLK
;-------------------------------------------------------------------------------
            clr.w   R15                     ;							
Setup_TA    mov.w   #TASSEL1+TACLR,&TACTL   ; SMCLK
Setup_CC2   mov.w   #CCIS0+CM0+CAP,&CCTL2   ; Define CCR2,CAP,ACLK									
            bis.w   #MC1,&TACTL             ; Start timer_A: Continous Mode
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
            jz      DoneDCO                 ; jmp>DCO at slowest setting
            dec.b   &BCSCTL1                ; Decrement RSEL.x
            jmp     Test_DCO                ;
IncDCO      inc.b   &DCOCTL                 ; Speed DCO with DCO and MOD
            jnc     Test_DCO                ; Faster?
            cmp.b   #XT2OFF+DIVA_3+07h,&BCSCTL1; Can RSEL.x be increased?
            jz      DoneDCO                 ; jmp> DCO at fastest settting
            inc.b   &BCSCTL1                ; Increment RSEL.x
            jmp     Test_DCO                ;
DoneDCO     clr.w   &CCTL2                  ; Stop CCR2
            clr.w   &TACTL                  ; Stop timer_A
            ret                             ; Return from subroutine
                                            ;
;-------------------------------------------------------------------------------
TA0_ISR;    Timer_A C0 ISR
;-------------------------------------------------------------------------------
            push.w  &CCR0                   ; Captured TAR, rising edge
            mov.w   @SP,Period              ;
            sub.w   OldCap,Period           ; New - old = period
            pop.w   OldCap                  ; For next calculation
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
WDT_ISR;    Exit LPM3 on reti
;-------------------------------------------------------------------------------
            bic.w    #LPM3,0(SP)            ; Clear LPM3 from TOS
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
