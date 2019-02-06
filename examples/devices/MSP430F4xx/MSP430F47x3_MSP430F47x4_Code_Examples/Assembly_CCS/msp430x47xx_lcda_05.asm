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
;    MSP430x47xx Demo - LCD_A Put "0123456" on SBLCDA4 LCD
;
;  Description; Put "0123456" on SBLCDA4 LCD.
;  ACLK = LFXT1 = 32768, MCLK = SMCLK = DCO = 32xACLK = 1.048576MHz
;  //*An external watch crystal is required on XIN/XOUT for ACLK*//	
;  Note: A 4.7 uF or larger capacitor must be connected from pin LCDCAP to
;  ground when the internal charge pump is enabled. Otherwise, damage can occur.
;
;                MSP430x47xx
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
;         --|RST          XOUT|-         SBLCDA4 
;           |                 |        ___________
;           |              S4 |------>|14         |
;           |              S5 |------>|13         | 
;           |             ..  | ....  |..         |
;           |             S17 |------>|1          | 
;     ------|LCDCAP/R33   COM0|------>|15         |
;    CAP    |             COM1|------>|16         | 
;   TO GND  |             COM2|------>|17         | 
;           |             COM3|------>|18         |
;           |             S18 |------>|19         | 
;           |             S19 |------>|20         | 
;           |              .. | ....  |..         | 
;     ------|LCDCAP/R33   S25 |------>|26         |     
;           |_________________|       |___________| 
;
;
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
			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #900h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #XCAP11PF,&FLL_CTL0     ; Set load capacitance for xtal
            mov.b   #LCDON + LCD4MUX, &LCDACTL ; 4mux LCD, ACLK/32
            mov.b   #0x7E, &LCDAPCTL0       ; Segments 0-13
            mov.b   #0x1C, &P5SEL           ; set COM pins for LCD
            bis.b   #LCDCPEN,&LCDAVCTL0     ; charge pump enable
            bis.b   #VLCD3,&LCDAVCTL1       ; to 3.02V 
ClearLCD    mov.w   #20,R15                 ; 15 LCD memory bytes to clear
Clear1      mov.b   #0,LCDM1(R15)           ; Write zeros in LCD RAM locations
                                            ; to clear display
            dec     R15                     ; All LCD mem clear?
            jc      Clear1                  ; More LCD mem to clear go, use JC
                                            ; to get memory location 0
setLCD      clr.w   R15                     ; 0 is first digit to display
            mov.w   #2,R14
writeLCD    mov.b   LCD_Tab(R15),LCDMEM(R14); Write to LCD with offset of 2
            inc     R14                     ; inc table pointer
            inc     R15
            cmp     #9, R14
            jl      writeLCD
            bis     #LPM3,SR                ; Set SR bits to wait in LPM3

d           .equ     0x08
c           .equ     0x04
b           .equ     0x02
a           .equ     0x01
h           .equ     0x80
e           .equ     0x40
g           .equ     0x20
f           .equ     0x10

LCD_Tab     .byte      a+b+c+d+e+f             ; Displays '0'
            .byte      b+c                     ; Displays '1'
            .byte      a+b+d+e+g               ; Displays '2'
            .byte      a+b+c+d+g               ; Displays '3'
            .byte      b+c+f+g                 ; Displays '4'
            .byte      a+c+d+f+g               ; Displays '5'
            .byte      a+c+d+e+f+g             ; Displays '6'
            .byte      a+b+c                   ; Displays '7'
            .byte      a+b+c+d+e+f+g           ; Displays '8'
            .byte      a+b+c+d+f+g             ; Displays '9'

;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect	".reset"            	; MSP430 RESET Vector
            .short  RESET                   ;
            .end
    
