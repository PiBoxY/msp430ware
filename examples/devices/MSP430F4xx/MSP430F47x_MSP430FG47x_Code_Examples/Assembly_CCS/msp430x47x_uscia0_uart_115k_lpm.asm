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
;    MSP430x47x Demo - USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM3
;
;   Description: Echo a received character, RX ISR used. Normal mode is LPM3.
;   Automatic clock activation for SMCLK through the USCI is demonstrated.
;   USCI_A0 RX interrupt triggers TX Echo.
;   Baud rate divider with 1048576hz = 1048576/115200 = ~9.1 (009h|01h)
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   //* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;                MSP430x47x
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |     P2.4/UCA0TXD|------------>
;            |                 | 115200 - 8N1
;            |     P2.5/UCA0RXD|<------------
;
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
;-------------------------------------------------------------------------------
			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps

OFIFGcheck  bic.b   #OFIFG,&IFG1            ; Clear OFIFG
            mov.w   #047FFh,R15             ; Wait for OFIFG to set again if
OFIFGwait   dec.w   R15                     ; not stable yet
            jnz     OFIFGwait
            bit.b   #OFIFG,&IFG1            ; Has it set again?
            jnz     OFIFGcheck              ; If so, wait some more

SetupP2     bis.b   #BIT4+BIT5,&P2SEL       ; P2.4,5 USCI option select
SetupUSCIA0 bis.b   #UCSSEL_2,&UCA0CTL1     ; SMCLK
            mov.b   #0x09,&UCA0BR0          ; 1MHz 115200
            mov.b   #00,&UCA0BR1            ; 1MHz 115200
            mov.b   #02,&UCA0MCTL           ; Modulation
            bic.b   #UCSWRST,&UCA0CTL1      ; **Initialize USCI state machine**
            bis.b   #UCA0RXIE,&IE2          ; Enable USCI_A0 RX interrupt
                                            ;
Mainloop    bis.b   #LPM3+GIE,SR            ; Enter LPM3, interrupts enabled
            nop                             ; Needed only for debugger
                                            ;
;------------------------------------------------------------------------------
USCIA0RX_ISR;  Echo back RXed character, confirm TX buffer is ready first
;------------------------------------------------------------------------------
TX0         bit.b   #UCA0TXIFG,&IFG2        ; USCI_A0 TX buffer ready?
            jz      TX0                     ; Jump if TX buffer not ready
            mov.b   &UCA0RXBUF,&UCA0TXBUF   ; TX -> RXed character
            reti                            ;
                                            ;
;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect    ".int09"       		;
            .short   USCIA0RX_ISR           ; USCI receive
            .sect    ".reset"            	; POR, ext. Reset, Watchdog
            .short   RESET                  ;
            .end

