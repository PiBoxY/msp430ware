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
;  MSP430F673x Demo - USCI_B0 I2C Master TX single bytes to MSP430 Slave
;
;  Description: This demo connects two MSP430's via the I2C bus. The master
;  transmits to the slave. This is the MASTER code. It continuously
;  transmits 00h, 01h, ..., 0ffh and demonstrates how to implement an I2C
;  master transmitter sending a single byte using the USCI_B0 TX interrupt.
;  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
;
;     ***to be used with "MSP430F673X_USCIB0_I2C_07.asm" ***
;
;                                /|\  /|\
;                MSP430F673x     10k  10k     MSP430F673x
;                   slave         |    |         master
;             -----------------   |    |   -----------------
;           -|XIN  P2.1/UCB0SDA|<-|----+->|P2.1/UCB0SDA  XIN|-
;            |                 |  |       |                 |
;           -|XOUT             |  |       |             XOUT|-
;            |     P2.0/UCB0SCL|<-+------>|P2.0/UCB0SCL     |
;            |                 |          |                 |
;
;  M. Swanson
;  Texas Instruments Inc.
;  December 2011
;  Built with CCS Version: 5.1.0
;*******************************************************************************
            .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
TXData      .equ    R5
TXByteCtr   .equ    R6
;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x5C00,SP                ; Initialize stackpointer
            mov.w   #WDTPW + WDTHOLD,&WDTCTL; Stop WDT

            bis.b   #BIT0 + BIT1,&P2SEL     ; Assign pins to I2C lines
            
            bis.w   #UCSWRST,&UCB0CTLW0     ; **Put state machine in reset**
            mov.w   #UCMST + UCMODE_3 + UCSSEL_2 + UCSWRST,&UCB0CTLW0
                                            ; I2C Master, synchronous mode
            mov.b   #12,&UCB0BRW_L          ; fSCL = SMCLK/12 = ~100kHz
            mov.b   #0,&UCB0BRW_H           ;
            mov.w   #0x48,&UCB0I2CSA        ; Slave Address is 0x48
            bic.w   #UCSWRST,&UCB0CTLW0     ; **Initialize USCI state machine**
            bis.w   #UCTXIE0,&UCB0IE         ; Enable TX interrupt

            clr.b   TXData                  ; Load TX data into R5

Mainloop    mov.w   #1,TXByteCtr            ; Load TX byte counter

Mainloop_1  bit.w   #UCTXSTP,&UCB0CTLW0      ; Loop until I2C STP got sent
            jc      Mainloop_1
            bis.w   #UCTR + UCTXSTT,&UCB0CTLW0; I2C TX, start condition

            bis.b   #LPM0 + GIE,SR          ; Enter LPM0 w/ interrupts
            nop                             ; Remain in LPM0 until all data
                                            ; is TX'd
            inc.b   TXData                  ; Increment data byte
            jmp     Mainloop

;-------------------------------------------------------------------------------
; The USCIAB0_ISR is structured such that it can be used to transmit any
; number of bytes by pre-loading TXByteCtr with the byte count.
;-------------------------------------------------------------------------------
USCI_B0_ISR;        USCI_B0 Interrupt Handler ISR
;-------------------------------------------------------------------------------
            add.w   &UCB0IV,PC              ; Add offset to PC
            reti                            ; Vector 0: No interrupt
            reti                            ; Vector 2: ALIFG
            reti                            ; Vector 4: NACKIFG
            reti                            ; Vector 6: STTIFG
            reti                            ; Vector 8: STPIFG
            reti                            ; Vector 10: RXIFG3
            reti                            ; Vector 12: TXIFG3
            reti                            ; Vector 14: RXIFG2
            reti                            ; Vector 16: TXIFG2
            reti                            ; Vector 18: RXIFG1
            reti                            ; Vector 20: TXIFG1
            reti                            ; Vector 22: RXIFG0
            jmp     TXIFG_ISR               ; Vector 24: TXIFG0
            reti                            ; Vector 26: BCNTIFG
            reti                            ; Vector 28: CLTOIFG
            reti                            ; Vector 30: BIT9IFG
                                            ;
TXIFG_ISR                                   ; TXIFG Interrupt Handler
            tst.w   TXByteCtr               ; Check TX byte counter
            jz      USCIAB0TX_ISR_1         ; Jump if all bytes were TX'd

            mov.w   TXData,&UCB0TXBUF       ; Load TX buffer

            dec.w   TXByteCtr               ; Decrement TX byte counter
            reti

USCIAB0TX_ISR_1
            bic.w   #UCTXIFG,&UCB0IFG       ; Clear USCI_B0 TX int flag
            bis.w   #UCTXSTP,&UCB0CTLW0      ; I2C stop condition
            bic.b   #LPM0,0(SP)             ; Exit LPM0
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
                  ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int58"
            .short  USCI_B0_ISR
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
