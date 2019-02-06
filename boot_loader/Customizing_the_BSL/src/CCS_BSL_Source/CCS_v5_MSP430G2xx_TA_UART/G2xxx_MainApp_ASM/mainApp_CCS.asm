;*******************************************************************************
;   Example of an Main Application to be used in conjunction with G2xxx_BSL
;
;                MSP430G2xxx
;             -----------------
;         /|\|                 |
;          | |        P1.0/P1.6|---> LED for MAIN application
;          --|RST              |
;            |                 |
;            |      BSLPIN/P1.3|<--- Enable Pin / S2 on LaunchPad
;            |                 |
;            |                 |
;            |          rX/P1.1|<--- UART 9600 8N1
;            |          tX/P1.2|---> rx/tx swapped on LaunchPad!
;             -----------------
;
;
;   Texas Instruments
;   Built with CCS 4.2.3
;
;*******************************************************************************
;  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;    Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

;*******************************************************************************
            .cdecls C,LIST,"msp430.h"  ; Include device header file

;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
            ;Main / Customer Application
;-------------------------------------------------------------------------------
LED         .equ    BIT0                    ; RED LED
;LED        .equ    BIT6                    ; GREEN LED
;-------------------------------------------------------------------------------
            .text                           ; User Application
            ; ================================================================
            ; Never place any code / data between .text (RSEG CODE) and main
            ; label. This adress is used as start adress for user code
            ; ================================================================
;-------------------------------------------------------------------------------
            ; Customer Application starts here
main        nop                             ; User Code needs to start here!
            bis.b   #LED,&P1DIR             ; Make pin an output
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer

Mainloop    xor.b   #LED,&P1OUT             ; Toggle LED
            mov.w   #0FFFFh,R15             ; Delay to R15
DelayL1     dec.w   R15                     ; Decrement R15
            ; ================================================================
            ; Note that any Register and RAM can be used
            ; ================================================================
            inc.w   R15                     ; Increment R15
            dec.w   R15                     ; Decrement R15
            jnz     DelayL1                 ; Delay over?
            
            mov.b   #10, R14                ; Rep Count
Blink1      xor.b   #LED,&P1OUT             ; Toggle P1.0
            mov.w   #0AFFFh,R15             ; Delay to R15
DelayB1     dec.w   R15                     ; Decrement R15
            inc.w   R15                     ; Increment R15
            dec.w   R15                     ; Decrement R15
            jnz     DelayB1                 ; Delay over?
            dec.b   R14
            jnz     Blink1

            mov.b   #20, R14                ; Rep Count
Blink2      xor.b   #LED,&P1OUT             ; Toggle P1.0
            mov.w   #057ffh,R15             ; Delay to R15
DelayB2     dec.w   R15                     ; Decrement R15
            inc.w   R15                     ; Increment R15
            dec.w   R15                     ; Decrement R15
            jnz     DelayB2                 ; Delay over?
            dec.b   R14
            jnz     Blink2
            
            mov.b   #40, R14                ; Rep Count
Blink3      xor.b   #LED,&P1OUT             ; Toggle P1.0
            mov.w   #02BFFh,R15             ; Delay to R15
DelayB3     dec.w   R15                     ; Decrement R15
            inc.w   R15                     ; Increment R15
            dec.w   R15                     ; Decrement R15
            jnz     DelayB3                 ; Delay over?
            dec.b   R14
            jnz     Blink3
            jmp     Mainloop
;-------------------------------------------------------------------------------
            ;END of Main / Customer Application
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            ; ================================================================
            ; Place all user program interrupt vectors here
            ; ================================================================



            ; ================================================================
            ; The following vectors are used by G2xxxBSL and can not be used
            ;  by user application
            ; ================================================================
            .sect   ".reset"                ; RESET Vector
            .short  main                    ; G2xxx BSL will overwrite this
                                            ;  vector but user program will 
                                            ;  start as if RESET points to main
                                            ;  label
            .end
