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
;   MSP430x24x Demo - USCI_A0, SPI Interface to HC164 Shift Register
;
;   Description: This program demonstrates USCI_A0 in SPI mode, interfaced to
;   a 'HC164 shift register, transferring the value contained in Data.
;   The value is incremented in the mainloop, effectively incrementing
;   HC164 QA - QH.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~1.045MHz, BRCLK = SMCLK/2
;
;                MSP430F249
;             -----------------
;         /|\|              XIN|-
;          | |                 |        ^      HC164
;          --|RST          XOUT|-       |  -------------
;            |                 |        |-|/CLR,B       |  8
;            |    UCA0SIMO/P3.4|--------->|A          Qx|--\->
;            |     UCA0CLK/P3.0|--------->|CLK          |
;            |                 |          |             |
;
;
;  JL Bile
;  Texas Instruments Inc.
;  May 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
Data        .equ     0200h
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0500h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP3     bis.b   #11h,&P3SEL             ; P3.0,4 USCI_A0 option select
SetupSPI    bis.b   #UCCKPH+UCMSB+UCMST+UCSYNC,&UCA0CTL0 ; 3-pin, 8-bit SPI mast
            bis.b   #UCSSEL_2,&UCA0CTL1     ; SMCLK
            bis.b   #02h,&UCA0BR0
            clr.b   &UCA0BR1                ;
            clr.b   &UCA0MCTL
            bic.b   #UCSWRST,&UCA0CTL1      ; **Initialize USCI state machine**
            mov.b   #0FFh,&Data             ; Load inital data
                                            ;
Mainloop    inc.b   &Data                   ; Increment Data value
            call    #TX_HC164               ; Data --> HC164
            call    #Delay                  ; Wait
            jmp     Mainloop                ; Repeat
                                            ;
;-------------------------------------------------------------------------------
TX_HC164;   SPI Data  --> HC164
;-------------------------------------------------------------------------------
TX0         bit.b   #UCA0TXIFG,&IFG2        ; USCI_A0 TX buffer ready?
            jz      TX0                     ; Jump --> TX buffer not ready
            mov.b   &Data,&UCA0TXBUF        ; Byte to SPI TXBUF
            ret                             ; Return from subroutine
                                            ;
;-------------------------------------------------------------------------------
Delay;      Software delay
;-------------------------------------------------------------------------------
            push.w  #0FFFFh                 ; Delay to TOS
L1          dec.w   0(SP)                   ; Decrement TOS
            jnz     L1                      ; Delay over?
            incd.w  SP                      ; Clean TOS
            ret                             ;
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".reset"            ; POR, ext. Reset
            .short  RESET      
            .end
