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
;   MSP430x24x Demo - USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
;
;   Description: This demo connects two MSP430's via the I2C bus. The slave
;   transmits to the master. This is the master code. It continuously
;   receives an array of data and demonstrates how to implement an I2C
;   master receiver receiving multiple bytes using the USCI_B0 TX interrupt.
;   ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
;
; ***to be used with "msp430x24x_uscib0_i2c_11.asm" ***
;
;                                 /|\  /|\
;                MSP430F249       10k  10k     MSP430F249
;                    slave         |    |        master
;              -----------------   |    |  -----------------
;            -|XIN  P3.1/UCB0SDA|<-|---+->|P3.1/UCB0SDA  XIN|-
;             |                 |  |      |                 | 32kHz
;            -|XOUT             |  |      |             XOUT|-
;             |     P3.2/UCB0SCL|<-+----->|P3.2/UCB0SCL     |
;             |                 |         |             P1.0|--> LED
;
;  JL Bile
;  Texas Instruments Inc.
;  May 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
RxData      .usect ".bss",128                     ; Allocate 128 byte of RAM
;-------------------------------------------------------------------------------
			.text							;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0500h,SP         		; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP3     bis.b   #06h,&P3SEL             ; Assign I2C pins to USCI_B0
SetupUCB0   bis.b   #UCSWRST,&UCB0CTL1      ; Enable SW reset
            mov.b   #UCMST+UCMODE_3+UCSYNC,&UCB0CTL0
                                            ; I2C Master, synchronous mode
            mov.b   #UCSSEL_2+UCSWRST,&UCB0CTL1
                                            ; Use SMCLK, keep SW reset
            mov.b   #12,&UCB0BR0            ; fSCL = SMCLK/12 = ~100kHz
            mov.b   #00,&UCB0BR1
            mov.w   #048h,&UCB0I2CSA        ; Slave Address is 048h
            bic.b   #UCSWRST,&UCB0CTL1      ; Clear SW reset, resume operation
            bis.b   #UCB0RXIE,&IE2          ; Enable RX interrupt

Main        mov.w   #RxData,R5              ; Start of RX buffer
            mov.w   #5,R6                   ; Load RX byte counter
Main_1      bit.b   #UCTXSTP,&UCB0CTL1      ; Ensure stop condition got sent
            jc      Main_1
            bis.b   #UCTXSTT,&UCB0CTL1      ; I2C start condition
            bis.b   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
                                            ; Remain in LPM0 until all data
                                            ; is RX'd
            nop                             ; Set breakpoint >>here<< and
                                            ; read out the RxData buffer
            jmp     Main                    ; Repeat
;-------------------------------------------------------------------------------
; The USCI_B0 data ISR is used to move received data from the I2C slave
; to the MSP430 memory. It is structured such that it can be used to receive
; any 2+ number of bytes by pre-loading R6 with the byte count.
;-------------------------------------------------------------------------------
USCIAB0TX_ISR;      USCI_B0 Data ISR
;-------------------------------------------------------------------------------
            dec.w   R6                      ; Decrement RX byte counter
            jz      USCIAB0TX_ISR_2         ; Jump if all bytes were RX'd
            mov.b   &UCB0RXBUF,0(R5)        ; Move RX data to address R5
            inc.w   R5                      ; Increment address pointer
            cmp.w   #1,R6                   ; Only one byte left?
            jeq     USCIAB0TX_ISR_1         ; Jump if yes
            reti

USCIAB0TX_ISR_1
            bis.b   #UCTXSTP,&UCB0CTL1      ; Generate I2C stop condition
            reti

USCIAB0TX_ISR_2
            mov.b   &UCB0RXBUF,0(R5)        ; Move final RX data to address R5
            bic.w   #CPUOFF,0(SP)           ; Exit LPM0
            reti
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int22"        ; USCI_B0 I2C Data Int Vector
            .short  USCIAB0TX_ISR
            .sect   ".reset"            ; POR, ext. Reset, Watchdog
            .short  RESET
            .end
