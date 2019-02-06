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
;  MSP430FR59xx Demo - eUSCI_A0 UART echo at 9600 baud using BRCLK = 32768Hz
;
;  Description: This demo echoes back characters received via a PC serial port.
;  ACLK is used as a clock source and the device is put in LPM3
;  Note that level shifter hardware is needed to shift between RS232 and MSP
;  voltage levels.
;
;  The example code shows proper initialization of registers
;  and interrupts to receive and transmit data.
;  To test code in LPM3, disconnect the debugger and use an external supply
;  Otherwise use LPM0
;
;  ACLK = 32768Hz, MCLK =  SMCLK = default DCO
;
;                MSP430FR5969
;             -----------------
;            |                 |--LFXTIN (32768Hz reqd.)
;            |                 |--LFXTOUT
;            |                 |
;       RST -|     P2.0/UCA0TXD|----> PC (echo)
;            |                 |
;            |                 |
;            |     P2.1/UCA0RXD|<---- PC
;            |                 |
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
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupGPIO   bis.b   #BIT0+BIT1,&P2SEL1      ; USCI_A0 UART operation
            bic.b   #BIT0+BIT1,&P2SEL0
            bis.w   #BIT4+BIT5,&PJSEL0      ; For XT1

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

SetupCS     mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2
            mov.w   #DIVA__1+DIVS__1+DIVM__1,&CSCTL3 ; Set all dividers
            bic.w   #LFXTOFF,&CSCTL4

OSCFlag     bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            clr.b   &CSCTL0_H               ; Lock CS registers

SetupUART   mov.w   #UCSWRST,&UCA0CTLW0     ; Put eUSCI in reset
            bis.w   #UCSSEL__ACLK,&UCA0CTLW0 ; CLK = ACLK
            mov.b   #3, &UCA0BR0            ; 9600 baud
            bis.w   #0x5300,&UCA0MCTLW      ; 32768/9600 - INT(32768/9600)=0.41
                                            ; UCBRSx value = 0x53 (See UG)
            clr.b   &UCA0BR1
            bic.b   #UCSWRST,&UCA0CTL1      ; Initialize eUSCI
            bis.w   #UCRXIE,&UCA0IE         ; Enable USCI_A0 RX interrupt
            nop                             ; 
            bis.w   #LPM3+GIE,SR            ; Enter LPM3, interrupts enabled
            nop                             ; For debugger

;------------------------------------------------------------------------------
USCI_A0_ISR;    USCI A0 Receive/Transmit Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &UCA0IV,PC              ; add offset to PC
            reti                            ; Vector 0: No interrupts
            jmp     Receive                 ; Vector 2: USCI UCRXIFG
            reti                            ; Vector 4: USCI UCTXIFG
            reti                            ; Vector 6: USCI UCSTTIFG
            reti                            ; Vector 8: USCI UCTXCPTIFG
Receive     bit.w   #UCTXIFG,&UCA0IFG
            jeq     Receive
            mov.w   &UCA0RXBUF,&UCA0TXBUF
            nop
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   USCI_A0_VECTOR          ; USCI A0 Receive/Transmit Vector
            .short  USCI_A0_ISR
            .end
