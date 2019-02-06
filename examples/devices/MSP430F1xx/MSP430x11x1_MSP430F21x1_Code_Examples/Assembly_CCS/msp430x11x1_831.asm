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
;   MSP430x11x1 Demo - Software SPI Interface to TLV0831
;
;   Description: This program will read a TLV0831 ADC and store the 8-bit
;   digital code in RAM byte 200h. The TLV0831 is sampled in a loop.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~800k
;
;                       MSP430F1121
;                     ----------------
;                /|\|              XIN|-
;                 | |                 |
;                 --|RST          XOUT|-
;                   |                 |
;      TLV0831      |                 |
;    ----------     |                 |
;   |        CS|<---|P2.0             |
; ~>| IN+   CLK|<---|P2.1             |
;   |        DO|--->|P2.3             |
;
;
CS          .equ   001h                     ; P2.0 Chip Select
CLK         .equ   002h                     ; P2.1 Clock
DO          .equ   008h                     ; P2.3 Data Out
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
RESET       mov     #0300h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer
SetupP2     bis.b   #CS,&P2OUT              ; CS, set
            bis.b   #CS+CLK,&P2DIR          ; CS and Clk Output direction
                                            ;
Mainloop    call    #Meas_0831              ; Call subroutine
            jmp     Mainloop                ; Repeat
                                            ;
;-------------------------------------------------------------------------------
Meas_0831;  Subroutine to read TLV0831, data is shifted into RAM byte
;           200h - temporary use of stack for bit-counter.
;-------------------------------------------------------------------------------
            bic.b   #CS,&P2OUT              ; Chip Select low
            push.b  #9                      ; 9 bits = 1 start+8 data
ADC_Loop    bis.b   #CLK,&P2OUT             ; Clock high
            bic.b   #CLK,&P2OUT             ; Clock low
            bit.b   #DO,&P2IN               ; DO -> C (carry)
            rlc.b   &200h                   ; C -> 200h
            dec.b   0(SP)                   ; All shifted in?
            jnz     ADC_Loop                ; If not --> ADC_Loop
            incd.w  SP                      ; Clean up stack
            bis.b   #CS,&P2OUT              ; Chip Select high
            ret                             ; Return from Subroutine
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
