; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2018, Texas Instruments Incorporated
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
;  MSP430x09x Demo - Demonstration of calling various API functions in ROM
;
;  Description: This example calls the SWID, SpiReadByte, and ApiCall functions
;               available in the loader code the the MSP430x09x devices. For
;               more information regarding the usage of these APIs please refer
;               to SLAU324 (https://www.ti.com/lit/slau324)
;
;               This example was tested using the MSP-TS430L092 EVM with the
;               M95160R EEPROM on-board.
;
;  WARNING:     The GPIO used for SPI communication in this example are the same
;               used for the JTAG interface. Therefore, the code will not operate
;               properly if run in debug mode.
;
;  Caleb Overbay
;  Texas Instruments Inc.
;  April 2018
;*******************************************************************************

    .cdecls C,LIST,"msp430.h"
    .cdecls C,LIST,"loader.h"  ; header with API function defintions
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
JTAG_SEL_PORT0   .set   P2SEL0  ; JTAG port select 0
JTAG_SEL_PORT1   .set   P2SEL1  ; JTAG port select 1

; SPI pin definitions
SPI_PORT_OUT     .set   P2OUT
SPI_PORT_DIR     .set   P2DIR
SPI_MOSI_PIN     .set   BIT1
SPI_MISO_PIN     .set   BIT3
SPI_CS_PIN       .set   BIT0
SPI_CLK_PIN      .set   BIT2

;-------------------------------------------------------------------------------
            .global main
            .text                           ; Assemble to memory
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

main
    ; Set P1.1 and P1.0 to output low for debugging purposes
    bic.b   #BIT1 + BIT0,  &P1OUT
    bis.b   #BIT1 + BIT0,  &P1DIR

InitAPI
    call    #initAPI                    ; Init peripherals necessary for API

    bic.w   #RAMLCK1+RAMLCK0, &SYSCNF   ; Unlock API software registers

SWID_Test
    call    &SWID                       ; Call SWID
    cmp.w   #0x2843,R12                 ; Expecting 0x2843 in R12
    jne     SWID_Failure                ; If SWID call fails, jump


LedOn_Test
    call   &LedOn           ; turn on power supply to EEPROM

SpiReadByte_Test
    mov.w  #0x0000,R12      ; upper byte of EEPROM address
    mov.w  #0x0000,R13      ; lower byte of EEPROM address
    call   &SpiReadByte     ; Read from address 0x0000 of EEPROM

    cmp.b  #0x55, R12       ; Compare the results read with what is expected
    jne    Read_Failure     ; SWID call failed

LedOff_Test
    call   &LedOff          ; turn off power supply to EEPROM

    bis.w   #RAMLCK1+RAMLCK0, &SYSCNF ; Lock API software registers

main_loop
    nop
    jmp main_loop    ; Loop forever
    nop

SWID_Failure
    nop
    bis.b   #BIT0,  &P1OUT ; Set P1.0 high to indicate SWID failure
    jmp     main_loop
    nop

Read_Failure
    nop
    bis.b   #BIT1,  &P1OUT ; Set P1.1 high to indicate SpiReadByte failure
    jmp     main_loop
    nop

; Initializes Compact Clock System, JTAG ports, SPI pins, & API software regs
initAPI:
   ; Setup clocks
    mov.w   #0A500h, &CCSCTL0        ; open CCS
    mov.w   #0000h,  &CCSCTL7        ; clr all OFFGs
    mov.w   #0000h,  &SFRIFG1        ; clr any pending system interrupts
    mov.w   #0000h,  &CCSCTL4        ; Set HF OSC to all outputs
    mov.w   #0000h,  &CCSCTL5        ; set speed to 1MHz

    ; Disable JTAG (SPI comm is on the same pins as JTAG)
    clr.b   &JTAG_SEL_PORT0
    clr.b   &JTAG_SEL_PORT1

    ; Configure SPI pins
    mov.b   #SPI_CS_PIN,  &SPI_PORT_OUT ; set Chip Select high

    ;Set CS, MOSI, and CLK to output, MISO to input
    mov.b   #SPI_CS_PIN + SPI_MOSI_PIN + SPI_CLK_PIN, &SPI_PORT_DIR

    bic.w   #RAMLCK1+RAMLCK0, &SYSCNF  ; Unlock API software registers

    ; Initialize API software regs
    mov.w   #0x00,   &Status_Reg              ; initialize status reg
    mov.w   #0x00,   &CurOvlSpiH              ; initialize overlay high addr
    mov.w   #0x02,   &CurOvlSpiL              ; initialize overlay low addr
    mov.w   #0x00,   &CurOvlAdrL              ; initialize 430 adr
    mov.w   #TimerA0_2_PWM_On, &LedOnPtr      ; initialize LedOn socket
    mov.w   #TimerA0_2_PWM_Off, &LedOffPtr    ; initialize LefOff socket

    bis.w   #RAMLCK1+RAMLCK0, &SYSCNF         ; Unlock API software registers

    ret

; Initializes and turns on TA0.2 PWM output at 200kHz
TimerA0_2_PWM_On:
    ; Configure P1.2 for TA0.2 PWM Output (Used for LedOn and LedOff)
    bic.b   #BIT2,  &P1SEL1
    bis.b   #BIT2,  &P1SEL0
    bis.b   #BIT2,  &P1DIR

    ; Setup TA0.2 for 200kHz PWM
    mov.w   #4,      &TA0CCR0               ; period of 4
    mov.w   #1,      &TA0CCR2               ; at tick #1
    mov.w   #OUTMOD_4, &TA0CCTL2            ; Toggle mode output
    mov.w   #TASSEL_2+MC__UP+TACLR, &TA0CTL ; Source = SMCLK, Up mode, Clr timer

    ret

; Turns off TA0.2 PWM output
TimerA0_2_PWM_Off:
    mov.w #0, &TA0CTL ; Stop the timer

    bic.b   #BIT2,  &P1SEL1  ; Set P1.2 to GPIO
    bic.b   #BIT2,  &P1SEL0  ; Set P1.2 to GPIO
    bic.b   #BIT2,  &P1OUT   ; Set P1.2 to output low

    ret

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack

;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
