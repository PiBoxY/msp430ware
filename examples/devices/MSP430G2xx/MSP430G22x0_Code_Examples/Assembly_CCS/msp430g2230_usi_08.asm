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
;  MSP430G2230 Demo - I2C Slave Receiver, single byte
;
;  Description: I2C Slave communicates with I2C Master using
;  the USI. Master data should increment from 0x00 with each transmitted byte
;  which is verified by the slave.
;  LED off for address or data Ack; LED on for address or data NAck.d by the slave.
;  ACLK = VLO, MCLK = SMCLK = Calibrated 1MHz
;
;  ***THIS IS THE SLAVE CODE***
;
;                  Slave                      Master
;                                      (msp430g2230_usi_07.c)
;               MSP430G2230               MSP430G2230
;             -----------------          -----------------
;         /|\|              XIN|-    /|\|              XIN|-
;          | |                 |      | |                 |
;          --|RST          XOUT|-     --|RST          XOUT|-
;            |                 |        |                 |
;      LED <-|P1.2             |        |                 |
;            |                 |        |             P1.2|-> LED
;            |         SDA/P1.7|<-------|P1.7/SDA         |
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
mstr_data   .equ    R5
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

            cmp.b   #0xFF,&CALBC1_1MHZ      ; If calibration constants erased
            jne     Load                    ; if not erased, load.         
Trap        jmp     Trap                    ; if erased do not load, trap CPU!

Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   CALBC1_1MHZ,&BCSCTL1    ; Set Range
            mov.b   CALDCO_1MHZ,&DCOCTL     ; Set DCO step + modulation

            bis.b   #0xC0,&P1OUT            ; P1.6 and P1.7 Pullups
            bis.b   #0xC0,&P1REN            ; P1.6 and P1.7 Pullups
            bis.b   #BIT2+BIT5+BIT6+BIT7,&P1DIR ; Available P1.x pins: outputs

            mov.b   #USIPE6+USIPE7+USISWRST,&USICTL0; Ports and USI mode setup
            mov.b   #USII2C+USIIE+USISTTIE,&USICTL1 ; Enable I2C mode/USI interrupt
            mov.b   #USICKPL,&USICKCTL      ; SCL = SMCLK/8 (~120kHz)
            bis.b   #USIIFGCC,&USICNT       ; Disable automatic clear control
            bic.b   #USISWRST,&USICTL0      ; Enable USI
            bic.b   #USIIFG,&USICTL1        ; Clear pending flag
            clr.w   I2CState
            clr.b   mstr_data
            mov.b   #0x90,slav_add

loop        bis.w   #LPM0+GIE,SR            ; CPU off, await USI interrupt
            call    #delay
            jmp     loop


;-------------------------------------------------------------------------------
USI_ISR
;-------------------------------------------------------------------------------
            bit.b   #USISTTIFG,&USICTL1     ; Start entry?
            jnc     Check_State
            bis.b   #BIT2,&P1OUT            ; turn on LED, sequence start
            mov.w   #2,I2CState             ; First I2C state, Rx address
Check_State add.w   I2CState,PC             ; I2C State Machine
            jmp     STATE0
            jmp     STATE2
            jmp     STATE4
            jmp     STATE6
            jmp     STATE8
            jmp     STATE10

STATE0      ; Idle, should not get here
            jmp     exit
STATE2      ; Receive Address
            bit.b   #0xE0,&USICNT           ; Bit counter = 8, RX Address
            bis.b   #0x08,&USICNT
            bic.b   #USISTTIFG,&USICTL1     ; Clear start flag
            mov.b   #4,I2CState             ; Go to next state: check address
            jmp     exit
STATE4      ; Process Address and send (N)Ack
            bit.b   #0x01,&USISRL           ; If read
            jnc     Chk_Add
            inc.b   slav_add                ; Save R/W bit
Chk_Add     bis.b   #USIOE,&USICTL0         ; SDA = output
            cmp.b   slav_add,&USISRL        ; address match?
            jnz     Add_NACK
            clr.b   &USISRL                 ; Send ack
            bic.b   #BIT2,&P1OUT            ; LED off
            mov.w   #8,I2CState             ; Go to next state: RX data
            bis.b   #0x01,&USICNT           ; Send Ack bit
            jmp     exit
Add_NACK    mov.b   #0xFF,&USISRL           ; Send NACK
            bis.b   #BIT2,&P1OUT            ; LED on:error
            mov.w   #6,I2CState             ; Go to next state: prep for next Start
            bis.b   #0x01,&USICNT           ; Bit counter = 1, Send Nack
            jmp     exit
STATE6      ; Prep for Start condition
            bic.b   #USIOE,&USICTL0         ; SDA = input
            mov.b   #0x90,slav_add          ; Reset slave address
            mov.w   #0,I2CState             ; Reset state machine
            jmp     exit
STATE8      ; Send Data byte
            bic.b   #USIOE,&USICTL0         ; SDA = input
            bis.b   #0x08,&USICNT           ; Bit counter = 8, RX data
            mov.w   #10,I2CState            ; Go to next state: Test data and (N)Ack
            jmp     exit
STATE10     ; Check Data & TX (N)Ack
            bis.b   #USIOE,&USICTL0         ; SDA = output
            cmp.b   mstr_data,&USISRL       ; If data valid ...
            jne     Data_NACK
            clr.b   &USISRL                 ; Send Ack
            inc.b   mstr_data               ; Increment master data
            bic.b   #BIT2,&P1OUT            ; LED off
            jmp     STATE10_Exit
Data_NACK
            mov.b   #0xFF,&USISRL           ; Send Nack
            bis.b   #BIT2,&P1OUT            ; LED on : error
STATE10_Exit
            bis.b   #0x1,&USICNT            ; Bit counter = 1, Send Nack
            mov.w   #6,I2CState             ; next state, prep for next start

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
