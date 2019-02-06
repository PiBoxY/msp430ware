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
;   MSP-FET430P140 Demo - I2C, Master Interface to PCF8574, Read/Write
;
;   Description: I2C communication with a PCF8574 in read and write mode is
;   demonstrated. PCF8574 port P is configured with P0-P3 input, P4-P7. Read
;   P0-P3 input data is written back to Port P4-P7.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~800kHz, SCL = SMCLK/10
;   //* MSP430F169 Device Required *//
;
;                                 MSP430F169
;                              -----------------
;                  /|\ /|\ /|\|              XIN|-
;                  10k 10k  | |                 |
;       PCF8574     |   |   --|RST          XOUT|-
;       ---------   |   |     |                 |
;  --->|P0    SDA|<-|---+---->|P3.1/SDA         |
;  --->|P1       |  |         |                 |
;  --->|P2       |  |         |                 |
;  --->|P3    SCL|<-+---------|P3.3/SCL         |
;  <---|P4       |            |                 |
;  <---|P5       |            |                 |
;  <---|P6       |            |                 |
;  <---|P7       |            |                 |
;   +--|A0,A1,A2 |            |                 |
;   |  |         |            |                 |
;  \|/
;
;
;   M. Buccini / L. Westlund
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
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer
SetupP3     bis.b   #0Ah,&P3SEL             ; Assign I2C pins to module
Setupi2c    bis.b   #I2C+SYNC,&U0CTL        ; Switch USART0 to I2C mode
            bic.b   #I2CEN, U0CTL           ; Recommended I2C init procedure
            mov.b   #I2CSSEL_2,&I2CTCTL     ; SMCLK
            mov.b   #003h,&I2CSCLH          ; High period of SCL
            mov.b   #003h,&I2CSCLL          ; Low period of SCL
            mov.b   #001h,&I2CNDAT          ; transmit one byte
            mov.w   #20h,&I2CSA             ; set slave address
            bis.b   #I2CEN,&U0CTL           ; Enable I2C
            mov.b   #RXRDYIE,&I2CIE         ; I2C receive ready interrupt enable
            mov.w   #CCIE,&CCTL0            ; CCR0 interrupt enabled
            mov.w   #TASSEL_2+ MC_2,&TACTL  ; SMCLK, contmode
                                            ;
Mainloop    bis.w   #CPUOFF+GIE,SR          ; CPU off, interrupts enabled
            nop                             ; Required for debugger
                                            ;
;------------------------------------------------------------------------------
TA0_ISR;
;------------------------------------------------------------------------------
            bis.b   #MST,&U0CTL             ; Master
            mov.b   #I2CSTT+I2CSTP,&I2CTCTL ; Recieve, ST, SP (Clears I2CTRX)
            reti                            ;		
                                            ;
;------------------------------------------------------------------------------
USART0TX_ISR;
;------------------------------------------------------------------------------
            add.w   &I2CIV,PC               ; Add offset vector
            reti                            ; 0
            reti                            ; 2
            reti                            ; 4
            reti                            ; 6
            reti                            ; 8
            jmp     I2C_RXrdy               ; 10
            reti                            ; 12
            reti                            ; 14
            reti                            ; 16

I2C_RXrdy   mov.b   &I2CDRB,R4              ;
            rlc.b   R4                      ;
            rlc.b   R4                      ;
            rlc.b   R4                      ;
            rlc.b   R4                      ;
            bis.b   #0Fh,R4                 ;
            bis.b   #MST,&U0CTL             ; Master
            bis.b   #I2CTRX+I2CSTT+I2CSTP,&I2CTCTL ; Transmit, ST, SP
            mov.b   R4,&I2CDRB              ;
            reti                            ;		
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int08"                ;
            .short  USART0TX_ISR            ; USART0 transmit
            .sect   ".int06"                ;
            .short  TA0_ISR                 ; Timer_A0 Vector
            .end
