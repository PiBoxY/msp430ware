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
;  MSP-FET430P140 Demo - DMA0/1, Block Mode UART1 9600 Auto RX/TX String, ACLK
;
;  Description: DMA1 is used to capture a 32 byte string from USART1. The
;  DMA1 interrupt is used to trigger DMA0 to echo back the received string.
;  USART1 is configured at 96008N1.
;  Level senstive trigger used for UTXIFG1 to prevent loss of initial edge
;  sensitive triggers - UTXIFG1 which is set at POR.
;  ACLK = 32768Hz = UCLK1, MCLK = SMCLK = default DCO ~ 800k
;  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
;  //* An external watch crystal on XIN XOUT is required for ACLK *//	
;  //* Final Production MSP430F16x(x) Device Required *//
;
;               MSP430F169
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32768Hz
;         --|RST          XOUT|-
;           |                 |
;           |             P3.6|------------>
;           |                 | 9600 - 8N1
;           |             P3.7|<------------
;
;
;   H. Grewal / A. Dannenberg
;   Texas Instruments Inc.
;   November 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
            mov.b   #0C0h,&P3SEL            ; P3.6,7 = USART1 TXD/RXD
            bis.b   #UTXE1+URXE1,&ME2       ; Enable USART1 TXD/RXD
            bis.b   #CHAR,&UCTL1            ; 8-bit characters
            mov.b   #SSEL0,&UTCTL1          ; UCLK1 = ACLK
            mov.b   #03h,&UBR01             ; 9600 from 32768Hz
            mov.b   #00h,&UBR11             ;
            mov.b   #04Ah,&UMCTL1           ; Modulation
            bic.b   #SWRST,&UCTL1           ; **Initialize USART state machine**
            mov.w   #DMA1TSEL_9+DMA0TSEL_10,&DMACTL0 ; DMA1=URXIFG1, DMA0=UTXIFG1
Playbck     mov.w   #0220h,&DMA0SA          ; Src = RAM memory
            mov.w   #U1TXBUF, &DMA0DA       ; Dst = UART TX Buffer
            mov.w   #010h,&DMA0SZ           ; Size in words
            mov.w   #DMASRCINCR_3+DMASBDB+DMALEVEL,&DMA0CTL ; Inc src, enable
Record      mov.w   #U1RXBUF, &DMA1SA       ; Src address = UART RX Buffer
            mov.w   #0220h,&DMA1DA          ; Dst address = RAM memory
            mov.w   #010h,&DMA1SZ           ; Size in words
            mov.w   #DMADSTINCR_3+DMASBDB+DMAIE+DMAEN,&DMA1CTL ; Sng, config
            bis.b   #CPUOFF+GIE,SR          ; Enter LPM0, enable interrupts
;------------------------------------------------------------------------------
DMA_ISR;    Common ISR for DMA/DAC12
;------------------------------------------------------------------------------
            bic.w   #DMAIFG,&DMA1CTL        ; Clear DMA1 interrupt flag
            bis.w   #DMAEN,&DMA0CTL         ; Enable
            bis.w   #DMAEN,&DMA1CTL         ; (re)Enable
            reti                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset, Watchdog, Flash
            .short  RESET                   ;
            .sect   ".int00"                ; DMA Vector
            .short  DMA_ISR                 ;
            .end

