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
;   MSP-FET430P140 Demo - USART0, SPI Interface to HC165 Shift Register
;
;   Description: This program demonstrate a USART0 in SPI mode interface to a
;   an 'HC165 shift register.  Read 8-bit Data from A-H are stored at a RAM
;   byte 0200h defined as Data.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~ 800k, UCLK0 = SMCLK/2
;   // **SWRST** please see MSP430x1xx Users Guide for description //
;
;                          MSP430F149
;                       -----------------
;                   /|\|              XIN|-
;                    | |                 |
;          HC165     --|RST          XOUT|-
;        ----------    |                 |
;    8  |      /LD|<---|P3.0             |
;   -\->|A-H   CLK|<---|P3.3/UCLK0       |
;     |-|INH    QH|--->|P3.2/SOMI        |
;     |-|SER      |    |                 |
;     v
;
Data        .equ    0200h
;
;   M. Buccini / G. Morton
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-----------------------------------------------------------------------------
            .text                           ; Program Start
;-----------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
            call    #Init_Sys               ; Initialize system
                                            ;
Mainloop    call    #RX_HC165               ; Read HC165
            mov.b   &RXBUF0,&Data           ; Data = HC165
            jmp     Mainloop                ; Repeat
                                            ;
;-----------------------------------------------------------------------------
RX_HC165
;-----------------------------------------------------------------------------
            bic.b   #01h,&P3OUT             ; Latch data into 'HC165
            bis.b   #01h,&P3OUT             ;
            bic.b   #URXIFG0,&IFG1          ; Clear RXBUF flag
            mov.b   #00h,&TXBUF0            ; Dummy write to start SPI
L1          bit.b   #URXIFG0,&IFG1          ; RXBUF ready?
            jnc      L1                     ; 1 = ready
            ret                             ;
                                            ;
;-----------------------------------------------------------------------------
Init_Sys;   Subroutine to Initialize MSP430F149 Peripherials
;-----------------------------------------------------------------------------
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP3     bis.b   #0Ch,&P3SEL             ; P3.2,3 SPI option select
            bis.b   #09h,&P3DIR             ; P3.3,0 output direction
SetupSPI    bis.b   #040h,&ME1              ; Enable USART0 SPI
            bis.b   #CKPH+SSEL1+SSEL0+STC,&UTCTL0 ;* SMCLK, 3-pin mode
            bis.b   #CHAR+SYNC+MM,&UCTL0    ; 8-bit SPI Master **SWRST**
            mov.b   #02h,&UBR00             ; SMCLK/2 for baud rate
            clr.b   &UBR10                  ; SMCLK/2 for baud rate
            clr.b   &UMCTL0                 ; Clear modulation
            bic.b   #SWRST,&UCTL0           ; **Initialize USART state machine**
            ret                             ; Return from subroutine
;
;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
             .sect   ".reset"                ; POR, ext. Reset, Watchdog, Flash
             .short  RESET                   ;
             .end

