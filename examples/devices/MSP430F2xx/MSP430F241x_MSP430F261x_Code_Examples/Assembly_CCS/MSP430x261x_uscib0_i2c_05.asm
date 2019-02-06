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
;   MSP430x261x Demo - USCI_B0 I2C Slave TX single bytes to MSP430 Master
;
;   Description: This demo connects two MSP430's via the I2C bus. The master
;   reads from the slave. This is the slave code. The TX data begins at 0
;   and is incremented each time it is sent. An incoming start condition
;   is used as a trigger to increment the outgoing data. The master checks the
;   data it receives for validity. If it is incorrect, it stops communicating
;   and the P1.0 LED will stay on. The USCI_B0 TX interrupt is used to know
;   when to TX.
;   ACLK = n/a, MCLK = SMCLK = default DCO = ~1.045MHz
;
; ***to be used with "MSP430x261x_uscib0_i2c_04.asm" ***
;
;                                 /|\  /|\
;           MSP430F261x/241x      10k  10k     MSP430F261x/241x
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
;  June 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0850h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP3     bis.b   #06h,&P3SEL             ; Assign I2C pins to USCI_B0
SetupUCB0   bis.b   #UCSWRST,&UCB0CTL1      ; Enable SW reset
            mov.b   #UCMODE_3+UCSYNC,&UCB0CTL0
                                            ; I2C Slave, synchronous mode
            mov.w   #048h,&UCB0I2COA        ; Own Address is 048h
            bic.b   #UCSWRST,&UCB0CTL1      ; Clear SW reset, resume operation
            bis.b   #UCSTTIE,&UCB0I2CIE     ; Enable STT interrupt
            bis.b   #UCB0TXIE,&IE2          ; Enable TX interrupt

            mov.b   #0ffh,R5                ; Use R5 to hold TX data

Main        bis.b   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
            jmp     Main                    ; Repeat
;-------------------------------------------------------------------------------
USCIAB0TX_ISR;      USCI_B0 Data ISR
;-------------------------------------------------------------------------------
            mov.b   R5,&UCB0TXBUF           ; TX data in R5
            bic.w   #LPM0,0(SP)             ; Clear LPM0
            reti
;-------------------------------------------------------------------------------
USCIAB0RX_ISR;      USCI_B0 State ISR
;-------------------------------------------------------------------------------
            bic.b   #UCSTTIFG,&UCB0STAT     ; Clear start condition int flag
            inc.b   R5                      ; Increment data
            reti
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".int22"        ; USCI_B0 I2C Data Int Vector
            .short	USCIAB0TX_ISR
            .sect	".int23"        ; USCI_B0 I2C Sate Int Vector
            .short	USCIAB0RX_ISR
            .sect	".reset"	           	; POR, ext. Reset,Watchdog
            .short  RESET  
            .end
