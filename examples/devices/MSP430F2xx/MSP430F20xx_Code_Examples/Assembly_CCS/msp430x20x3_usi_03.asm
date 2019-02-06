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
;   MSP430F20x2/3 Demo - SPI full-Duplex 3-wire Slave
;
;   Description: SPI Master communicates full-duplex with SPI Slave using
;   3-wire mode. The level on P1.4 is TX'ed and RX'ed to P1.0.
;   Master will pulse slave reset for synch start.
;   ACLK = n/a, MCLK = SMCLK = Default DCO
;
;                 Slave                      Master
;                MSP430F20x2/3              MSP430F20x2/3
;              -----------------          -----------------
;             |              XIN|-    /|\|              XIN|-
;             |                 |      | |                 |
;             |             XOUT|-     --|RST          XOUT|-
;             |                 | /|\    |                 |
;             |          RST/NMI|--+<----|P1.2             |
;       LED <-|P1.0             |        |             P1.4|<-
;           ->|P1.4             |        |             P1.0|-> LED
;             |         SDI/P1.7|<-------|P1.6/SDO         |
;             |         SDO/P1.6|------->|P1.7/SDI         |
;             |        SCLK/P1.5|<-------|P1.5/SCLK        |
;
;   P.Thanigai
;   Texas Instruments Inc.
;   May 2007
;   Built with Code Composer Essentials Version: 2.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP1     mov.b   #010h,&P1OUT            ; P1.4 set, else
            bis.b   #010h,&P1REN            ; P1.4 pullup
            bis.b   #001h,&P1DIR            ;
SetupUSI    bis.b   #USIPE7+USIPE6+USIPE5+USIOE,&USICTL0; Port, SPI slave
            bis.b   #USIIE,&USICTL1         ; Counter interrupt, flag remains
            bic.b   #USISWRST,&USICTL0      ; Enable USI
            mov.b   &P1IN,&USISRL           ; init-load TX data
            mov.b   #08h,&USICNT            ; init-load counter, clear flag
                                            ;
Mainloop    bis.b   #LPM4+GIE,SR            ; LPM4 w/ interrupts enabled
            nop                             ; Required for debugger only
                                            ;
;------------------------------------------------------------------------------
USI_ISR;
;------------------------------------------------------------------------------
            mov.b   &USISRL,R4              ; Temp save RX'ed char
            mov.b   &P1IN,&USISRL           ;
            mov.b   #08h,&USICNT            ; re-load counter, clear flag
            bic.b   #01h,&P1OUT             ;
            bit.b   #010h,R4                ;
            jnc     L2                      ;
L1          bis.b   #01h,&P1OUT             ;
L2          reti                            ; Exit ISR
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int04"                ; USI Vector
            .short  USI_ISR                 ;        
            .end
