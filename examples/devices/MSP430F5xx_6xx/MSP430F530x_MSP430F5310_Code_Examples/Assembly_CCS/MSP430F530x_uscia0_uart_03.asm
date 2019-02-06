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
;  MSP430F530x Demo - USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
;
;  Description: Echo a received character, RX ISR used. Normal mode is LPM3,
;  USCI_A0 RX interrupt triggers TX Echo.
;  ACLK = 32768Hz crystal, MCLK = SMCLK = DCO ~1.045MHz
;  Baud rate divider with 32768Hz XTAL @9600 = 32768Hz/9600 = 3.41
;  See User Guide for baud rate divider table
;
;                MSP430F530x
;             -----------------
;        /|\ |              XIN|-
;         |  |                 | 32kHz
;         ---|RST          XOUT|-
;            |                 |
;            |     P3.3/UCA0TXD|------------>
;            |                 | 9600 - 8N1
;            |     P3.4/UCA0RXD|<------------
;
;  K. Chen
;  Texas Instruments Inc.
;  March 2012
;  Built with CCS Version: 5.2
;*******************************************************************************

 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
;-------------------------------------------------------------------------------
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
            mov.w   #WDTPW | WDTHOLD,&WDTCTL; Stop WDT

            mov.b   #BIT3|BIT4,&P3SEL       ; P3.3,4 = USCI_A0 TXD/RXD
            bis.b   #UCSWRST,&UCA0CTL1      ; **Put state machine in reset**
            bis.b   #UCSSEL_1,&UCA0CTL1     ; CLK = ACLK
            mov.b   #0x03,&UCA0BR0          ; 32kHz/9600=3.41 (see User's Guide)
            mov.b   #0x00,&UCA0BR1
            mov.b   #UCBRS_3 | UCBRF_0,&UCA0MCTL
                                            ; Modulation UCBRSx=3, UCBRFx=0
            bic.b   #UCSWRST,&UCA0CTL1      ; **Initialize USCI state machine**
            bis.b   #UCRXIE,&UCA0IE         ; Enable USCI_A0 RX interrupt
            nop
            bis.b   #LPM3 | GIE,SR          ; Enter LPM3, interrupts enabled
            nop                             ; For debugger

;-------------------------------------------------------------------------------
USCI_A0_ISR
;-------------------------------------------------------------------------------
            ; Echo back RXed character, confirm TX buffer is ready first
            add.w   &UCA0IV,PC      
            reti                            ; Vector 0 - no interrupt
            jmp     RXIFG_HND               ; Vector 2 - RXIFG
            reti                            ; Vector 4 - TXIFG

RXIFG_HND
wait_TX_rdy bit.b   #UCTXIFG,&UCA0IFG       ; USCI_A0 TX buffer ready? 
            jnc     wait_TX_rdy
            mov.b   &UCA0RXBUF,&UCA0TXBUF   ; RXBUF -> TXBUF
            reti                            ; Return from interrupt 

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   USCI_A0_VECTOR
            .short  USCI_A0_ISR
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end