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
;   MSP-FET430P430 Demo - DMA0, Repeated Block UART0 9600, CCR2IFG, ACLK
;
;   Description: DMA0 is used to transfer a string byte-by-byte as a repeating
;   block to U1TXBUF. Timer_A operates continuously at 32768Hz with CCR2IFG
;   triggering DMA0. "Hello World" is TX'd via 9600 baud UART1.
;   Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (03h|4Ah)
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   ;* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;                MSP430FG439
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |       P2.4/UTXD0|------------> "Hello World"
;            |                 | 9600 - 8N1
;
;   M. Buccini / M. Mitchell
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

LF          .equ    0ah                     ; ASCII Line Feed
CR          .equ    0dh                     ; ASCII Carriage Return
                                            ;
;------------------------------------------------------------------------------
            .text                  ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
SetupP3     bis.b   #018h,&P2SEL            ; P2.4,5 = USART0 TXD/RXD
SetupUART0  bis.b   #UTXE0+URXE0,&ME1       ; Enable USART0 TXD/RXD
            bis.b   #CHAR,&UCTL0            ; 8-bit characters
            mov.b   #SSEL0,&UTCTL0          ; UCLK = ACLK
            mov.b   #003h,&UBR00            ; 32k/9600=3.41
            mov.b   #000h,&UBR10            ;
            mov.b   #04Ah,&UMCTL0           ; Modulation
            bic.b   #SWRST,&UCTL0           ; *Initialize USART state machine*
SetupDMA0   mov.w   #DMA0TSEL_1,&DMACTL0    ; CCR2 trigger
            mov.w   #String1,&DMA0SA        ; Source block address
            mov.w   #TXBUF0,&DMA0DA         ; Destination single address
            mov.w   #13,&DMA0SZ             ; Block size
            mov.w   #DMADT_4+DMASRCINCR_3+DMASBDB+DMAEN,&DMA0CTL; Repeat, inc src
SetupTA     mov.w   #8192,&TACCR0           ; Char freq = TACLK/CCR0
            mov.w   #1,&TACCR2              ; DMA0 trigger
            mov.w   #TASSEL_1+MC_1,&TACTL   ; ACLK, up-mode
                                            ;
Mainloop    bis.w   #LPM3,SR                ; Enter LPM3
            nop                             ; Required only for debugger
                                            ;
String1      .byte  CR,LF, "Hello World"
                                            ;
;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .end
