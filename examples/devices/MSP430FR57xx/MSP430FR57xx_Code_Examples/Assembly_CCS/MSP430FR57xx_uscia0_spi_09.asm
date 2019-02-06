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
;   MSP430F54x Demo - USCI_A0, SPI 3-Wire Master Incremented Data
;
;   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
;   data is sent by the master starting at 0x01. Received data is expected to
;   be same as the previous transmission TXData = RXData-1.  
;   USCI RX ISR is used to handle communication with the CPU, normally in LPM0. 
;   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~1MHz.  BRCLK = ACLK/2
;
;
;                   MSP430FR5739
;                 -----------------
;             /|\|                 |
;              | |                 |
;              --|RST              |
;                |                 |
;                |             P2.0|-> Data Out (UCA0SIMO)
;                |                 |
;                |             P2.1|<- Data In (UCA0SOMI)
;                |                 |
;                |             P1.5|-> Serial Clock Out (UCA0CLK)
;
;
;   Tyler Witt
;   Texas Instruments Inc.
;   September 2011
;   Built with Code Composer Studio V4.2
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
RXData  .set    R5
TXData  .set    R6
;------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #BIT0,&P1DIR
            bic.b   #BIT0,&P1OUT
            clr.w   RXData                  ; Initialize RXData
            
            bis.b   #BIT4+BIT5,&PJSEL0
            mov.b   #0xA5,&CSCTL0_H
            bis.w   #DCOFSEL0+DCOFSEL1,&CSCTL1 ; Set max DCO setting
            mov.w   #SELA_0+SELS_3+SELM_3,&CSCTL2 ; ACLK = XT1; MCLK = DCO
            mov.w   #DIVA_0+DIVS_3+DIVM_3,&CSCTL3 ; set all dividers
            bis.w   #XT1DRIVE_0,&CSCTL4
            bic.w   #XT1OFF,&CSCTL4
OSCFlag     bic.w   #XT1OFFG,&CSCTL5            ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            cmp.w   #001h,&OFIFG            ; Test oscillator fault flag
            jz      OSCFlag
            
            bis.b   #BIT5,&P1SEL1
            bis.b   #BIT0+BIT1,&P2SEL1
            bis.w   #UCSWRST,&UCA0CTLW0     ; **Put state machine in reset**
            bis.w   #UCMST+UCSYNC+UCCKPL+UCMSB,&UCA0CTLW0 ; 3-pin, 8-bit SPI master
                                                          ; Clock polarity high, MSB
            bis.w   #UCSSEL_1,&UCA0CTLW0    ; ACLK
            mov.b   #0x02,&UCA0BR0          ; /2
            clr.b   &UCA0BR1
            clr.w   &UCA0MCTLW              ; No modulation
            bic.w   #UCSWRST,&UCA0CTLW0     ; **Initialize USCI state machine**
            bis.w   #UCRXIE,&UCA0IE         ; Enable USCI_A0 RX interrupt
            mov.w   #0x01,TXData            ; Holds TX data
            
Mainloop    bis.w   #UCTXIE,&UCA0IE
            nop
            bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupt
            nop                             ; remain in LPM0
            mov.w   #1000,R15               ; Delay before next transmission
Delay       dec.w   R15
            jnz     Delay
            inc.w   TXData                  ; increment transmit data
            jmp     Mainloop
            nop                             ; for debug
            
;------------------------------------------------------------------------------
USCI_ISR ;  USCI Interrupt Service Routine
;------------------------------------------------------------------------------
            add     &UCA0IV,PC              ; Add offset to PC
            reti                            ; no interrupt
            jmp     Receive                 ; receive interrupt
            jmp     Transmit                ; transmit interrupt
Receive     mov.w   &UCA0RXBUF,RXData
            bic.w   #UCRXIFG,&UCA0IFG
            bic.w   #LPM0,0(SP)             ; Wake up to setup next TX
            reti
Transmit    mov.w   TXData,&UCA0TXBUF
            bic.w   #UCTXIE,&UCA0IE
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   USCI_A0_VECTOR          ; USCI_A0_VECTOR
            .short  USCI_ISR                ;
            .end
