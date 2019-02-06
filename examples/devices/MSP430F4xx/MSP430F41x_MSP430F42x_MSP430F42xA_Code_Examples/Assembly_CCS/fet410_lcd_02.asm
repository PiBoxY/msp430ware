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
;   MSP-FET430P410 Demo - LCD, Display Numbers on a Static LCD
;
;   Description: This program displays a 3.5 digit number stored in register
;   R14 on a static LCD, then waits in low power mode 3. The leading digit
;   must be '1', nothing else will display. To use the program enter a 4
;   digit BCD (characters 0-9) number in R14 and press enter. Reset and run
;   the program.  The value in R14 will be displayed on the LCD. If the user
;   does not have the LCD connected the functioning of the software can still
;   be followed by single stepping and watching the register values
;   and the LCD memory values in the LCD memory
;   locations as the software is executed.
;
;   NOTE: The code is written such that single digits can be passed to the
;         display routine. The routine to shift bytes, currently located
;         in the mainloop, could be moved to the display routine if the
;         passing of multiple digits is preferred.
;
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   ;* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;
;                                 Connections MSP430 -> LCD
;                                 -------------------------
;
;                                T.I.               Varitronix
;                           MSP430x41x MCU     3.5 digit, static LCD
;                                                 # VI-302-DP-S
;                           ---------------       --------------
;                          |          COM0 |-----|1,40 COM      |
;                          |          SEG0 |-----|21            |
;                          |          SEG1 |-----|20            |
;                          |          SEG2 |-----|19            |
;                          |          SEG3 |-----|18            |
;                          |          SEG4 |-----|17            |
;                          |          SEG5 |-----|22            |
;                          |          SEG6 |-----|23            |
;                          |          SEG7 |-----|38 (low batt) |
;                          |          SEG8 |-----|25            |
;                          |          SEG9 |-----|24            |
;                          |          SEG10|-----|15            |
;                          |          SEG11|-----|14            |
;                          |          SEG12|-----|13            |
;                          |          SEG13|-----|26            |
;                          |          SEG14|-----|27            |
;                          |          SEG15|-----|28 (L)  COLON |
;                          |          SEG16|-----|30            |
;                          |          SEG17|-----|29            |
;                          |          SEG18|-----|11            |
;                          |          SEG19|-----|10            |
;                       ---|R03       SEG20|-----|9             |
;                      |   |          SEG21|-----|31            |
;                      |   |          SEG22|-----|32            |
;                     \|/  |          SEG23|-----|3 (B-C) =     |
;                     GND  |               |     | .5 digit ="1"|
;                           ---------------       --------------
;
;                 NOTE: Pin R03 on the MSP430 must be connected to GND
;
;   G. Morton / A. Dannenberg
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-----------CPU registers used-------------------------------------------------
;;;;;;;     R12, R13, R14, R15              ; Used as temporary registers, can
                                            ; be reused
;------------------------------------------------------------------------------
            .text                  ; Program reset
;------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize stack pointer
            call    #Setup                  ; Setup

Mainloop    mov.b   R14,R13                 ; Copy R14 to R13 (save R14)
            call    #ToLCD                  ; Call to display 1st digit
            mov.b   R14,R13                 ; Copy R14 to R13 (save R14)
            rra.b   R13                     ; Right shift R13 4 times to
            rra.b   R13                     ; move 2nd nibble to lower 4 bits
            rra.b   R13                     ;
            rra.b   R13                     ;
            call    #ToLCD                  ; Call to display 2nd digit
            swpb    R14                     ; Swap bytes in R14
            mov.b   R14,R13                 ; Copy R14 to R13 (save R14)
            call    #ToLCD                  ; Call to display 3rd digit
            mov.b   R14,R13                 ; Copy R14 to R13
            rra.b   R13                     ; Right shift R13 4 times to
            rra.b   R13                     ; move 2nd nibble to lower 4 bits
            rra.b   R13                     ;
            rra.b   R13                     ;
            call    #ToLCD                  ; Call to display 4th digit if = 1
            bis.w   #LPM3,SR                ; set SR bits to wait in LPM3

;------------------------------------------------------------------------------
;           Display 1 digit on static LCD. CPU Registers R15, R14 and R13
;           used temporarily.
;------------------------------------------------------------------------------
ToLCD       cmp.w   #09Dh,R15               ; Pointer incremented for 4th
                                            ; Digit = 1/2 digit position?
            jeq     Halfdigit               ; Yes, jump to test/set leading 1
            and.w   #000Fh,R13              ; Blank upper 12 bits
            add.w   R13,R13                 ; Double R13 since is word data
                                            ; where addresses = 2 bytes apart
            add.w   R13,R13                 ; Double R13 again since 2 words
                                            ; per digit in static mode
ContLCD     mov.w   LCD_Tab(R13),R12    	; Low word into temp memory
            mov.b   R12,0(R15)          	; Low byte of low word to LCD
            swpb    R12                 	; Swap bytes in temp memory
            bis.b   R12,1(R15)          	; High byte of low word to LCD
            incd    R13                     ; Double increment to high word of
                                            ; digit
            mov.w   LCD_Tab(R13),R12    	; High word into temp memory
            mov.b   R12,2(R15)          	; Low byte of high word to LCD
            swpb    R12                 	; Swap bytes in temp memory
            bis.b   R12,3(R15)          	; High byte of high word to LCD
            add     #4,R15                  ; Increase R15 (LCD mem pointer)
                                            ; by 4 since 4 bytes per digit
EndLCD      ret                             ; Return from call

;------------------------------------------------------------------------------
;           Test if 1 is to be displayed in 1/2 digit position and display.
;           If leading zero then nothing is displayed.
;------------------------------------------------------------------------------
Halfdigit   cmp.b   #1,R13                  ; Digit to display = 1?
            jne     EndHalf                 ; No, jump to not display digit
            bis.b   #10h,-1(R15)            ; set h bit in 12th byte(0-11)
                                            ; =1/2 digit (leading 1) bit
EndHalf     mov.b   #LCDM1,R15              ; set pointer to start of LCD mem
            jmp     EndLCD                  ; Return from routine

;------------------------------------------------------------------------------
Setup       ; Configure modules and control registers
;------------------------------------------------------------------------------
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer
SetupFLL2   bis.b   #XCAP18PF,&FLL_CTL0     ; set load capacitance for xtal
            mov.w   #10000,R15              ;
Xtal_Wait   dec.w   R15                     ; Delay for 32 kHz crystal to
            jnz     Xtal_Wait		    	; stabilize
SetupLCD    mov.b   #LCDP1+LCDP0+LCDSTATIC+LCDON,&LCDCTL
                                            ; Static LCD, segments S0-S23
SetupBT     mov.b   #BTFRFQ1+BTFRFQ0,&BTCTL ; set freqLCD = ACLK/256
SetupPorts  mov.b   #0FFh,&P1DIR            ; set port to outputs
SetupPx     mov.b   #0FFh,&P2DIR            ; set port to outputs
            mov.b   #0FFh,&P3DIR            ; set port to outputs
            mov.b   #0FFh,&P4DIR            ; set port to outputs
            mov.b   #0FFh,&P5DIR            ; set port to outputs
            mov.b   #0FFh,&P6DIR            ; set port to outputs

ClearLCD    mov.w   #15,R15                 ; 15 LCD memory bytes to clear
Clear1      mov.b   #0,LCDM1(R15)           ; Write zeros in LCD RAM locations
                                            ; to clear display
            dec.w   R15                     ; All LCD mem clear?
            jc      Clear1                  ; More LCD mem to clear, use JC
                                            ; to get memory location 0
            mov.b   #LCDM1,R15              ; R15 points to first LCD location
            ret
              		
;------------------------------------------------------------------------------
;             Define characters for VI-302 3.5 digit static LCD
;------------------------------------------------------------------------------
a           .equ    0001h
b           .equ    0010h
c           .equ    0100h
d           .equ    1000h
e           .equ    0001h
f           .equ    0010h
g           .equ    0100h
h           .equ    1000h                   ; Decimal point or special
                                            ; character

            .align  2                       ; Align words on even boundary
LCD_Tab     .short  a+b+c+d                 ; Displays '0' low word
            .short  e+f                     ; Displays '0' high word
            .short  b+c                     ; Displays '1' low word
            .short  0                       ; Displays '1' high word (empty)
            .short  a+b+d                   ; Displays '2' low word
            .short  e+g                     ; Displays '2' high word
            .short  a+b+c+d                 ; Displays '3' low word
            .short  g                       ; Displays '3' high word
            .short  b+c                     ; Displays '4' low word
            .short  f+g                     ; Displays '4' high word
            .short  a+c+d                   ; Displays '5' low word
            .short  f+g                     ; Displays '5' high word
            .short  a+c+d                   ; Displays '6' low word
            .short  e+f+g                   ; Displays '6' high word
            .short  a+b+c                   ; Displays '7' low word
            .short  0                       ; Displays '7' high word (empty)
            .short  a+b+c+d                 ; Displays '8' low word
            .short  e+f+g                   ; Displays '8' high word
            .short  a+b+c+d                 ; Displays '9' low word
            .short  f+g                     ; Displays '9' high word

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET
            .end

