; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2014, Texas Instruments Incorporated
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
;  MSP430FR69xx Demo - eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave
;
;  Description: This demo connects two MSP430's via the I2C bus. The master
;  reads 5 bytes from the slave. This is the MASTER CODE. The data from the slave
;  transmitter begins at 0 and increments with each transfer.
;  The USCI_B0 RX interrupt is used to know when new data has been received.
;  ACLK = n/a, MCLK = SMCLK = BRCLK =  DCO = 1MHz
;
;    *****used with "MSP430FR69xx_euscib0_i2c_11.c"****
;
;                                /|\  /|\
;               MSP430FR6989     10k  10k     MSP430F6989
;                   slave         |    |        master
;             -----------------   |    |   -----------------
;           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
;            |                 |  |       |                 | 32kHz
;           -|XOUT             |  |       |             XOUT|-
;            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
;            |                 |          |             P1.0|--> LED
;
;   E. Chen
;   Texas Instruments Inc.
;   April 2014
;   Built with Code Composer Studio V6.0
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
RXData      .set    R5
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
            bis.b   #BIT0,&P1DIR            ; For LED
            bis.b   #BIT6+BIT7,P1SEL0       ; I2C pins

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

SetupI2C    bis.w   #UCSWRST,&UCB0CTLW0     ; Software reset enabled
            bis.w   #UCMODE_3+UCMST+UCSYNC,&UCB0CTLW0 ; I2C mode, Master mode, sync
            bis.w   #UCASTP_2,&UCB0CTLW1    ; Automatic stop generated
                                            ; after UCB0TBCNT is reached
            mov.w   #0x0008,UCB0BRW         ; baudrate = SMCLK / 8
            mov.w   #0x0005,UCB0TBCNT       ; number of bytes to be received
            mov.w   #0x0048,UCB0I2CSA       ; Slave address
            bic.b   #UCSWRST,&UCB0CTL1
            bis.w   #UCRXIE+UCNACKIE+UCBCNTIE,&UCB0IE

Mainloop    mov.w   #0x0297,R15
Delay       dec.w   R15
            jne     Delay
Stopsent    bit.b   #UCTXSTP,&UCB0CTL1      ; Ensure stop condition got sent
            jne     Stopsent
            bis.b   #UCTXSTT,&UCB0CTL1      ; I2C start condition
            nop                             ;
            bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupt
            nop                             ;
            jmp     Mainloop

;------------------------------------------------------------------------------
USCI_B0_ISR;    USCI B0 Receive/Transmit Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &UCB0IV,PC              ; add offset to PC
            reti                            ; Vector 0: No interrupts
            reti                            ; Vector 2: ALIFG
            jmp     NACK                    ; Vector 4: NACKIFG
            reti                            ; Vector 6: STTIFG
            reti                            ; Vector 8: STPIFG
            reti                            ; Vector 10: RXIFG3
            reti                            ; Vector 14: TXIFG3
            reti                            ; Vector 16: RXIFG2
            reti                            ; Vector 18: TXIFG2
            reti                            ; Vector 20: RXIFG1
            reti                            ; Vector 22: TXIFG1
            jmp     GetRX                   ; Vector 24: RXIFG0
            reti                            ; Vector 26: TXIFG0
            jmp     ToggleLED               ; Vector 28: BCNTIFG
            reti                            ; Vector 30: clock low timeout
            reti                            ; Vector 32: 9th bit
NACK        bis.b   #UCTXSTT,&UCB0CTL1      ; I2C start condition
            reti
GetRX       mov.w   &UCB0RXBUF,RXData       ; Get RX data
            bic.w   #LPM0,0(SP)             ; Exit LPM0
ToggleLED   xor.b   #BIT0,&P1OUT            ; Toggle LED on P1.0
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   USCI_B0_VECTOR          ; USCI B0 Receive/Transmit Vector
            .short  USCI_B0_ISR
            .end
