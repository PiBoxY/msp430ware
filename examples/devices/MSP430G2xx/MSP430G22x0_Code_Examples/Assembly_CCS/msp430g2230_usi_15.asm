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
;  MSP430G2230 Demo - I2C Slave Receiver / Slave Transmitter, multiple bytes
;
;  Description: I2C Master communicates with I2C Slave using
;  the USI. Master data should increment from 0x55 with each transmitted byte.
;  ACLK = VLO, MCLK = SMCLK = Calibrated 1MHz
;
;  ***THIS IS THE SLAVE CODE***
;
;                  Slave                      Master
;                                    (msp430g2230_usi_12.c)
;               MSP430G2230                MSP430G2230
;             -----------------          -----------------
;         /|\|              XIN|-    /|\|              XIN|-
;          | |                 |      | |                 |
;          --|RST          XOUT|-     --|RST          XOUT|-
;            |                 |        |                 |
;      LED <-|P1.2             |        |                 |
;            |                 |        |             P1.2|-> LED
;            |         SDA/P1.7|------->|P1.6/SDA         |
;            |         SCL/P1.6|<-------|P1.7/SCL         |
;
;  Note: internal pull-ups are used in this example for SDA & SCL
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
;******************************************************************************

I2CState    .equ    R4
SLV_Data    .equ    R5
SLV_Addr    .equ    R6
Bytecount   .equ    R7
transmit    .equ    R8
repeated_start  .equ    R9
MST_Data    .equ    R10
number_of_bytes .set    5

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x27C,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

            cmp.b   #0xFF,&CALBC1_1MHZ      ; Check calibration constant
            jne     Load                    ; if not erased, load.      
Trap        jmp     Trap                    ; if erased do not load, trap CPU!

Load        clr.w   MST_Data
            mov.b   #0x55,SLV_Data          ; Variable for transmitted data
            mov.b   #0x90,SLV_Addr
            clr.w   transmit

            clr.w   I2CState
            clr.w   Bytecount
            clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   &CALBC1_1MHZ,&BCSCTL1   ; Set Range
            mov.b   &CALDCO_1MHZ,&DCOCTL    ; Set DCO step + modulation
            call    #Setup_USI_Slave
            bis.b   #LPM0,SR                ; CPU off, await USI interrupt
            nop

;******************************************************
USI_ISR
; USI interrupt service routine
; Data Transmit : state 0 -> 2 -> 4 -> 10 -> 12 -> 14
; Data Recieve  : state 0 -> 2 -> 4 -> 6 -> 8 -> 14
;******************************************************
            bit.b   #USISTTIFG,&USICTL1     ; Start entry?
            jnc     Check_State
            bis.b   #BIT2,&P1OUT            ; turn on LED, sequence start
            mov.b   #2,I2CState             ; First I2C state, Rx address
Check_State add.w   I2CState,PC             ; I2C State Machine
            jmp     STATE0
            jmp     STATE2
            jmp     STATE4
            jmp     STATE6
            jmp     STATE8
            jmp     STATE10
            jmp     STATE12
            jmp     STATE14

STATE0      ; Idle, should not get here
            jmp     exit
STATE2      ; RX Address
            mov.b   &USICNT,R11
            and.b   #0xE0,R11
            add.b   #0x8,R11
            mov.b   R11,&USICNT
            bic.b   #USISTTIFG,&USICTL1     ; Clear start flag
            mov.b   #4,I2CState             ; Go to next state: check address
            jmp     exit
STATE4      ; Process Address and send (N)Ack
            bit.b   #0x01,&USISRL           ; If master read...
            jnc     MWrite
            mov.b   #0x91,SLV_Addr          ; Save R/W bit
            mov.b   #1,transmit
            jmp     Chk_Add
MWrite      clr.b   transmit
            mov.b   #0x90,SLV_Addr
Chk_Add     bis.b   #USIOE,&USICTL0         ; SDA = output
            cmp.b   SLV_Addr,&USISRL        ; address match?
            jne     Add_NACK

            clr.b   &USISRL                 ; Send Ack
            bic.b   #BIT2,&P1OUT            ; LED off
            cmp.b   #0,transmit
            jne     TXstate
            mov.b   #6,I2CState             ; Go to next state: RX data
            jmp     State4Exit

TXstate     mov.b   #10,I2CState            ; Else go to next state: TX data
            jmp     State4Exit
Add_NACK    mov.b   #0xFF,&USISRL           ; Send NACK
            bis.b   #BIT2,&P1OUT            ; LED on:error
            mov.w   #8,I2CState             ; Go to next state: prep for next Start
State4Exit  bis.b   #1,&USICNT              ; Bit counter = 1, send (N)Ack bit
            jmp     exit
STATE6      ; Receive data byte
            call    #RX_Data
            jmp     exit
STATE8      ; Check Data & TX (N)Ack
            bis.b   #USIOE,&USICTL0         ; SDA = output   
            cmp.w   #number_of_bytes-1,Bytecount
            jn      NotLast
Last        mov.b   #0xFF,&USISRL           ; Send NAck
            bic.b   #USIOE,&USICTL0         ; SDA = input
            mov.b   #0x90,SLV_Addr          ; Reset slave address
            clr.b   I2CState                ; Reset state machine
            clr.b   Bytecount               ; Reset counter for next TX/RX
            jmp     exit
NotLast     clr.b   USISRL                  ; Send Ack
            mov.b   #6,I2CState             ; Rcv another byte
            inc.b   Bytecount
            bis.b   #1,&USICNT              ; Bit counter = 1, send (N)Ack bit
            jmp     exit
STATE10     ; Send Data byte
            call    #TX_Data
            jmp     exit
STATE12     ; Receive Data (N)Ack
            bic.b   #USIOE,&USICTL0         ; SDA = input
            bis.b   #0x01,&USICNT           ; Bit counter = 1, receive (N)Ack
            mov.b   #14,I2CState            ; Go to next state: check (N)Ack
            jmp     exit
STATE14     ; Process Data Ack/NAck
            bit.b   #0x01,&USISRL
            jeq     Ack
            bic.b   #USIOE,&USICTL0         ; SDA = input
            mov.b   #0x90,SLV_Addr          ; Reset slave address
            clr.b   I2CState                ; Reset state machine
            clr.b   Bytecount
            jmp     exit
Ack         bic.b   #BIT2,P1OUT             ; LED off
            call    #TX_Data                ; TX next byte

exit        bic.b   #USIIFG,&USICTL1        ; Clear pending flag
            reti

;-------------------------------------------------------------------------------
RX_Data
;-------------------------------------------------------------------------------
            bic.b   #USIOE,&USICTL0         ; SDA = input --> redundant
            bis.b   #0x08,&USICNT           ; Bit counter = 8, RX data
            mov.b   #0x08,I2CState          ; Next state: Test data and (N)Ack
            ret

;-------------------------------------------------------------------------------
TX_Data
;-------------------------------------------------------------------------------
            bis.b   #USIOE,&USICTL0         ; SDA = output
            mov.b   SLV_Data,&USISRL
            inc.b   SLV_Data
            bis.b   #0x08,&USICNT           ; Bit counter = 8, TX data
            mov.b   #12,I2CState            ; Go to next state: receive (N)Ack
            ret

;-------------------------------------------------------------------------------
Setup_USI_Slave
;-------------------------------------------------------------------------------
            bis.b   #0xC0,&P1OUT            ; P1.6 and P1.7 Pullups
            bis.b   #0xC0,&P1REN            ; P1.6 and P1.7 Pullups
            bis.b   #BIT2+BIT5+BIT6+BIT7,&P1DIR ; Available P1.x pins: outputs

            mov.b   #USIPE6+USIPE7+USISWRST,&USICTL0; Ports and USI mode setup
            mov.b   #USII2C+USIIE+USISTTIE,&USICTL1 ; Enable I2C mode/USI interrupt
            mov.b   #USICKPL,&USICKCTL      ; SCL = SMCLK/8 (~120kHz)
            bis.b   #USIIFGCC,&USICNT       ; Disable automatic clear control
            bic.b   #USISWRST,&USICTL0      ; Enable USI
            bic.b   #USIIFG,&USICTL1        ; Clear pending flag

            clr.b   transmit
            bis.b   #GIE,SR
            ret

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   USI_VECTOR              ; USI
            .short  USI_ISR
            .end

