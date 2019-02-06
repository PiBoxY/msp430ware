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
;  MSP430F20x2/3 Demo - SPI 3-Wire Slave Data Echo
;
;  Description: SPI slave talks to SPI master using 3-wire mode. Data received
;  from master is echoed back.
;  ACLK = VLO, MCLK = SMCLK = Default DCO
;
;  Note: Ensure the slave device is running before the master device starts
;    code execution
;
;                    Slave                     Master
;                                      (msp430g2230_usi_02.asm)
;                 MSP430G2230                MSP430G2230
;             -----------------          -----------------
;            |              XIN|-    /|\|              XIN|-
;            |                 |      | |                 |
;            |             XOUT|-     --|RST          XOUT|-
;            |                 | /|\    |                 |
;            |          RST/NMI|--+     |                 |
;            |                 |        |                 |
;            |                 |        |             P1.2|-> LED
;            |         SDI/P1.7|<-------|P1.6/SDO         |
;            |         SDO/P1.6|------->|P1.7/SDI         |
;            |        SCLK/P1.5|<-------|P1.5/SCLK        |
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
;******************************************************************************

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

            bis.b   #USIPE7+USIPE6+USIPE5+USIOE,&USICTL0; Ports, SPI slave
            bis.b   #USIIE,&USICTL1         ; Counter interrupt, flag remains set
            bic.b   #USISWRST,&USICTL0      ; USI released for operation
            mov.b   #0,&USISRL              ; init-load data, dummy data
            mov.b   #8,&USICNT              ; init-load counter

            bis.w   #LPM0+GIE,SR            ; LPM0 and interrupts
            nop

;-------------------------------------------------------------------------------
USI_ISR
;-------------------------------------------------------------------------------
            mov.b   &USISRL,R4              ; R4 = Rx_Data
            mov.b   R4,&USISRL              ; Echo back Rxed character
            mov.b   #8,USICNT               ; re-load counter
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   USI_VECTOR              ; USI
            .short  USI_ISR
            .end
