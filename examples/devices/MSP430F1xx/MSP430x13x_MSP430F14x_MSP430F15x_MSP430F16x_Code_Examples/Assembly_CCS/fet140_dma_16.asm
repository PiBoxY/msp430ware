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
;  MSP-FET430P140 Demo - DMA0/1/2, USART1 SPI 3-Wire Master P1.x Exchange
;
;  Description: SPI Master communicates at fast as possible, full-duplex with
;  SPI Slave using 3-wire mode. The level on P1.4/5 is TX'ed and RX'ed to P1.0
;  and P1.1. Master will pulse slave Reset on init to insure synch start.
;  With data transfer driven directly by DMA - no CPU resources used.
;  DMA0 trigger from TX read, which uses MPY to shift read P1 left
;  DMA1 trigger from MPY moves shifted P1 data to the USART1 TX buffer
;  DMA2 trigger from USART1 RX buffer moves received data to P1
;  Master mode is LPM0.
;  ACLK = n/a, MCLK = SMCLK = DCO ~ 800kHz, ULCK = external
;  //* Final Production MSP430F16x(x) Device Required *//
;
;             fet140_dma_15              fet140_dma_16
;             MSP430F169 Slave           MSP430F169 Master
;             -----------------          -----------------
;            |              XIN|-    /|\|              XIN|-
;            |                 |      | |                 |
;            |             XOUT|-     --|RST          XOUT|-
;            |                 | /|\    |                 |
;            |              RST|--+<----|P5.0             |
;      LED <-|P1.0             |        |             P1.4|<-
;      LED <-|P1.1             |        |             P1.5|<-
;          ->|P1.4             |        |             P1.0|-> LED
;          ->|P1.5             |        |             P1.1|-> LED
;            |       SIMO0/P3.1|<-------|P5.1/SIMO1       |
;            |       SOMI0/P3.2|------->|P5.2/SOMI1       |
;            |        UCLK/P3.3|<-------|P5.3/UCLK1       |
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
            mov.b   #0h,&P1OUT              ; P1.0/1 setup for LED output
            mov.b   #03h,&P1DIR             ;
SetupP5     bis.b   #0Eh,&P5SEL             ; P5.1,2,3 SPI option select
            bic.b   #01h,&P5OUT
            bis.b   #01h,&P5DIR             ; Reset Slave with P5.0
            bic.b   #01h,&P5DIR             ;
SetupSPI1   bis.b   #CHAR+SYNC+MM+SWRST,&UCTL1 ; 8-bit SPI Master
            bis.b   #CKPL+SSEL1+STC,&UTCTL1 ; SMCLK, 3-pin
            mov.b   #02h,&UBR01             ; SMCLK/2 for baud rate
            clr.b   &UBR11                  ; SMCLK/2 for baud rate
            clr.b   &UMCTL1                 ; Clear modulation
            bis.b   #USPIE1,&ME2            ; Enable USART1 SPI
            bic.b   #SWRST,&UCTL1           ; **Initialize USART state machine**
            mov.w   #0FFFh,R15              ; Time for slave to ready
L$1         dec.w   R15                     ;
            jnz     L$1                     ;
            mov.w   #1000h,&MPY             ; MPY first operand
            mov.w   #DMA2TSEL_9+DMA1TSEL_11+DMA0TSEL_10,&DMACTL0 ; URXIFG1, MPY, UTXIFG1
RX_store    mov.w   #U1RXBUF, &DMA2SA       ; Src address = UART RX Buffer
            mov.w   #P1OUT, &DMA2DA         ; Dst address = P1
            mov.w   #01h,&DMA2SZ            ; Size in words
            mov.w   #DMADT_4+DMASBDB+DMAEN,&DMA2CTL ; Sng, config
TX_load     mov.w   #RESHI, &DMA1SA         ; Src address = multiplier
            mov.w   #U1TXBUF, &DMA1DA       ; Dst address = UART TX Buffer
            mov.w   #01h,&DMA1SZ            ; Size in words
            mov.w   #DMADT_4+DMASBDB+DMAEN,&DMA1CTL ; Sng, config
TX_Init     mov.w   #P1IN, &DMA0SA          ; Src address
            mov.w   #OP2, &DMA0DA           ; Dst address = multiplier
            mov.w   #01h,&DMA0SZ            ; Size in words
            mov.w   #DMADT_4+DMASBDB+DMALEVEL+DMAEN,&DMA0CTL ; Sng rpt, config
            bis.b   #LPM0+GIE,SR            ; Enter LPM0
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset, Watchdog, Flash
            .short  RESET                   ;
            .end

