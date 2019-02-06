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
;  MSP-FET430P140 Demo - I2C, Master Reads from MSP430 Slave, Repeat Mode
;
;  Description: This demo connects two MSP430's via the I2C bus. The master
;  reads from the slave in repeat mode. This is the master code. The slave
;  code is called fet140_i2c_09.asm. The data from the slave TX
;  begins at 0 and is incremented with each transfer. The received data is in
;  RXData and is checked for validity. If the received data is incorrect, the
;  CPU is trapped and the P1.0 LED will stay on. The RXRDYIFG interrupt is
;  used to know when new data has been received.
;  A Stop is issued after Master receives 6 bytes and then a new Start is
;  issued.
;  ACLK = n/a, MCLK = SMCLK = I2CCLOCK = DCO ~ 800kHz
;  ;* MSP430F169 Device Required *//
;
;                                 /|\  /|\
;                  MSP430F169     10k  10k     MSP430F169
;                    slave         |    |        master
;              -----------------|  |    |  -----------------
;             |             P3.1|<-|---+->|P3.1             |
;             |                 |  |      |             P1.0|-->LED
;             |                 |  |      |                 |
;             |             P3.3|<-+----->|P3.3             |
;             |                 |         |                 |
;
;
;   H. Grewal / A. Dannenberg / L. Westlund
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                  ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #0Ah,&P3SEL             ; Select I2C pins
            bis.b   #01h,&P1DIR             ; P1.0 output
            bic.b   #01h,&P1OUT             ; Clear P1.0

I2C_init    bis.b   #I2C+SYNC,&U0CTL        ; Recommended init procedure
            bic.b   #I2CEN,&U0CTL           ; Recommended init procedure
            bis.b   #I2CSSEL1+I2CRM,&I2CTCTL; SMCLK,repeat Mode
            mov     #0048h,&I2CSA           ; Slave Address is 048h
            bis.b   #RXRDYIE,&I2CIE         ; Enable RXRDYIE
            bis.b   #I2CEN,&U0CTL           ; Enable I2C

            clr     R6                      ; Use R6 to check incoming data
            clr     R7                      ; check if 6 bytes rcvd

            eint                            ; Enable interrupts

            bis.b   #MST,&U0CTL             ; Master mode
            bis.b   #I2CSTT,&I2CTCTL        ; Initiate transfer
Mainloop    cmp     #06h,R7
            jnz     rptRX
            clr     R7
            inc.b   R6
            bis.b   #I2CSTP,&I2CTCTL        ; Issue STOP
chkSTP      bit.b   #I2CSTP,&I2CTCTL        ; Check STP condition
            jnz     chkSTP
            bis.b   #MST,&U0CTL             ; Master mode
            bis.b   #I2CSTT,&I2CTCTL        ; Initiate transfer
rptRX       inc.b   R7
            bis.b   #LPM0,SR                ; Enter LPM0
            bis.b   #01h,&P1OUT             ; .set P1.0
            cmp     R5,R6                   ; Test received data
            jne     Trap                    ; Trap CPU if wrong
            bic.b   #01h,&P1OUT             ; Clear P1.0
            inc.b   R6                      ; Increment correct RX value
            jmp     Mainloop                ; Repeat

Trap        jmp     $                       ; Trap CPU

;------------------------------------------------------------------------------
I2C_ISR;    Common ISR for I2C Module
;------------------------------------------------------------------------------
            add     &I2CIV,PC               ; Add I2C offset vector
            reti                            ; No interrupt
            reti                            ; Arbitration Lost
            reti                            ; No Acknowledge
            reti                            ; Own Address
            reti                            ; Register Access Ready
            jmp     RXRDY_ISR               ; Receive Ready
            reti                            ; Transmit Ready
            reti                            ; General Call
            reti                            ; Start Condition

RXRDY_ISR   mov.b   &I2CDRB,R5              ; RX data in R5
            bic     #LPM0,0(SP)             ; Clear LPM0
            reti

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET vector
            .short  RESET                   ;
            .sect   ".int08"                ; I2C interruplt vector
            .short  I2C_ISR                 ;
            .end
