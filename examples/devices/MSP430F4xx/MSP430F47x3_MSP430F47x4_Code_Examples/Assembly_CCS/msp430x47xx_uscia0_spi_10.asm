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
;   MSP430x47xx Demo - USCI_A0, SPI 3-Wire Slave Data Echo
;
;   Description: SPI slave talks to SPI master using 3-wire mode. Data received
;   from master is echoed back.  USCI RX ISR is used to handle communication,
;   CPU normally in LPM4.  Prior to initial data exchange, master pulses
;   slaves RST for complete reset.
;   ACLK = 32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz
;
;   Use with SPI Master Incremented Data code example.  If the slave is in
;   debug mode, the reset signal from the master will conflict with slave's
;   JTAG; to work around, use IAR's "Release JTAG on Go" on slave device.  If
;   breakpoints are set in slave RX ISR, master must stopped also to avoid
;   overrunning slave RXBUF.
;
;                MSP430x47xx
;             -----------------
;         /|\|              XIN|-
;          | |                 |   32kHz xtal
;          | |             XOUT|-
; Master---+-|RST              |
;            |             P2.4|<- Data In (UCA0SIMO)
;            |                 |
;            |             P2.5|-> Data Out (UCA0SOMI)
;            |                 |
;            |             P3.0|<- Serial Clock In (UCA0CLK)
;
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

MST_Data	.equ   R6
SLV_Data    .equ   R7

;-------------------------------------------------------------------------------
			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #900h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupFLL    bis.b   #XCAP11PF,&FLL_CTL0     ; Configure load caps

OFIFGcheck  bic.b   #OFIFG,&IFG1            ; Clear OFIFG
            mov.w   #047FFh,R15             ; Wait for OFIFG to set again if
OFIFGwait   dec.w   R15                     ; not stable yet
            jnz     OFIFGwait
            bit.b   #OFIFG,&IFG1            ; Has it set again?
            jnz     OFIFGcheck              ; If so, wait some more

            mov.w   #2100,R15               ; Now with stable ACLK, wait for
DCO_delay   dec.w   R15                     ; DCO to stabilize
            jnz     DCO_delay

waitForMstr bit.b   #BIT0,&P3IN             ; If clock sig from mstr stays low,
            jz      waitForMstr             ; it is not yet in SPI mode

SetupP2     bis.b   #BIT4+BIT5,&P2SEL       ; UCAOSPI option select
SetupP3     bis.b   #BIT0,&P3SEL            
SetupSPI    mov.b   #UCSWRST,&UCA0CTL1      ; **Put state machine in reset**
            bis.b   #UCSYNC+UCCKPL+UCMSB,&UCA0CTL0;3-pin, 8-bit SPI master
            bic.b   #UCSWRST,&UCA0CTL1      ; **Initialize USCI state machine**
            bis.b   #UCA0RXIE,&IE2          ; Enable USCI_A0 RX interrupt

Mainloop    bis.b   #LPM4+GIE,SR            ; Enter LPM4, enable interrupts
            nop                             ; Required for debugger only
                                            ;
;------------------------------------------------------------------------------
USCIA0RX_ISR;       Test for valid RX and TX character
;------------------------------------------------------------------------------
TX1         bit.b   #UCA0TXIFG,&IFG2
            jz      TX1
            mov.b   &UCA0RXBUF,&UCA0TXBUF
            reti
;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect	".int09"        ; USCI_A0 Rx Vector
            .short	USCIA0RX_ISR         	;
            .sect	".reset"            ; RESET Vector
            .short	RESET                   ;
            .end

