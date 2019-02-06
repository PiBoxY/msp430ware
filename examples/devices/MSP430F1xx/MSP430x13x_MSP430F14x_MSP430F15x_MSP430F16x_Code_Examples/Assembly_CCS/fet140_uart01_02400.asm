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
;   MSP-FET430P140 Demo - USART0, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
;
;   Description: Echo a received character, RX ISR used. Normal mode is LPM3,
;   USART0 RX interrupt triggers TX Echo.
;   ACLK = UCLK0 = LFXT1 = 32768Hz, MCLK = SMCLK = DCOCLK
;   Baud rate divider with 32768hz XTAL @2400 = 32768Hz/2400 = 13.65
;   //* An external watch crystal on XIN XOUT is required for ACLK *//	
;
;                 MSP430F149
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |             P3.4|----------->
;            |                 | 2400 - 8N1
;            |             P3.5|<-----------
;
;   M. Buccini / G. Morton
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP3     bis.b   #030h,&P3SEL            ; P3.4,5 = USART0 TXD/RXD
SetupUART0  bis.b   #UTXE0+URXE0,&ME1       ; Enable USART0 TXD/RXD
            bis.b   #CHAR,&UCTL0            ; 8-bit characters
            mov.b   #SSEL0,&UTCTL0          ; UCLK = ACLK
            mov.b   #00Dh,&UBR00            ; 32k/2400 - 13.65
            mov.b   #000h,&UBR10            ; 32k 2400
            mov.b   #06Bh,&UMCTL0           ; 32k 2400 modulation
            bic.b   #SWRST,&UCTL0           ; **Initialize USART state machine**
            bis.b   #URXIE0,&IE1            ; Enable USART0 RX interrupt
                                            ;
Mainloop    bis.b   #LPM3+GIE,SR            ; Enter LPM3, interrupts enabled
            nop                             ; Required for debugger
                                            ;
;------------------------------------------------------------------------------
USART0RX_ISR;  Echo back RXed character, confirm TX buffer is ready first
;------------------------------------------------------------------------------
TX1         bit.b   #UTXIFG0,&IFG1          ; USART0 TX buffer ready?
            jz      TX1                     ; Jump is TX buffer not ready
            mov.b   &RXBUF0,&TXBUF0         ; TX -> RXed character
            reti                            ;
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ;
            .short  RESET                   ; POR, ext. Reset, Watchdog
            .sect   ".int09"                ;
            .short  USART0RX_ISR            ; USART0 receive
            .end
