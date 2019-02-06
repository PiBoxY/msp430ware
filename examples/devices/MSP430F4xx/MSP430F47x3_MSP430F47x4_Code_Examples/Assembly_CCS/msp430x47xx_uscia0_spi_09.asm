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
;   MSP430x47xx Demo - USCI_A0, SPI 3-Wire Master Incremented Data
;
;   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
;   data is sent by the master starting at 0x01. Received data is expected to
;   be same as the previous transmission.  USCI RX ISR is used to handle
;   communication with the CPU, normally in LPM0. If high, P5.1 indicates
;   valid data reception.  Because all execution after LPM0 is in ISRs,
;   initialization waits for DCO to stabilize against ACLK.
;   ACLK = 32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz, BRCLK = SMCLK/2
;
;   Use with SPI Slave Data Echo code example.  If slave is in debug mode, P5.2
;   slave reset signal conflicts with slave's JTAG; to work around, use IAR's
;   "Release JTAG on Go" on slave device.  If breakpoints are set in
;   slave RX ISR, master must stopped also to avoid overrunning slave
;   RXBUF.
;
;                    MSP430x47xx
;                 -----------------
;             /|\|              XIN|-
;              | |                 |  32kHz xtal
;              --|RST          XOUT|-
;                |                 |
;                |             P2.4|-> Data Out (UCA0SIMO)
;                |                 |
;          LED <-|P5.1         P2.5|<- Data In (UCA0SOMI)
;                |                 |
;  Slave reset <-|P5.2         P3.0|-> Serial Clock Out (UCA0CLK)
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
SLV_Data	.equ   R7
;-------------------------------------------------------------------------------
			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #900h,SP         		; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupFLL    bis.b   #XCAP11PF,&FLL_CTL0     ; Configure load caps

OFIFGcheck  bic.b   #OFIFG,&IFG1            ; Clear OFIFG
            mov.w   #047FFh,R15             ; Wait for OFIFG to set again if
OFIFGwait   dec.w   R15                     ; not stable yet
            jnz     OFIFGwait
            bit.b   #OFIFG,&IFG1            ; Has it set again?
            jnz     OFIFGcheck              ; If so, wait some more

            mov.w   #2100,R15               ; Now with stable ACLK, wait for
DCO_delay   dec.w   R15                     ; DCO to stabilize.
            jnz     DCO_delay               ;

SetupP5     mov.b   #04h,&P5OUT             ; P5 setup for LED and slave reset
            bis.b   #06h,&P5DIR             ;

SetupP2     bis.b   #BIT4+BIT5,&P2SEL       ; UCAOSPI option select
SetupP3     bis.b   #BIT0,&P3SEL
SetupSPI    bis.b   #UCMST+UCSYNC+UCCKPL+UCMSB,&UCA0CTL0;3-pin, 8-bit SPI master
            bis.b   #UCSSEL_2,&UCA0CTL1     ; SMCLK
            mov.b   #02h,&UCA0BR0           ; /2
            clr.b   &UCA0BR1                ;
            clr.b   &UCA0MCTL               ; No modulation
            bic.b   #UCSWRST,&UCA0CTL1      ; **Initialize USCI state machine**
            bis.b   #UCA0RXIE,&IE2          ; Enable USCI_A0 RX interrupt

            bic.b   #04h,&P5OUT             ; Now with SPI signals initialized,
            bis.b   #04h,&P5OUT             ; reset slave
            mov.w   #50,R15                 ; Wait for slave to initialize
waitForSlv  dec.w   R15                     ;
            jnz     waitForSlv

            mov.b   #001h,MST_Data          ; Initialize data values
            mov.b   #000h,SLV_Data          ;

Mainloop    mov.b   MST_Data,&UCA0TXBUF     ; Transmit first character
            bis.b   #LPM0+GIE,SR            ; CPU off, enable interrupts
            nop                             ; Required for debugger only
                                            ;
;------------------------------------------------------------------------------
USCIA0RX_ISR;       Test for valid RX and TX character
;------------------------------------------------------------------------------
TX1         bit.b   #UCA0TXIFG,&IFG2        ; USCI_A0 TX buffer ready?
            jz      TX1                     ; Jump is TX buffer not ready

            cmp.b   SLV_Data,&UCA0RXBUF     ; Test for correct character RX'd
            jeq     PASS_TEST
FAIL_TEST   bic.b   #02h,&P5OUT             ; If incorrect, clear LED
            jmp     TX2
PASS_TEST   bis.b   #02h,&P5OUT             ; If correct, light LED
TX2         inc.b   MST_Data                ; Increment master value
            inc.b   SLV_Data                ; Increment expected slave value
            mov.b   MST_Data,&UCA0TXBUF        ; Send next value

            mov.w   #30,R15                 ; Add time between transmissions to
cycleDelay  dec.w   R15                     ; make sure slave can keep up
            jnz     cycleDelay

            reti                            ; Exit ISR

;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".int09"        ; USCI_A0 Rx Vector
            .short  USCIA0RX_ISR            ;
            .sect	".reset"            ; RESET Vector
            .short  RESET                   ;
            .end
