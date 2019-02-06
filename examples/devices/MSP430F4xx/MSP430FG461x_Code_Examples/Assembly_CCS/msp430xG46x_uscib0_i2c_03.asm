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
;   MSP430xG461x Demo - USCI_B0 I2C Master Interface to DAC8571, Write
;
;   Description: Using UCB0TXIE, a continuous sine wave is output to the
;   external DAC using a 16-point look-up table. Only one start condition
;   is executed. Data is handled by the ISR and the CPU is normally in LPM0.
;   ACLK = 32kHz, MCLK = SMCLK = TACLK = BRCLK = 1MHz
;
;                MSP430xG461x                       DAC8571
;            ------------------                   ------------
;          -|XIN   P3.1/UCB0SDA|<--------------->|SDA         |
;     32kHz |      P3.2/UCB0SCL|---------------->|SCL  I2C    |
;          -|XOUT              |                 |    SLAVE   |
;           |     I2C MASTER   |              GND|A0          |
;
;
;   DAC8571 I2C address = 0x4C (A0 = GND)
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
RESET       mov.w   #900,SP         ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer
SetupP3     bis.b   #06h,&P3SEL             ; Assign I2C pins to USCI_B0
SetupUCB0   bis.b   #UCSWRST,&UCB0CTL1      ; Enable SW reset
            mov.b   #UCMST+UCMODE_3+UCSYNC,&UCB0CTL0
                                            ; I2C Master, synchronous mode
            mov.b   #UCSSEL_2+UCSWRST,&UCB0CTL1
                                            ; Use SMCLK, keep SW reset
            mov.b   #11,&UCB0BR0            ; fSCL = SMCLK/11 = 95.3kHz
            mov.b   #00,&UCB0BR1
            mov.w   #04ch,&UCB0I2CSA        ; Set slave address
            bic.b   #UCSWRST,&UCB0CTL1      ; Clear SW reset, resume operation
            bis.b   #UCB0TXIE,&IE2          ; Enable TX ready interrupt

Main        clr.w   R12                     ; Clear byte counter
            bis.b   #UCTR+UCTXSTT,&UCB0CTL1 ; I2C TX, start condition
            mov.b   #010h,&UCB0TXBUF        ; Write DAC control byte
            bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupt
            nop                             ; Required for debug
;-------------------------------------------------------------------------------
USCIAB0TX_ISR;
;-------------------------------------------------------------------------------
            mov.b   Sine_Tab(R12),&UCB0TXBUF; Transmit data byte
            inc.w   R12                     ;
            and.w   #01Fh,R12               ; Do not exceed table
            reti                            ; Return ISR
;-------------------------------------------------------------------------------
;           16 Point 16-bit Sine Table
;-------------------------------------------------------------------------------
Sine_Tab    .byte      0FFh                    ; MSB Word 0
            .byte      0FFh                    ; LSB
            .byte      0F6h                    ; MSB Word 1
            .byte      040h                    ; LSB
            .byte      0DAh                    ; MSB Word 2
            .byte      081h                    ; LSB
            .byte      0B0h                    ; MSB Word 3
            .byte      0FAh                    ; LSB
            .byte      07Fh                    ; MSB Word 4
            .byte      0FFh                    ; LSB
            .byte      04Fh                    ; MSB Word 5
            .byte      003h                    ; LSB
            .byte      025h                    ; MSB Word 6
            .byte      07Ch                    ; LSB
            .byte      009h                    ; MSB Word 7
            .byte      0BDh                    ; LSB
            .byte      000h                    ; MSB Word 8
            .byte      000h                    ; LSB
            .byte      009h                    ; MSB Word 9
            .byte      0BDh                    ; LSB
            .byte      025h                    ; MSB Word 10
            .byte      07Ch                    ; LSB
            .byte      04Fh                    ; MSB Word 11
            .byte      003h                    ; LSB
            .byte      07Fh                    ; MSB Word 12
            .byte      0FEh                    ; LSB
            .byte      0B0h                    ; MSB Word 13
            .byte      0FAh                    ; LSB
            .byte      0DAh                    ; MSB Word 14
            .byte      081h                    ; LSB
            .byte      0F6h                    ; MSB Word 15
            .byte      040h                    ; LSB
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int24"        		; USCI_B0 I2C Data Int Vector
            .short  USCIAB0TX_ISR
            .sect	".reset"            	;	POR, ext. Reset, Watchdog
            .short  RESET					;
            .end   
                     
