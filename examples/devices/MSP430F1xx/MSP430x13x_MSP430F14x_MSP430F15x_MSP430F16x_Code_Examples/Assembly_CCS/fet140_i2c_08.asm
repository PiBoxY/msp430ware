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
;   MSP-FET430P140 Demo - I2C, Master Writes Multiple Bytes to MSP430 Slave
;
;   Description: This demo connects two MSP430's via the I2C bus. The master
;   writes 3 bytes to the slave. This is the master code. The slave code is
;   called fet140_i2c_07.asm. The TX data is incremented each time it is
;   sent.
;   ACLK = n/a, MCLK = SMCLK = I2CCLOCK = DCO ~800kHz;
;   ;* MSP430F169 Device Required *//
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
;   H. Grewal / L. Westlund
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
            bis.b   #I2CSSEL1+I2CTRX,&I2CTCTL ; SMCLK
            mov.b   #03h,&I2CNDAT           ; Read one byte
            mov     #0048h,&I2CSA           ; Slave Address is 048h
            bis.b   #TXRDYIE,&I2CIE         ; Enable RXRDYIE
            bis.b   #I2CEN,&U0CTL           ; Enable I2C
            clr     R5                      ; Use R5 to hold TX data

Mainloop    bis.b   #MST,&U0CTL             ; Master mode
            bis.b   #I2CSTT+I2CSTP,&I2CTCTL ; Initiate transfer
Byte1       bit.b   #TXRDYIFG,&I2CIFG
            jz      Byte1                   ; Repeat
            mov.b   R5,&I2CDRB              ; Load I2CDRB
            inc.b   R5
Byte2       bit.b   #TXRDYIFG,&I2CIFG
            jz      Byte2                   ; Repeat
            mov.b   R5,&I2CDRB              ; Load I2CDRB
            inc.b   R5
Byte3       bit.b   #TXRDYIFG,&I2CIFG
            jz      Byte3                   ; Repeat
            mov.b   R5,&I2CDRB              ; Load I2CDRB
            jmp     $

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET vector
            .short  RESET                   ;
            .end
