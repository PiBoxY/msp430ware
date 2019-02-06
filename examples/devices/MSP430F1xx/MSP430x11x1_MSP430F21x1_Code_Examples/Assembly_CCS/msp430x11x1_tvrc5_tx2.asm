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
;   MSP430x11x1 Demo - Encode RC5 IR Remote Control, No XTAL, Rosc
;
;   Description: TX an RC5 packet from IRData. The packet is TXed every eight
;   seconds.  Power on command is sent first, followed by Channel + in an
;   endless loop. Timer_A CCR2 and CCR0 in upmode are used to generate 40kHz
;   modulation. CCR0_ISR generates 25us ISR and is used for timing of RC5 bit
;   length periods. The WDT_ISR is used as a wake-up from LPM0 only to simulate
;   a wake up from a keypad - normal keypad wake up can come from LPM4.
;   P1.0 is set at start of IRTX and cleared at the end.
;   Using a 100k resistor on ROSC, with default DCO setting, set DCOCLK ~2MHz.
;   ACLK = n/a, MCLK = SMCLK ~2MHz DCOCLK
;   ;* Use of external resistor reduces temperature sensitivity of DCOCLK *//
;
;                  MSP430F1121
;               -----------------
;     /|\   /|\|              XIN|-
;      |     | |                 |
;     100k   --|RST          XOUT|-
;      |       |                 |
;      +-------|P2.5/ROSC        |
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
;>>> only required to simulate keypad input
SetupWDT    mov.w   #WDT_MDLY_32,&WDTCTL    ; WDT ~30ms interval timer
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
            clr.b   R15
;<<<
SetupBC     bis.b   #DCOR,&BCSCTL2          ; Rosc
            bis.w   #OSCOFF,SR              ; XTAL not used
SetupP1     bic.b   #004h,&P1OUT            ; P1.2 = 0
            bis.b   #004h,&P1DIR            ; P1.2 output
            bis.b   #01h,&P1DIR             ; P1.0 = Output
            eint                            ;													
                                            ;													
Mainloop    mov.w   #000Ch,IRData           ; RC5 TV1 Power on command
            call    #IRTX_Ready             ;
Chan_UP     bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupts
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
TA0_ISR;    Decrement 25us counters
;-------------------------------------------------------------------------------
            dec.w   IRCnt                   ;
            reti                            ;		

;-------------------------------------------------------------------------------
WDT_ISR;    Exit LPMX
;-------------------------------------------------------------------------------
            dec.b   R15
            jnz     WDT_ISR_1
            bic.w   #SCG1+SCG0+CPUOFF,0(SP) ; Exit LPM4 with enabled interrupt
WDT_ISR_1   reti                            ;		
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
