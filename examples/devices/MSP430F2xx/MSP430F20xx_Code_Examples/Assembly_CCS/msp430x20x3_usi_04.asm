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
;   MSP430F20xx Demo - USI SPI Interface to HC165/164 Shift Registers
;
;   Description: Demonstrate USI in two-way SPI mode. Data are read from
;   an HC165, and same data written back to the HC164.
;   ACLK = n/a  MCLK = SMCLK = Default DCO, USI = SMCLK/2
;
;                         MSP430F20x2/3
;                       -----------------
;                   /|\|              XIN|-
;                    | |                 |     ^      HC164
;          HC165     --|RST          XOUT|-    |  -------------
;        ----------    |                 |     |-|/CLR,B       |  8
;    8  |      /LD|<---|P1.1   SIMO0/P1.6|------>|A          Qx|--\->
;   -\->|A-H   CLK|<---|P1.5/SCLK  - P1.5|------>|CLK          |
;     |-|INH    QH|--->|P1.7/SOMI        |       |             |
;     |-|SER      |    |                 |       |             |
;     - |         |    |                 |       |             |
;
;   P.Thanigai
;   Texas Instruments Inc.
;   May 2007
;   Built with Code Composer Essentials Version: 2.0
;*******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP1     bis.b   #02h,&P1OUT             ;
            bis.b   #02h,&P1DIR             ;
SetupUSI    bis.b   #USIPE7+USIPE6+USIPE5+USIMST+USIOE,&USICTL0; Port, SPI master
            bis.b   #USICKPH+USIIE,&USICTL1 ; Counter interrupt, flag remains
            mov.b   #USIDIV_1+USISSEL_2,&USICKCTL    ; /2 SMCLK
            bic.b   #USISWRST,&USICTL0      ; Enable USI
            mov.b   #08h,&USICNT            ; init-load counter, clears flag
                                            ;
Mainloop    call    #RXTX_HC16x             ; Exchange data
Delay       push.w  #0                      ; Software delay
D1          dec.w   0(SP)                   ;
            jnz     D1                      ;
            incd.w  SP                      ;
            jmp     Mainloop                ; Repeat
                                            ;
;-------------------------------------------------------------------------------
RXTX_HC16x;   HC165--> URXBUF0  UTXBUF0 --> HC164
;-------------------------------------------------------------------------------
TX0         bit.b   #USIIFG,&USICTL1        ; USART0 TX buffer ready?
            jz      TX0                     ; Jump --> TX buffer not ready
            bic.b   #02h,&P1OUT             ; Latch data into 'HC165
            bis.b   #02h,&P1OUT             ;
;           ********************            ; Read data are ready to be written
            mov.b   #08h,&USICNT            ; Load counter, clears flag
            ret                             ; Return from subroutine
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;      
            .end

