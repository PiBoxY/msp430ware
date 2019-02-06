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
;   MSP430xG461x Demo - USCI_A0 IrDA Monitor, 4MHz SMCLK
;
;   Description: This example receives bytes through the USCI module
;   configured for IrDA mode, and sends them out as ASCII strings using UART1
;   to a PC running a terminal software. The code can be used to monitor
;   and log an IrDA communication.
;
;   MCLK = SMCLK = DCO = 4MHz, ACLK = 32kHz
;   //* An external 32kHz XTAL on XIN XOUT is required for ACLK *//
;
;                                     MSP430xG461x
;                                -----------------------
;                               |                       |
;                            /|\|                    XIN|-
;                             | |                       | 32kHz
;                             --|RST                XOUT|-
;                               |                       |
;     GP2W0116YPS   /|\         |                       |
;       -------      |          |                       |
;      |    Vcc|-----+  IrDA    |                       |
;      #    LED|-----+ 9600 8N1 |                       |
;      #    TxD|<---------------|P2.4/UCA0TXD           |
;      #    RxD|--------------->|P2.5/UCA0RXD      UTXD1|--> 115,200 8N1
;      #     SD|-----+          |                       |    Terminal SW
;      |    GND|-----+          |                       |
;       -------      |           -----------------------
;                   ---
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
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #900,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP2     bis.b   #030h,&P2SEL            ; Use P2.4/P2.5 for USCI_A0
SetupP4     bis.b   #03h,&P4SEL             ; P4.0,1 = USART1 TXD/RXD
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
            mov.b   #UCIRRXPL,&UCA0IRRCTL   ; Light = low pulse
            bic.b   #UCSWRST,&UCA0CTL1      ; Resume USCI_A0 operation
SetupUSART1 bis.b   #SWRST,&U1CTL           ; Set USART1 SW Reset
            bis.b   #UTXE1+URXE1,&ME2       ; Enable USART1 TXD/RXD
            mov.b   #CHAR+SWRST,&U1CTL      ; 8-bit characters, keep SW reset
            mov.b   #SSEL1,&U1TCTL          ; BRCLK = SMCLK
            mov.b   #022h,&U1BR0            ; 4MHz/115,200=34.72
            mov.b   #000h,&U1BR1            ;
            mov.b   #0ddh,&U1MCTL           ; Modulation
            bic.b   #SWRST,&U1CTL           ; Release USART1 state machine

Mainloop    dint
            bis.b   #UCA0RXIE,&IE2          ; Enable RX int
            bis.w   #CPUOFF+GIE,SR          ; Enter LPM0, interrupts enabled
            mov.b   R5,R6                   ; Save received character
            clrc
            rrc.b   R5                      ; R5 >>= 4
            rra.b   R5
            rra.b   R5
            rra.b   R5
Rdy4TX1     bit.b   #UTXIFG1,&IFG2
            jnc     Rdy4TX1
            mov.b   Nibble2ASCII(R5),&U1TXBUF
                                            ; TX upper nibble
Rdy4TX2     bit.b   #UTXIFG1,&IFG2
            jnc     Rdy4TX2
            and.b   #0fh,R6
            mov.b   Nibble2ASCII(R6),&U1TXBUF
                                            ; TX lower nibble
Rdy4TX3     bit.b   #UTXIFG1,&IFG2
            jnc     Rdy4TX3
            mov.b   #' ',&U1TXBUF           ; TX space character
            jmp     Mainloop                ; Again
;-------------------------------------------------------------------------------
Nibble2ASCII;  Table for nibble-to-ASCII conversion
;-------------------------------------------------------------------------------
            .byte      '0'
            .byte      '1'
            .byte      '2'
            .byte      '3'
            .byte      '4'
            .byte      '5'
            .byte      '6'
            .byte      '7'
            .byte      '8'
            .byte      '9'
            .byte      'A'
            .byte      'B'
            .byte      'C'
            .byte      'D'
            .byte      'E'
            .byte      'F'
;-------------------------------------------------------------------------------
USCIRX_ISR; Read RXed character, return from LPM0
;-------------------------------------------------------------------------------
            mov.b   &UCA0RXBUF,R5           ; Get RXed character
            bic.b   #UCA0RXIE,&IE2          ; Disable RX int
            bic.w   #CPUOFF,0(SP)           ; Return active after receiption
            reti                            ; Return from ISR
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect     ".int25"        ; USCI A0/B0 Receive
            .short    USCIRX_ISR
            .sect	  ".reset"            ; POR, ext. Reset
            .short    RESET
            .end
