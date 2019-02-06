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
;   MSP430x261x Demo - DMA0, single transfer Mode UART1 9600, ACLK
;
;   Description: DMA0 is used to transfer a string as a block to U1TXBUF.
;   UTXIFG1 WILL trigger DMA0. "Hello World" is TX'd via 9600 baud on UART1.
;   Watchdog in interval mode triggers block transfer every 1000ms.
;   Level senstive trigger used for UTXIFG1 to prevent loss of inital edge
;   sensitive triggers - UTXIFG1 which is set at POR.
;   ACLK = UCLK 32768Hz, MCLK = SMCLK = default DCO 1048576Hz
;   Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (0003h)
;
;                MSP430x2619
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32768Hz
;          --|RST          XOUT|-
;            |                 |
;            |             P3.6|------------> "Hello World"
;            |                 | 9600 - 8N1
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
LF          .equ    0ah                     ; ASCII Line Feed
CR          .equ    0dh                     ; ASCII Carriage Return
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0850h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDT_ADLY_1000,&WDTCTL  ; WDT 1000ms, ACLK, interval timer
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
SetupP3     bis.b   #BIT6+BIT7,&P3SEL       ; P3.6,7 = USART1 TXD/RXD
SetupUSCI1  bis.b   #UCSSEL_1,&UCA1CTL1     ; ACLK
            mov.b   #3,&UCA1BR0             ; 32768Hz 9600 32k/9600=3.41
            mov.b   #0,&UCA1BR1             ; 32768Hz 9600
            mov.b   #UCBRS_3,&UCA1MCTL      ; Modulation UCBRSx = 3
            bic.b   #UCSWRST,&UCA1CTL1      ; **Initialize USCI state machine**
SetupDMA0   mov.w   #DMA0TSEL_10,&DMACTL0   ; UTXIFG1 trigger
            movx.a  #String1,&DMA0SA        ; Source block address
            movx.a  #UCA1TXBUF,&DMA0DA      ; Destination single address
            mov.w   #0013,&DMA0SZ           ; Block size
            mov.w   #DMASRCINCR_3+DMASBDB+DMALEVEL,&DMA0CTL; Repeat, inc src
                                            ;
Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3 w/ interrupts
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
WDT_ISR;    Trigger DMA block transfer
;-------------------------------------------------------------------------------
            bis.w   #DMAEN,&DMA0CTL         ; Enable
            reti                            ;
;-------------------------------------------------------------------------------
String1     .word      CR,LF, 'Hello World'
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int26"              ; Watchdog Timer
            .short  WDT_ISR
            .sect	".reset"	           ; POR, ext. Reset, Watchdog
            .short  RESET
            .end
