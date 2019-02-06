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
;   MSP-FET430P430 Demo - LCD, Display "430" on Varitronix VI-322 LCD
;
;   Description: Displays "430" on Varitronix VI-322 LCD as used on Softbaugh D437
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   //* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;               MSP430FG439
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
;         --|RST          XOUT|-
;           |                 |      Varitronix VI-322
;           |                 |     (as used on Softbaugh D437 board)
;           |             S0  |     ---------
;           |              -  |--> | + 3 2 1 |
;           |             S31 |     ---------
;           |             COM0|-----|
;           |                 |
;           |                 |
;           |                 |
;           |                 |
;
;
;   L. Westlund / M. Mitchell
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                  ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

SetupFLL2   bis.b   #XCAP18PF,&FLL_CTL0     ; Set load capacitance for xtal

SetupLCD    mov.b   #LCDON+LCDSTATIC+LCDOGOFF,&LCDCTL ;static LCD,
                                                      ;segments = 0 - 31
SetupBT     mov.b   #BTFRFQ1+BTFRFQ0,&BTCTL ; LCD freq

            mov.b   #0xFC, &P5SEL           ; Common and Rxx all selected
                                            ; For P5.0 P5.1 P4.6 P4.7 setting
                                            ; SEL bits selects Analog for FG

ClearLCD    mov.w   #18,R15                 ; 18 LCD memory bytes to clear
Clear1      mov.b   #0,LCDM1(R15)           ; write zeros in LCD RAM locations
            dec.w   R15                     ; all LCD mem clear?
            jc      Clear1                  ; more LCD mem to clear, use JC

                                            ; R15  Digits to display
                                            ; R14   LCD Table data for digit
Display     mov.w   #0x430, R15             ; move data to display
            mov.w   R15, R12                ; preserve data
            and.w   #0x0F, R12              ; get 1st digit
            mov.w   LCD_Tab(R12), R14       ; lookup display data in table
            bit.w   #0x01, R12              ; test for odd digit
            jz      EVEN_1                  ; if not, do nothing
            swpb    r14                     ; else, swap bytes to get high
                                            ; byte in table location
EVEN_1      mov.b   R14, &LCDM1             ; shift in data to LCD memory...
            rra.w   R14
            mov.b   R14, &LCDM2
            rra.w   R14
            mov.b   R14, &LCDM3
            rra.w   R14
            mov.b   R14, &LCDM4

            mov.w   R15, R12                ; move data to display
            rra.w   R12                     ; shift to get 2nd digit....
            rra.w   R12
            rra.w   R12
            rra.w   R12
            and.w   #0x0F, R12              ; get 2nd digit
            mov.w   LCD_Tab(R12), R14       ; lookup display data in table
            bit.w   #0x01, R12              ; test for odd digit
            jz      EVEN_2                  ; if not, do nothing
            swpb    r14                     ; else, swap bytes to get high byte
EVEN_2      mov.b   R14, &LCDM5             ; shift data to LCD memory...
            rra.w   R14
            mov.b   R14, &LCDM6
            rra.w   R14
            mov.b   R14, &LCDM7
            rra.w   R14
            mov.b   R14, &LCDM8

            mov.w   R15, R12                ; move data to display
            swpb    R12                     ; 8 RRAs to get 3rd digit
            and.w   #0x0F, R12              ; get 3rd digit
            mov.w   LCD_Tab(R12), R14       ; lookup display data in table
            bit.w   #0x01, R12              ; test for odd digit
            jz      EVEN_3                  ; if not, do nothing
            swpb    r14                     ; else, swap bytes to get high byte
EVEN_3      mov.b   R14, &LCDM11            ; shift data to LCD memory...
            rra.w   R14
            mov.b   R14, &LCDM12
            rra.w   R14
            mov.b   R14, &LCDM13
            rra.w   R14
            mov.b   R14, &LCDM14

LOOP        jmp     LOOP                    ; do nothing so as to display data
;------------------------------------------------------------------------------
;            LCD Type Definition
;------------------------------------------------------------------------------
;Segments definition
a            .equ    001h
b            .equ    010h
c            .equ    002h
d            .equ    020h
e            .equ    004h
f            .equ    040h
g            .equ    008h
h            .equ    080h
             .align 2                       ; align words on .align 2 boundry		
LCD_Tab      .byte   a+b+c+d+e+f            ; Displays "0"
             .byte   b+c                    ; Displays "1"
             .byte   a+b+d+e+g              ; Displays "2"
             .byte   a+b+c+d+g              ; Displays "3"
             .byte   b+c+f+g                ; Displays "4"
             .byte   a+c+d+f+g              ; Displays "5"
             .byte   a+c+d+e+f+g            ; Displays "6"
             .byte   a+b+c                  ; Displays "7"
             .byte   a+b+c+d+e+f+g          ; Displays "8"
             .byte   a+b+c+d+f+g            ; Displays "9"        		
;------------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .end
