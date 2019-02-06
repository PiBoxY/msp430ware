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
;   MSP430x261x Demo - Timer_B, Toggle P4.0-6, Cont. Mode ISR, DCO SMCLK
;
;   Description:  Use Timer_B CCRx units and overflow to generate eight
;   independent timing intervals.  For demonstration, TBCCR0-6 output
;   units are optionally selected with P4 in toggle mode.
;   As such, these pins will toggle when respective TBCCRx registers
;   match the TBR counter.  Interrupts are also enabled with all TBCCRx units,
;   software only adds offset to next interval - as long as the interval
;   offset is aded to TBCCRx, toggle rate is generated in hardware.  Timer_B
;   overflow is used to toggle P1.0 with software. Proper use of the TBIV
;   interrupt vector generator is demonstrated.
;   MCLK = SMCLK = TBCLK = DCO 8MHz
;
;   As coded with TBCLK 8MHz DCO, toggle rates are:
;   P4.0 = TBCCR0 = 8000000/(2*200) = 20KHz
;   P4.1 = TBCCR1 = 8000000/(2*400) = 10KHz
;   P4.2 = TBCCR2 = 8000000/(2*600) = 6.67KHz
;   P4.3 = TBCCR3 = 8000000/(2*800) = 5KHz
;   P4.4 = TBCCR4 = 8000000/(2*1000) = 4KHz
;   P4.5 = TBCCR5 = 8000000/(2*2000) = 2KHz
;   P4.6 = TBCCR6 = 8000000/(2*10000) = 400Hz
;   P1.0 = overflow = 8000000/(2*65536) = 61Hz
;
;                MSP430F241x
;                MSP430F261x
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |         P4.0/TB0|--> TBCCR0
;            |         P4.1/TB1|--> TBCCR1
;            |         P4.2/TB2|--> TBCCR2
;            |         P4.3/TB3|--> TBCCR3
;            |         P4.4/TB4|--> TBCCR4
;            |         P4.5/TB5|--> TBCCR5
;            |         P4.6/TB6|--> TBCCR6
;            |             P1.0|--> Overflow/software
;
;  JL Bile
;  Texas Instruments Inc.
;  June 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text   ;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0850h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
CheckCal    cmp.b   #0xFF,&CALBC1_8MHZ      ; Check calibration constant
            jne     Load                    ; if not erased, load.   
Trap        jmp     Trap                    ; if erased do not load, trap CPU!
Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   &CALBC1_8MHZ,&BCSCTL1   ; Set DCO to 8MHz
            mov.b   &CALDCO_8MHZ,&DCOCTL    ; 

SetupP2     bis.b   #7Fh,&P4SEL             ; P2 option select
            bis.b   #7Fh,&P4DIR             ; P2 outputs
SetupP1     bis.b   #BIT0,&P1DIR            ; P1.0 = output

SetupCx     mov.w   #OUTMOD_4+CCIE,&TBCCTL0 ; TBCCR0 interrupt enabled
            mov.w   #OUTMOD_4+CCIE,&TBCCTL1 ; TBCCR1 interrupt enabled
            mov.w   #OUTMOD_4+CCIE,&TBCCTL2 ; TBCCR2 interrupt enabled
            mov.w   #OUTMOD_4+CCIE,&TBCCTL3 ; TBCCR3 interrupt enabled
            mov.w   #OUTMOD_4+CCIE,&TBCCTL4 ; TBCCR4 interrupt enabled
            mov.w   #OUTMOD_4+CCIE,&TBCCTL5 ; TBCCR5 interrupt enabled
            mov.w   #OUTMOD_4+CCIE,&TBCCTL6 ; TBCCR6 interrupt enabled
SetupTB     mov.w   #TBSSEL_2+MC_2+TBIE,&TBCTL ; SMCLK, contmode, interrupt
                                            ;                       
Mainloop    bis.w   #CPUOFF+GIE,SR          ; CPU off, interrupts enalbled
            nop                             ; Required for debugger
                                            ;
;------------------------------------------------------------------------------
TB0_ISR;    Toggle P1.0
;------------------------------------------------------------------------------
            add.w   #200,&TBCCR0            ; Offset until next interrupt
            reti                            ;       
;
;------------------------------------------------------------------------------
TBX_ISR;    Common ISR for TBCCR1-6 and overflow
;------------------------------------------------------------------------------
            add.w   &TBIV,PC                ; Add Timer_B offset vector
            reti                            ; CCR0 - no source
            jmp     TBCCR1_ISR              ; TBCCR1
            jmp     TBCCR2_ISR              ; TBCCR2
            jmp     TBCCR3_ISR              ; TBCCR3
            jmp     TBCCR4_ISR              ; TBCCR4
            jmp     TBCCR5_ISR              ; TBCCR5
            jmp     TBCCR6_ISR              ; TBCCR6
TB_over     bic.w   #TBIFG, &TBCTL
            xor.b   #BIT0,&P1OUT            ; Toggle P1.0
            reti                            ; Return from overflow ISR      
                                            ;
TBCCR1_ISR  bic.w   #CCIFG, &TBCCTL1
            add.w   #400,&TBCCR1            ; Offset until next interrupt
            jmp     TBX_ISR                 ;
                                            ;
TBCCR2_ISR  bic.w   #CCIFG, &TBCCTL2
            add.w   #600,&TBCCR2            ; Offset until next interrupt
            jmp     TBX_ISR                 ;
                                            ;
TBCCR3_ISR  bic.w   #CCIFG, &TBCCTL3
            add.w   #800,&TBCCR3            ; Offset until next interrupt
            jmp     TBX_ISR                 ;
                                            ;
TBCCR4_ISR  bic.w   #CCIFG, &TBCCTL4
            add.w   #1000,&TBCCR4           ; Offset until next interrupt
            jmp     TBX_ISR                 ;
                                            ;
TBCCR5_ISR  bic.w   #CCIFG, &TBCCTL5
            add.w   #2000,&TBCCR5           ; Offset until next interrupt
            jmp     TBX_ISR                 ;
                                            ;
TBCCR6_ISR  bic.w   #CCIFG, &TBCCTL6
            add.w   #10000,&TBCCR6          ; Offset until next interrupt
            jmp     TBX_ISR                 ;
                                            ;

;------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"               ; MSP430 RESET Vector
            .short  RESET                  ;
            .sect   ".int29"                ; Timer_B0 Vector
            .short  TB0_ISR                 ;
            .sect   ".int28"                ; Timer_BX Vector
            .short  TBX_ISR                 
            .end
            
