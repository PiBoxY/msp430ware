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
;  MSP430G2230 Demo - I2C Master Receiver, single byte
;
;  Description: I2C Master communicates with I2C Slave using
;  the USI. Slave data should increment from 0x00 with each transmitted byte
;  which is verified by the Master.
;  LED off for address or data Ack; LED on for address or data NAck.
;  ACLK = VLO, MCLK = SMCLK = Calibrated 1MHz
;
;  ***THIS IS THE MASTER CODE***
;
;                  Slave                      Master
;          (msp430g2230_usi_09.c)
;               MSP430G2230                MSP430G2230
;             -----------------          -----------------
;         /|\|              XIN|-    /|\|              XIN|-
;          | |                 |      | |                 |
;          --|RST          XOUT|-     --|RST          XOUT|-
;            |                 |        |                 |
;      LED <-|P1.2             |        |                 |
;            |                 |        |             P1.2|-> LED
;            |         SDA/P1.7|------->|P1.7/SDA         |
;            |         SCL/P1.6|<-------|P1.6/SCL         |
;
;  Note: internal pull-ups are used in this example for SDA & SCL
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
;******************************************************************************
I2CState    .equ    R4
slav_data   .equ    R5
slav_add    .equ    R6

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x280,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

            cmp.b   #0xFF,&CALBC1_1MHZ      ; Check calibration constant
            jne     Load                    ; if not erased, load.       
Trap        jmp     Trap                    ; if erased do not load, trap CPU!

Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   CALBC1_1MHZ,&BCSCTL1    ; Set Range
            mov.b   CALDCO_1MHZ,&DCOCTL     ; Set DCO step + modulation

            bis.b   #0xC0,&P1OUT            ; P1.6 and P1.7 Pullups
            bis.b   #0xC0,&P1REN            ; P1.6 and P1.7 Pullups
            bis.b   #BIT2+BIT5+BIT6+BIT7,&P1DIR ; Available P1.x pins: outputs

            bis.b   #USIPE6+USIPE7+USIMST+USISWRST,&USICTL0 ; Ports/USI mode setup
            bis.b   #USII2C+USIIE,&USICTL1  ; Enable I2C mode & USI interrupt
            bis.b   #USIDIV_3+USISSEL_2+USICKPL,&USICKCTL ; SCL = SMCLK/8 (~120kHz)
            bis.b   #USIIFGCC,&USICNT       ; Disable automatic clear control
            bic.b   #USISWRST,&USICTL0      ; Enable USI
            bic.b   #USIIFG,&USICTL1        ; Clear pending flag
            clr.w   I2CState
            clr.b   slav_data
            mov.b   #0x91,slav_add

loop        bis.b   #USIIFG,&USICTL1        ; Set flag and start communication
            bis.w   #LPM0+GIE,SR            ; CPU off, await USI interrupt
            call    #delay
            jmp     loop


;-------------------------------------------------------------------------------
USI_ISR
;-------------------------------------------------------------------------------
            add.w   I2CState,PC             ; I2C State Machine
            jmp     STATE0
            jmp     STATE2
            jmp     STATE4
            jmp     STATE6
            jmp     STATE8
            jmp     STATE10
STATE0      ; Generate Start Condition & send address to slave
            bis.b   #BIT2,&P1OUT            ; LED on: sequence start
            mov.b   #0,&USISRL              ; Generate start condition
            bis.b   #USIGE+USIOE,&USICTL0   ; Output latch always enabled
            bic.b   #USIGE,&USICTL0         ; Output latch depends on shift clock
            mov.b   slav_add,&USISRL        ; Transmit address. R/W=1
            bit.b   #0xE0,&USICNT
            bis.b   #0x08,&USICNT           ; Bit counter = 8, TX Address
            mov.w   #2,I2CState             ; Go to next state: rcv address (N)Ack
            jmp     exit
STATE2      ; Receive Address Ack/Nack bit
            bic.b   #USIOE,&USICTL0         ; SDA = input. Output disabled
            bis.b   #0x01,&USICNT           ; Bit counter = 1, recieve (N)Ack bit
            mov.b   #4,I2CState             ; Go to next state: check (N)Ack
            jmp     exit
STATE4      ; Process Address Ack/Nack & handle data RX
            bit.b   #0x01,&USISRL
            jnc     Data_Rx
            bis.b   #USIOE,&USICTL0         ; Nack recieved. Prep stop condition
            mov.b   #0,&USISRL
            bis.b   #0x01,&USICNT           ; Bit counter = 1, SCL high, SDA low
            mov.w   #10,I2CState            ; Go to next state: generate Stop
            bis.b   #BIT2,&P1OUT            ; Turn on LED: error
            jmp     exit
Data_Rx     ; Ack received. Receive Data from slave
            bis.b   #0x08,&USICNT           ; Bit counter = 8, RX data
            mov.w   #6,I2CState             ; Go to next state: Test data and (N)Ack
            bic.b   #BIT2,&P1OUT            ; LED off
            jmp     exit
STATE6      ; Send Data Ack/Nack bit
            bis.b   #USIOE,&USICTL0         ; SDA = output
            cmp.b   slav_data,&USISRL
            jne     NotValid
            mov.b   #0,&USISRL              ; Data Valid
            inc     slav_data
            bic.b   #BIT2,&P1OUT            ; LED off
            jmp     end_6
NotValid    mov.b   #0xFF,&USISRL           ; Send NAck
            bis.b   #BIT2,&P1OUT            ; LED on: error
end_6       bis.b   #0x01,&USICNT           ; Bit counter = 1, send (N)Ack bit
            mov.w   #8,I2CState             ; Go to next state: prep stop
            jmp     exit
STATE8      ; Prep Stop Condition
            bis.b   #USIOE,&USICTL0         ; SDA = output
            mov.b   #0,&USISRL
            bis.b   #0x01,&USICNT           ; Bit counter = 1, SCL high, SDA low
            mov.w   #10,I2CState            ; Go to next state: generate stop
            jmp     exit
STATE10     ; Generate Stop Condition
            mov.b   #0xFF,&USISRL           ; &USISRL = 1 to release SDA
            bis.b   #USIGE,&USICTL0         ; Transparent latch enabled
            bic.b   #USIGE+USIOE,&USICTL0   ; Latch/SDA output disabled
            mov.w   #0,I2CState             ; Reset state machine for next transmission
            bic.w   #LPM0,0(SP)             ; Exit active CPU

exit        bic.b   #USIIFG,&USICTL1        ; Clear pending flag
            reti

;-------------------------------------------------------------------------------
delay
;-------------------------------------------------------------------------------
            mov.w   #5000,R15               ; Dummy delay between communication cycles
delay_loop  dec     R15
            jnz     delay_loop
            ret

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   USI_VECTOR              ; USI
            .short  USI_ISR
            .end
