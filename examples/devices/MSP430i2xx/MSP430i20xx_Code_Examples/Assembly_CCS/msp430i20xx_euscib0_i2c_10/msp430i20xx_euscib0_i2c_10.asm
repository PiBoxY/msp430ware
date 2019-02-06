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
;  MSP430i20xx Demo - eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave
;
;  Description: This demo connects two MSP430's via the I2C bus. The master
;  reads 5 bytes from the slave. This is the MASTER CODE. The data from the slave
;  transmitter begins at 0 and increments with each transfer.
;                * used with "msp430i20xx_euscib0_i2c_11.asm *
;
;  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;                                /|\  /|\
;               MSP430i2041      10k  10k     MSP430i2041
;                   slave         |    |        master
;             -----------------   |    |   -----------------
;            |     P1.7/UCB0SDA|<-|----+->|P1.7/UCB0SDA     |
;            |                 |  |       |                 |
;            |                 |  |       |                 |
;            |     P1.6/UCB0SCL|<-+------>|P1.6/UCB0SCL     |
;            |                 |          |             P1.4|--> LED
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
RXData      .set    R7
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
            bic.b   #BIT4,&P1OUT            ; LED Off
            bis.b   #BIT4,&P1DIR            ; LED Output
            bis.b   #BIT6|BIT7,&P1SEL0      ; eUSCI_A0 Pin Function
            bic.b   #BIT6|BIT7,&P1SEL1
            bis.w   #UCSWRST,&UCB0CTLW0     ; eUSCI in reset
            bis.w   #UCMODE_3|UCMST|UCSYNC,&UCB0CTLW0 ; I2C Master mode
            bis.w   #UCASTP_2,&UCB0CTLW1    ; Stop generated after UCBOTBCNT
            mov.w   #0x0080,&UCB0BRW        ; SMCLK/128
            mov.w   #0x0005,&UCB0TBCNT      ; 5 bytes
            mov.w   #0x0048,&UCB0I2CSA      ; Save address
            bic.w   #UCSWRST,&UCB0CTLW0     ; Initialize eUSCI
            bis.w   #UCRXIE|UCNACKIE|UCBCNTIE,&UCB0IE ; Enable interrupts

Mainloop    mov.w   #32000,R15              ; Delay loop
Delay       dec.w   R15
            jnz     Delay
StopChk     bit.b   #UCTXSTP,&UCB0CTL1      ; Ensure stop condition was sent
            jne     StopChk
            bis.b   #UCTXSTT,&UCB0CTL1      ; I2C start condition
            bis.w   #LPM0|GIE,SR            ; Enter LPM0 w/ interrupts
            jmp     Mainloop                ; Again
                                            ;
;-------------------------------------------------------------------------------
USCI_B0_ISR    ; eUSCI_B0 ISR
;-------------------------------------------------------------------------------
            add.w   &UCB0IV,PC                ; Add interrupt offset
            reti                            ; No interrupt
            reti                            ; UCALIFG
            jmp     NACK_                    ; UCNACKIFG
            reti                            ; UCSTTIFG
            reti                            ; UCSTPIFG
            reti                            ; UCRXIFG3
            reti                            ; UCTXIFG3
            reti                            ; UCRXIFG2
            reti                            ; UCTXIFG2
            reti                            ; UCRXIFG1
            reti                            ; UCTXIFG1
            jmp     RX0_                    ; UCRXIFG0
            reti                            ; UCTXIFG0
            jmp     BCNT_                    ; UCBCNTIFG
            reti                            ; UCCLTOIFG
            reti                            ; UCBIT9IFG
NACK_       bis.b   #UCTXSTT,&UCB0CTL1        ; I2C start condition
            reti
RX0_        mov.b   &UCB0RXBUF,RXData        ; Get RX Data
            bic.w   #LPM0,0(SP)                ; Exit LPM0
            reti
BCNT_       xor.b   #BIT4,&P1OUT            ; Toggle LED
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int08"                ; eUSCI_B0 Vector
            .short  USCI_B0_ISR
            .end
