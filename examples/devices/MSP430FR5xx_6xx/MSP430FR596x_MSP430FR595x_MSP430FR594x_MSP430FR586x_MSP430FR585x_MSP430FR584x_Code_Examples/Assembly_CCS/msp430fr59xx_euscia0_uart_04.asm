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
;  MSP430FR59xx Demo - eUSCI_A0 External Loopback test
;
;  Description: This demo connects TX to RX of the MSP430 UART
;  The example code shows proper initialization of registers
;  and interrupts to receive and transmit data. If data is incorrect P1.0 LED is
;  turned ON.
;  ACLK = 32768Hz, MCLK = SMCLK = BRCLK = default DCO = ~1MHz
;
;
;               MSP430FR5969
;             -----------------
;            |              XIN|-
;            |                 |  32KHz Crystal
;            |             XOUT|-
;         /|\|                 |
;          | |                 |
;          --|RTS  P2.0/UCA0TXD|----|
;            |                 |    |
;            |                 |    |
;            |     P2.1/UCA0RXD|----|
;            |                 |
;            |             P1.0|---> LED
;
;   E. Chen
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio V5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
RXData      .set    R5
TXData      .set    R6
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupGPIO   bic.b   #BIT0,&P1OUT            ; Clear P1.0 output latch
            bis.b   #BIT0,&P1DIR            ; For LED on P1.0
            bis.b   #BIT0+BIT1,&P2SEL1      ; Configure UART pins
            bic.b   #BIT0+BIT1,&P2SEL0
            bis.w   #BIT4+BIT5,&PJSEL0      ; Configure XT1 pins

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

SetupCS     mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #DCOFSEL_0,&CSCTL1      ; Set DCO to 1MHz
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2
            mov.w   #DIVA__1+DIVS__1+DIVM__1,&CSCTL3 ; Set all dividers
            bic.w   #LFXTOFF,&CSCTL4        ; Enable LFXT1

OSCFlag     bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            clr.b   &CSCTL0_H               ; Lock CS registers

SetupUART   bis.b   #UCSWRST,&UCA0CTL1      ; Put eUSCI in reset
            mov.b   #UCSSEL__SMCLK,&UCA0CTL1 ; Set ACLK = 32768 as UCBRCLK
            mov.b   #3, &UCA0BR0            ; 9600 baud
            bis.w   #0x5300,&UCA0MCTLW      ; 32768/9600 - INT(32768/9600)=0.41
                                            ; UCBRSx value = 0x53 (See UG)
            clr.b   &UCA0BR1
            bic.b   #UCSWRST,&UCA0CTL1      ; release from reset
            bis.w   #UCRXIE,&UCA0IE         ; Enable USCI_A0 RX interrupt
            mov.w   #0,RXData               ; RXData = 0
            mov.w   #1,TXData               ; TXData = 1

Mainloop    bit.w   #UCTXIFG,&UCA0IFG
            jeq     Mainloop
            mov.w   TXData,&UCA0TXBUF       ; Load data onto buffer
            nop                             ; 
            bis.w   #LPM0+GIE,SR            ; Enter LPM0, interrupts enabled
            nop                             ; For debugger
            jmp     Mainloop

;------------------------------------------------------------------------------
USCI_A0_ISR;    USCI A0 Receive/Transmit Interrupt Service Routine
;------------------------------------------------------------------------------
            bic.w   #UCRXIFG,&UCA0IFG       ; Clear interrupt
            mov.w   &UCA0RXBUF,RXData       ; Clear buffer
            cmp.b   RXData,TXData           ; Check value
            jne     LEDOn
            inc.w   TXData                  ; increment TX data
            bic.w   #LPM0,0(SP)             ; Exit LPM0 on reti
            reti
LEDOn       bis.b   #BIT0,&P1OUT            ; If incorrect turn on P1.0
TrapCPU     jmp     TrapCPU                 ; Trap CPU
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   USCI_A0_VECTOR          ; USCI A0 Receive/Transmit Vector
            .short  USCI_A0_ISR
            .end
