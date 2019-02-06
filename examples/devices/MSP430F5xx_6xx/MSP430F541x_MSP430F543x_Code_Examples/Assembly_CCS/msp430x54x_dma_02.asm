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
;   MSP430x54x Demo - DMA0, Single Transfer in Block Mode UART1 9600, ACLK
;
;   Description: DMA0 is used to transfer a string as a block to USCI_A0.
;   USCIA0TXIFG WILL trigger DMA0. "Hello World" is TX'd via 9600 baud on
;   USCI_A0. Watchdog in interval mode triggers block transfer every 1000ms.
;   Level senstive trigger used for USCIA0TXIFG to prevent loss of inital edge
;   sensitive triggers - USCIA0TXIFG which is set at POR.
;   ACLK = UCLK 32768Hz, MCLK = SMCLK = default DCO 1048576Hz
;   Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (0003h 4Ah)
;
;                 MSP430x54x
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32768Hz
;          --|RST          XOUT|-
;            |                 |
;            |             P3.4|------------> "Hello World"
;            |                 | 9600 - 8N1
;
;   M. Morales
;   Texas Instruments Inc.
;   July 2009
;   Built with Code Composer Essentials v3.x
;*******************************************************************************

    .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
LF          .equ    0x0a                     ; ASCII Line Feed
CR          .equ    0x0d                     ; ASCII Carriage Return
;-------------------------------------------------------------------------------
 
;-------------------------------------------------------------------------------
            .global _main 
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
String1     .byte   CR,LF,"Hello World"
_main
;-------------------------------------------------------------------------------
RESET       mov.w   #0x5C00,SP              ; Initialize stackpointer
StopWDT     mov.w   #WDT_ADLY_1000,&WDTCTL  ; WDT 1000ms, ACLK, interval timer
            bis.w   #WDTIE,&SFRIE1          ; Enable WDT interrupt
SetupP3     bis.b   #0x30,&P3SEL            ; P3.4,5 = USCI_A0 TXD/RXD
SetupUSCIA0 bis.b   #UCSWRST,&UCA0CTL1      ; 8-bit characters
            mov.b   #UCSSEL_1,&UCA0CTL1     ; CLK = ACLK
            mov.b   #0x03,&UCA0BR0          ; 32k/9600=3.41
            mov.b   #0x00,&UCA0BR1          ;
            mov.b   #UCBRS_3+UCBRF_0,&UCA0MCTL ; Modulation
            bic.b   #UCSWRST,&UCA0CTL1      ; Release USCI state machine
SetupDMA0   mov.w   #DMA0TSEL_17,&DMACTL0   ; USCI_A0 TXIFG trigger
            movx.a  #String1,&DMA0SA        ; Source block address
            movx.a  #UCA0TXBUF,&DMA0DA      ; Destination single address
            mov.w   #13,&DMA0SZ             ; Block size
            mov.w   #DMASRCINCR_3+DMASBDB,&DMA0CTL ; Repeat, inc src
                                            ;
Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3 w/ interrupts
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
WDT_ISR ;    Trigger DMA block transfer
;-------------------------------------------------------------------------------
            bis.w   #DMAEN,&DMA0CTL         ; Enable
            bic.b   #UCTXIFG,&UCA0IFG       ; Toggle TX IFG to trigger first
            bis.b   #UCTXIFG,&UCA0IFG       ; DMA transfer...
            reti                            ;
            
;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int58"                  ; WatchDog Timer Vector
            .short  WDT_ISR
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
            
