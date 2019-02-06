; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2013, Texas Instruments Incorporated
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
;  MSP430i20xx Demo - eUSCI_A0, SPI 3-Wire Slave Data Echo
;
;  Description: SPI slave talks to SPI master using 3-wire mode. Data received
;  from master is echoed back.
;
;  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;  Note: Ensure slave is powered up before master to prevent delays due to
;  slave init
;
;           MSP430i20xx
;         ----------------
;     /|\|                |
;      | |                |
;      --|RST             |
;        |            P1.3|<-- Data In         (UCA0SIMO)
;        |            P1.2|--> Data Out      (UCA0SOMI)
;        |            P1.1|<-- Serial Clock (UCA0CLK)
;
;   T. Witt
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio v5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

            .ref    init                    ; Reference external function

RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
Cal         call    #init                   ; Calibrate peripherals
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #BIT1|BIT2|BIT3,&P1SEL0 ; eUSCI_A0 Pin Function
            bic.b   #BIT1|BIT2|BIT3,&P1SEL1
            mov.w   #UCSWRST,&UCA0CTLW0     ; eUSCI in reset
            bis.w   #UCSYNC|UCCKPL|UCMSB,&UCA0CTLW0
            ; 3-pin, 8-bit SPI slave, Clock polarity high, MSB
            bis.w   #UCSSEL_1,&UCA0CTLW0    ; ACLK
            mov.b   #0x02,&UCA0BR0
            clr.b   &UCA0BR1
            bic.w   #UCSWRST,&UCA0CTLW0     ; Initialize eUSCI
            bis.w   #UCRXIE,&UCA0IE         ; Enable RX interrupt

Mainloop    bis.w   #LPM0|GIE,SR            ; Enter LPM0 with interrupts
            nop                             ; For debugger
                                            ;
;-------------------------------------------------------------------------------
USCI_A0_ISR    ; eUSCI_A0 ISR
;-------------------------------------------------------------------------------
            add.w   &UCA0IV,PC              ; Add interrupt offset
            reti                            ; No interrupt
            jmp     RXIFG_                  ; RXIFG
            reti                            ; TXIFG
RXIFG_      bit.w   #UCTXIFG,&UCA0IFG       ; eUSCI_A0 TX buffer ready?
            jeq     RXIFG_
            mov.b   &UCA0RXBUF,&UCA0TXBUF   ; Echo received data
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int09"                ; eUSCI_A0 Vector
            .short  USCI_A0_ISR
            .end
