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
;   MSP430x47x Demo - USCI_A0 IrDA External Loopback Test, 4MHz SMCLK
;
;   Description: This example transmits bytes through the USCI module
;   configured for IrDA mode, and receives them using an external loopback
;   connection. The transfered sequence is 00h, 01h, 02h, ..., ffh. The
;   received bytes are also stored in memory starting at address RxData.
;   In the case of an RX error the LED is lighted and program execution stops.
;   An external loopback connection has been used as it allows for the
;   connection of a scope to monitor the communication, which is not possible
;   when using the internal loopback.
;
;   MCLK = SMCLK = DCO = 4MHz, ACLK = 32kHz
;   //* An external 32kHz XTAL on XIN XOUT is required for ACLK *//
;
;              MSP430x47x
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |          UCA0RXD|--+   external
;            |          UCA0TXD|--+   loopback connection
;            |                 |
;            |                 |
;            |             P4.6|--->  LED
;            |                 |
;
;   P.Thanigai
;   Texas Instruments Inc.
;   September 2008
;   Built with Code Composer Essentials Version: 3.0
;******************************************************************************
 .cdecls C,LIST, "msp430.h" 
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
 
RxData      .usect ".bss",256                     ; Allocate 256 byte of RAM
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP2     bis.b   #BIT4+BIT5,&P2SEL       ; Use P2.4/P2.5 for USCI_A0
SetupP4     bic.b   #BIT6,&P4OUT            ; Clear P4.6
            bis.b   #BIT6,&P4DIR            ; P4.6 output
SetupOSC    mov.b   #121,&SCFQCTL           ; Set FLL to 3.998MHz
            mov.b   #FLLD0+FN_2,&SCFI0      ; Adjust range select
SetupUCA0   bis.b   #UCSWRST,&UCA0CTL1      ; Set USCI_A0 SW Reset
            mov.b   #UCSSEL_2+UCSWRST,&UCA0CTL1
                                            ; Use SMCLK, keep SW reset
            mov.b   #26,&UCA0BR0            ; 4Mhz/26=153.8kHz
            mov.b   #0,&UCA0BR1
            mov.b   #UCBRF_1+UCOS16,&UCA0MCTL
                                            ; Set 1st stage modulator to 1
                                            ; 16-times oversampling mode
            mov.b   #UCIRTXPL2+UCIRTXPL0+UCIRTXCLK+UCIREN,&UCA0IRTCTL
                                            ; Pulse length = 6 half clock cyc
                                            ; Enable BITCLK16, IrDA enc/dec
            bic.b   #UCSWRST,&UCA0CTL1      ; Resume USCI_A0 operation
            bis.b   #UCA0RXIE,&IE2          ; Enable RX int

            clr.w   R4                      ; Init delay counter
            clr.b   R5                      ; TX data and pointer, 8-bit

Mainloop    inc.w   R4                      ; Small delay
            cmp.w   #1000,R4
            jne     Mainloop
            clr.w   R4                      ; Re-init delay counter

TX          bit.b   #UCA0TXIFG,&IFG2        ; USCI_A0 TX buffer ready?
            jnc     TX                      ; Loop if not
            mov.b   R5,&UCA0TXBUF           ; TX character

            dint
            bis.b   #UCA0RXIE,&IE2          ; Enable RX int
            bis.w   #CPUOFF+GIE,SR          ; Enter LPM0, interrupts enabled

            mov.b   R6,RxData(R5)           ; Store RXed character in RAM
            cmp.b   R5,R6                   ; RX OK?
            jeq     RX_OK

RX_ERROR    bis.b   #BIT6,&P4OUT            ; LED P4.6 on
            jmp     $                       ; Trap PC here

RX_OK       inc.b   R5                      ; Next character to TX
            jmp     Mainloop                ; Again
;-------------------------------------------------------------------------------
USCIRX_ISR; Clear UCA0RXIFG interrupt flag and return active
;-------------------------------------------------------------------------------
            mov.b   &UCA0RXBUF,R6           ; Get RXed character
            bic.b   #UCA0RXIE,&IE2          ; Disable RX int
            bic.w   #CPUOFF,0(SP)           ; Return active after receiption
            reti                            ; Return from ISR
;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect    ".int09"       		;
            .short   USCIRX_ISR             ; USCI receive
            .sect     ".reset"            	; POR, ext. Reset, Watchdog
            .short   RESET                  ;
            .end
