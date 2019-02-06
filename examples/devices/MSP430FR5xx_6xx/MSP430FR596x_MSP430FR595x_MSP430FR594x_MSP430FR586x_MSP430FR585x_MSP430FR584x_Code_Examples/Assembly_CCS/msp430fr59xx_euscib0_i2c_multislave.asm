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
;  MSP430FR59xx Demo - eUSCI_B0 I2C 4 Hardware I2C slaves
;
;  Description: This demo connects two MSP430's via the I2C bus.
;  This code configures the MSP430 USCI to be addressed as 4 independent I2C
;  slaves. Each slave has its owm interrupt flag and data variable to store
;  incoming data.
;  Use with msp430fr59xx_uscib0_i2c_master_multislave.c
;  ACLK = n/a, MCLK = SMCLK = default DCO = ~1.045MHz
;
;                                /|\  /|\
;                MSP430FR5969    10k  10k     MSP430FR5969
;                   slave         |    |         master
;             -----------------   |    |   -----------------
;           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
;            |                 |  |       |                 |
;           -|XOUT             |  |       |             XOUT|-
;            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
;            |                 |          |                 |
;            |                 |          |                 |
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
RXData0     .set    R5
RXData1     .set    R6
RXData2     .set    R7
RXData3     .set    R8
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
SetupGPIO   bis.b   #BIT6+BIT7,P1SEL1       ; I2C pins

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

SetupI2C    mov.w   #UCSWRST,&UCB0CTLW0     ; Software reset enabled
            bis.w   #UCMODE_3+UCSYNC,&UCB0CTLW0 ; I2C mode, sync mode
            mov.w   #0x0A+UCOAEN,&UCB0I2COA0 ; SLAVE0 own address is 0x0A + enable
            mov.w   #0x0B+UCOAEN,&UCB0I2COA1 ; SLAVE1 own address is 0x0B + enable
            mov.w   #0x0C+UCOAEN,&UCB0I2COA2 ; SLAVE2 own address is 0x0C + enable
            mov.w   #0x0D+UCOAEN,&UCB0I2COA3 ; SLAVE3 own address is 0x0D + enable
            bic.w   #UCSWRST,&UCB0CTLW0     ; clear reset register
            bis.w   #UCRXIE0+UCRXIE1+UCRXIE2+UCRXIE3,&UCB0IE ; receive interrupt enable

            clr.w   RXData0                 ; Initialize RX Data
            clr.w   RXData1
            clr.w   RXData2
            clr.w   RXData3
            nop                             ; 
            bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupt
            nop                             ; remain in LPM0

;------------------------------------------------------------------------------
USCI_B0_ISR;    USCI B0 Receive/Transmit Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &UCB0IV,PC              ; add offset to PC
            reti                            ; Vector 0: No interrupts
            reti                            ; Vector 2: ALIFG
            reti                            ; Vector 4: NACKIFG
            reti                            ; Vector 6: STTIFG
            reti                            ; Vector 8: STPIFG
            jmp     GetRX3                  ; Vector 10: RXIFG3
            reti                            ; Vector 14: TXIFG3
            jmp     GetRX2                  ; Vector 16: RXIFG2
            reti                            ; Vector 18: TXIFG2
            jmp     GetRX1                  ; Vector 20: RXIFG1
            reti                            ; Vector 22: TXIFG1
            jmp     GetRX0                  ; Vector 24: RXIFG0
            reti                            ; Vector 26: TXIFG0
            reti                            ; Vector 28: BCNTIFG
            reti                            ; Vector 30: clock low timeout
            reti                            ; Vector 32: 9th bit
GetRX3      mov.w   &UCB0RXBUF,RXData3      ; SLAVE3
            reti
GetRX2      mov.w   &UCB0RXBUF,RXData2      ; SLAVE2
            reti
GetRX1      mov.w   &UCB0RXBUF,RXData1      ; SLAVE1
            reti
GetRX0      mov.w   &UCB0RXBUF,RXData0      ; SLAVE0
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   USCI_B0_VECTOR          ; USCI B0 Receive/Transmit Vector
            .short  USCI_B0_ISR
            .end
