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
;  MSP430FR57xx Demo - USCI_B0 I2C Master TX bytes to Multiple Slaves
;
;  Description: This demo connects two MSP430's via the I2C bus. 
;  The master transmits to 4 different I2C slave addresses 0x0A,0x0B,0x0C&0x0D.
;  Each slave address has a specific related data in the array TXData[].
;  At the end of four I2C transactions the slave address rolls over and begins 
;  again at 0x0A.
;  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
;  Use with MSP430FR57xx_uscib0_i2c_MultiSlave.c
;
;                                /|\  /|\
;                MSP430FR5739    10k  10k     MSP430FR5739
;                   slave         |    |         master
;             -----------------   |    |   -----------------
;           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
;            |                 |  |       |                 |
;           -|XOUT             |  |       |             XOUT|-
;            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
;            |                 |          |                 |
;
;   Tyler Witt
;   Texas Instruments Inc.
;   Sepember 2011
;   Built with Code Composer Studio V4.2
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
TXData          .char   0xA1,0xB1,0xC1,0xD1
SlaveAddress    .char   0x0A,0x0B,0x0C,0x0D
TXByteCtr       .set    R5
SlaveFlag       .set    R6
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
            mov.b   #0xA5,&CSCTL0_H
            bis.w   #DCOFSEL0+DCOFSEL1,&CSCTL1 ; Set max DCO setting
            mov.w   #SELA_3+SELS_3+SELM_3,&CSCTL2 ; ACLK = MCLK = DCO
            mov.w   #DIVA_3+DIVS_3+DIVM_3,&CSCTL3 ; set all dividers to 1MHz
            bis.b   #BIT6+BIT7,&P1SEL1      ; Configure pins
            
            bis.w   #UCSWRST,&UCB0CTLW0     ; Software reset enabled
            bis.w   #UCMODE_3+UCMST+UCSYNC,&UCB0CTLW0 ; I2C slave sync mode
            mov.w   #0x8,&UCB0BRW           ; baudrate = SMCLK/8
            bic.w   #UCSWRST,&UCB0CTLW0     ; Software reset cleared
            bis.w   #UCTXIE0+UCSTPIE,&UCB0IE
            clr.b   SlaveFlag               ; Initialize SlaveFlag

Main        mov.w   #TXData,R7              ; Start of TX buffer
            mov.w   #SlaveAddress,R8        ; Configure slave address
Mainloop    mov.w   @R8,&UCB0I2CSA
            incd.w  R8                      ; increase address for next transfer
            mov.b   #1,TXByteCtr            ; Load TX byte counter
StopCond    bit.w   #UCTXSTP,&UCB0CTLW0     ; Ensure stop condition got sent 
            jnz     StopCond
            bis.w   #UCTR+UCTXSTT,&UCB0CTLW0 ; I2C TX, start condition
            nop
            bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupt
            nop                             ; remain in LPM0
            inc.b   SlaveFlag               ; Change slave address
            cmp.b   #4,SlaveFlag            ; Roll over slave address
            jlo     Mainloop
            clr.b   SlaveFlag
            mov.w   #500,R15                ; Delay between transmissions
Delay       dec.w   R15
            jnz     Delay
            jmp     Main                    ; re-initialize data/address
            nop                             ; for debug
            
;------------------------------------------------------------------------------
USCI_ISR ;  USCI Interrupt Service Routine
;------------------------------------------------------------------------------
            add     &UCB0IV,PC              ; Add offset to PC
            reti                            ; No interrupt
            reti                            ; ALIFG break
            jmp     NACK                    ; NACKIFG break
            reti                            ; STTIFG break
            reti                            ; STPIFG break
            reti                            ; RXIFG3 break
            reti                            ; TXIFG3 break
            reti                            ; RXIFG2 break
            reti                            ; TXIFG2 break
            reti                            ; RXIFG1 break
            reti                            ; TXIFG1 break
            reti                            ; RXIFG0 break
            jmp     GetTX                   ; TXIFG0 break
            reti                            ; BCNTIFG break
            reti                            ; clock low timeout break
            reti                            ; 9th bit break
NACK        bis.w   #UCTXSTT,&UCB0CTLW0     ; resend start if NACK
            reti
GetTX       cmp.w   #0x1,TXByteCtr          ; Check TX byte counter
            jne     StpCnd
            mov.w   @R7,&UCB0TXBUF           ; Put TX Data in buffer
            incd.w  R7                      ; increase for next transfer
            dec.b   TXByteCtr               ; Decrement TX byte counter
            reti
StpCnd      bis.w   #UCTXSTP,&UCB0CTLW0     ; I2C stop condition
            bic.w   #UCTXIFG,&UCB0IFG       ; Clear USCI_B0 TX int flag
            bic.w   #LPM0,0(SP)
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   USCI_B0_VECTOR          ; USCI_B0_VECTOR
            .short  USCI_ISR                ;
            .end
