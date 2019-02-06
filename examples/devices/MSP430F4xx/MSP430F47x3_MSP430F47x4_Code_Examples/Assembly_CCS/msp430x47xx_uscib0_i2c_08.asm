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
;   MSP430x47xx Demo - USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
;
;   Description: This demo connects two MSP430's via the I2C bus. The master
;   transmits to the slave. This is the master code. It continuously
;   transmits an array of data and demonstrates how to implement an I2C
;   master transmitter sending multiple bytes using the USCI_B0 TX interrupt.
;   ACLK = 32kHz, MCLK = SMCLK = TACLK = BRCLK = 1MHz
;
; ***to be used with "msp430x47xx_uscib0_i2c_09.asm" ***
;
;                                 /|\  /|\
;                MSP430x47xx      10k  10k     MSP430x47xx
;                    slave         |    |        master
;              -----------------   |    |  -----------------
;            -|XIN  P3.1/UCB0SDA|<-|---+->|P3.1/UCB0SDA  XIN|-
;       32kHz |                 |  |      |                 | 32kHz
;            -|XOUT             |  |      |             XOUT|-
;             |     P3.2/UCB0SCL|<-+----->|P3.2/UCB0SCL     |
;             |                 |         |                 |
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
			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #900h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP3     bis.b   #BIT1+BIT2,&P3SEL       ; Assign I2C pins to USCI_B0
SetupUCB0   bis.b   #UCSWRST,&UCB0CTL1      ; Enable SW reset
            mov.b   #UCMST+UCMODE_3+UCSYNC,&UCB0CTL0
                                            ; I2C Master, synchronous mode
            mov.b   #UCSSEL_2+UCSWRST,&UCB0CTL1
                                            ; Use SMCLK, keep SW reset
            mov.b   #11,&UCB0BR0            ; fSCL = SMCLK/11 = 95.3kHz
            mov.b   #00,&UCB0BR1
            mov.w   #048h,&UCB0I2CSA        ; Slave Address is 048h
            bic.b   #UCSWRST,&UCB0CTL1      ; Clear SW reset, resume operation
            bis.b   #UCB0TXIE,&IE2          ; Enable TX interrupt

Main        mov.w   #TxData,R5              ; TX array start address
            mov.w   #5,R6                   ; Load TX byte counter
            bis.b   #UCTR+UCTXSTT,&UCB0CTL1 ; I2C TX, start condition
            bis.b   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
                                            ; Remain in LPM0 until all data
                                            ; is TX'd

Main_1      bit.b   #UCTXSTP,&UCB0CTL1      ; Loop until STP is TX'd
            jc      Main_1
            jmp     Main                    ; Repeat
;-------------------------------------------------------------------------------
; The USCIAB0TX_ISR is structured such that it can be used to transmit any
; number of bytes by pre-loading R6 with the byte count. Also, R5 points to
; the next byte to transmit.
;-------------------------------------------------------------------------------
USCIAB0TX_ISR;      USCI_B0 Data ISR
;-------------------------------------------------------------------------------
            tst.w   R6                      ; Check TX byte counter
            jz      USCIAB0TX_ISR_1         ; Jump if all bytes were TX'd
            mov.b   @R5+,&UCB0TXBUF         ; Load TX buffer
            dec.w   R6                      ; Decrement TX byte counter
            reti

USCIAB0TX_ISR_1
            bis.b   #UCTXSTP,&UCB0CTL1      ; I2C stop condition
            bic.b   #UCB0TXIFG,&IFG2        ; Clear USCI_B0 TX int flag
            bic.w   #LPM0,0(SP)             ; Clear LPM0
            reti
;-------------------------------------------------------------------------------
TxData;     Table of data to transmit
;-------------------------------------------------------------------------------
            .byte      011h
            .byte      022h
            .byte      033h
            .byte      044h
            .byte      055h
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int08"		        ; USCI_B0 I2C Data Int Vector
            .short  USCIAB0TX_ISR
            .sect   ".reset"            ; POR, ext. Reset, Watchdog
            .short  RESET
            .end

