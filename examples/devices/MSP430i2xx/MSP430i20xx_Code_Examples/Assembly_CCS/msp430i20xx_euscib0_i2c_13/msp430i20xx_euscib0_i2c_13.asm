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
;  MSP430i20xx Demo - eUSCI_B0 I2C 4 Hardware I2C slaves
;
;  Description: This demo connects two MSP430's via the I2C bus.
;  This code configures the MSP430 USCI to be addressed as 4 independent I2C
;  slaves. Each slave has its owm interrupt flag and data variable to store
;  incoming data.
;                * used with "msp430i20xx_euscib0_i2c_12.asm *
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
;            |                 |          |                 |
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
RXData0     .set    R5
RXData1     .set    R6
RXData2     .set    R7
RXData3     .set    R8
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
            bis.b   #BIT6|BIT7,&P1SEL0      ; eUSCI_A0 Pin Function
            bic.b   #BIT6|BIT7,&P1SEL1
            bis.w   #UCSWRST,&UCB0CTLW0     ; eUSCI in reset
            bis.w   #UCMODE_3|UCSYNC,&UCB0CTLW0 ; I2C Slave mode
            mov.w   #0x0A|UCOAEN,&UCB0I2COA0; Slave address 0x0A + enable
            mov.w   #0x0B|UCOAEN,&UCB0I2COA1; Slave address 0x0B + enable
            mov.w   #0x0C|UCOAEN,&UCB0I2COA2; Slave address 0x0C + enable
            mov.w   #0x0D|UCOAEN,&UCB0I2COA3; Slave address 0x0D + enable
            bic.w   #UCSWRST,&UCB0CTLW0     ; Initialize eUSCI
            bis.w   #UCRXIE0|UCRXIE1|UCRXIE2|UCRXIE3,&UCB0IE
            ; Enable interrupts

            clr.w   RXData0
            clr.w   RXData1
            clr.w   RXData2
            clr.w   RXData3

Mainloop    bis.w   #LPM0|GIE,SR            ; Enter LPM0 w/ interrupts
            nop                             ; For Debugger
                                            ;
;-------------------------------------------------------------------------------
USCI_B0_ISR ; eUSCI_B0 ISR
;-------------------------------------------------------------------------------
            add.w   &UCB0IV,PC              ; Add interrupt offset
            reti                            ; No interrupt
            reti                            ; UCALIFG
            reti                            ; UCNACKIFG
            reti                            ; UCSTTIFG
            reti                            ; UCSTPIFG
            jmp     RX3_                    ; UCRXIFG3
            reti                            ; UCTXIFG3
            jmp     RX2_                    ; UCRXIFG2
            reti                            ; UCTXIFG2
            jmp     RX1_                    ; UCRXIFG1
            reti                            ; UCTXIFG1
            jmp     RX0_                    ; UCRXIFG0
            reti                            ; UCTXIFG0
            reti                            ; UCBCNTIFG
            reti                            ; UCCLTOIFG
            reti                            ; UCBIT9IFG
RX0_        mov.b   &UCB0RXBUF,RXData0      ; Slave 0
            reti
RX1_        mov.b   &UCB0RXBUF,RXData1      ; Slave 1
            reti
RX2_        mov.b   &UCB0RXBUF,RXData2      ; Slave 2
            reti
RX3_        mov.b   &UCB0RXBUF,RXData3      ; Slave 3
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int08"                ; eUSCI_B0 Vector
            .short  USCI_B0_ISR
            .end
