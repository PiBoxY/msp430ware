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
;   MSP-FET430P140 Demo - USART0, SPI 3-Wire Master
;
;   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
;   data is sent by the master starting at 0x00. Received data is expected to
;   decrement starting at 0xFF. USART0 RX ISR is used to handle communication
;   with the CPU normally in LPM0. The slave is initialized by the master
;   followed by a slave setup delay time in order to assure proper SPI
;   synchronization. P1.0 indicates proper data reception.
;   ACLK = n/a, MCLK = SMCLK = DCO ~ 800kHz
;
;                 MSP430F149
;              -----------------
;          /|\|              XIN|-
;           --|RST          XOUT|-
;             |                 |
;       LED <-|P1.0         P3.1|-> Data Out (SIMO0)
;             |                 |
;     Scope <-|P3.4         P3.2|<- Data In (SOMI0)
;             |                 |
; Slave RST <-|P3.5         P3.3|-> Serial Clock Out (UCLK)
;
;   Z. Albus / G. Morton
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

MST_Data   .equ      R6
SLV_Data   .equ      R7

;-----------------------------------------------------------------------------
            .text                           ; Program Reset
;-----------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP1     mov.b   #000h,&P1OUT            ; P1.0 setup for LED output
            bis.b   #001h,&P1DIR            ;
SetupP3     bis.b   #00Eh,&P3SEL            ; P3.1,2,3 SPI option select
            mov.b   #020h,&P3OUT            ; P3.4 setup for scope trigger and
            bis.b   #030h,&P3DIR            ; P3.5 for slave initialize
SetupSPI    mov.b   #CHAR+SYNC+MM+SWRST,&U0CTL ; 8-bit, SPI, Master
            bis.b   #CKPL+SSEL1+STC,&U0TCTL ; Polarity, SMCLK, 3-wire
            mov.b   #002h,&U0BR0            ; SPICLK = SMCLK/2
            mov.b   #000h,&U0BR1            ;
            mov.b   #000h,&U0MCTL           ;
            bis.b   #USPIE0,&ME1            ; Module enable
            bic.b   #SWRST,&U0CTL           ; SPI enable
            bis.b   #URXIE0,&IE1            ; Recieve interrupt enable
                                            ;
Init_Slave  bic.b   #020h,&P3OUT            ; Toggle P3.5: reset slave
            bis.b   #020h,&P3OUT            ;
Wait        mov.w   #050000,R15             ; Delay to R15
L1          dec.w   R15                     ; Decrement R15
            jnz     L1                      ; Delay over?
                                            ;
            mov.b   #000h,MST_Data          ;
            mov.b   #0FFh,SLV_Data          ;
                                            ;
Mainloop    mov.b   MST_Data,&TXBUF0        ; Transmit first character
            bis.b   #LPM0+GIE,SR            ; CPU off, enable interrupts
            nop                             ; Required for debugger only
                                            ;
;------------------------------------------------------------------------------
USART0RX_ISR;       Test for valid RX and TX character
;------------------------------------------------------------------------------
            xor.b   #010h,&P3OUT            ; XOR P3.4 for scope trigger
TX1         bit.b   #UTXIFG0,&IFG1          ; USART0 TX buffer ready?
            jz      TX1                     ; Jump if TX buffer not ready
            cmp.b   SLV_Data,&RXBUF0        ; Test for correct character RX'd
            jeq     PASS                    ;
FAIL        mov.b   MST_Data,&TXBUF0        ;
            bis.b   #001h,&P1OUT            ; Set P1.0 indicating data error
            reti                            ; Exit ISR
PASS        inc.b   MST_Data                ;
            dec.b   SLV_Data                ;
            mov.b   MST_Data,&TXBUF0        ;
            bis.b   #001h,&P1OUT            ; Pulse P1.0 indicating valid data
            bic.b   #001h,&P1OUT            ;
            reti                            ; Exit ISR
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ;
            .short  RESET                   ; POR, ext. Reset, Watchdog
            .sect   ".int09"                ;
            .short  USART0RX_ISR            ; USART0 receive
            .end                            ;
