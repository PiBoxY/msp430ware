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
;   MSP430x21x1 Demo - Timer_A, Ultra-Low Pwr UART 9600 Echo,1MHz DCO SMCLK
;
;   Description: This program demonstrates a full-duplex 9600-baud UART using
;   Timer_A3 and the DCO. A character is echoed on the Hyperterminal of a
;   a PC. The DCO frequency settings are stored in INFOA flash segment.
;   ACLK = n/a, MCLK = SMCLK = saved DCO 1Mhz
;
;                MSP430F21x1
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |   CCI0A/TXD/P1.1|-------->
;            |                 | 9600 8N1
;            |   CCI0B/RXD/P2.2|<--------
;
;   H. Grewal / A. Dannenberg
;   Texas Instruments Inc.
;   June 2005
;   Built with Code Composer Essentials Version: 1.0
;*****************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;   CPU registers used
RXTXData .equ     R4
BitCnt   .equ     R5
;
;   Conditions for 9600 Baud HW/SW UART, SMCLK = DCOCLK ~ 1MHz
Bitime_5    .equ    52                      ; ~ 0.5 bit length
Bitime      .equ    104                     ; ~ 9615 baud

RXD         .set    004h                    ; RXD on P2.2
TXD         .set    002h                    ; TXD on P1.1
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize Stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            cmp.b   #0xFF,&CALBC1_1MHZ      ; If calibration constants erased
            jne     Load                    ; if not erased, load.      
Trap        jmp     Trap                    ; if erased do not load, trap CPU!
Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   &CALBC1_1MHZ,&BCSCTL1   ; Set DCO
            mov.b   &CALDCO_1MHZ,&DCOCTL

Setup_TA    mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK, ContMode
SetupC0     mov.w   #OUT,&CCTL0             ; TXD Idle as Mark
SetupP1_2   bis.b   #TXD,&P1SEL             ; P1.1/TA0 for TXD function
            bis.b   #TXD,&P1DIR             ; TXD output on P1
            bis.b   #RXD,&P2SEL             ; P2.2/TA0 as RXD input
            bis.b   #08h,&P1DIR
            bis.b   #08h,&P1OUT
                                            ;
Mainloop    call    #RX_Ready               ; UART ready to RX one Byte
            bis.w   #LPM4+GIE,SR            ; Enter LPM4 w/ int until Byte RXed
            call    #TX_Byte                ; TX Back RXed Byte Received
            jmp     Mainloop                ;
                                            ;
;-----------------------------------------------------------------------------
TX_Byte   ; Subroutine that Transmit One Byte from RXTXData Buffer.
;-----------------------------------------------------------------------------
            mov.w   &TAR,&CCR0              ; Current state of TA Counter
            add.w   #Bitime,&CCR0           ; Some time till first bit
            bis.w   #0100h, RXTXData        ; Add mark stop bit to RXTXData
            rla.w   RXTXData                ; Add space start bit
            mov.w   #10,BitCnt              ; Load Bit Counter, 8data + ST/SP
            mov.w   #OUTMOD0+CCIE,&CCTL0    ; TXD = mark = idle
TX_Wait     bit.w   #CCIE,&CCTL0            ; Wait for TX completion
            jnz     TX_Wait                 ;
            ret                             ;
                                            ;
;-----------------------------------------------------------------------------
RX_Ready  ; Subroutine that will Receive One Byte into RXTXData Buffer.
          ; !! Sync capture not possible as DCO=TACLK=SMCLK can be off !!
;-----------------------------------------------------------------------------
            mov.w   #8,BitCnt               ; Load Bit Counter, 8 data bits
SetupRX     mov.w   #CM1+CCIS0+OUTMOD0+CAP+CCIE,&CCTL0  ; Neg Edge,Cap
            ret                             ;
                                            ;
;-----------------------------------------------------------------------------
TA0_ISR  ;  CCR0/UART ISR:   RXTXData Buffer holds UART Data.
;-----------------------------------------------------------------------------
            add.w   #Bitime,&CCR0           ; Time to Next Bit
            bit.w   #CCIS0,&CCTL0           ; RX on ISCCIB?
            jnz     UART_RX                 ; Jump --> RX
UART_TX     cmp.w   #00h,BitCnt             ;
            jne     TX_Next                 ; Next bit?
            bic.w   #CCIE,&CCTL0            ; All Bits TX or RX, Disable Int.
            reti                            ;
TX_Next     bic.w   #OUTMOD2,&CCTL0         ; TX Mark
            rra.w   RXTXData                ; LSB is shifted to carry
            jc      TX_Test                 ; Jump --> bit = 1
TX_Space    bis.w   #OUTMOD2,&CCTL0         ; TX Space
TX_Test     dec.w   BitCnt                  ; All bits sent (or received)?
            reti                            ;
                                            ;
UART_RX     bit.w   #CAP,&CCTL0             ; Compare mode for start bit edge
            jz      RX_Bit                  ; Start bit edge?
RX_Edge     bic.w   #CAP,&CCTL0             ; Switch to Compare mode
            add.w   #Bitime_5,&CCR0         ; First databit 1.5 bits from edge
            mov.w   #CPUOFF+GIE,0(SP)       ; !!! DCO needs to remain on !!!
            reti                            ;
RX_Bit      bit.w   #SCCI,&CCTL0            ; Get bit waiting in receive latch
            rrc.b   RXTXData                ; Store received bit
RX_Test     dec.w   BitCnt                  ; All bits sent (or received)?
            jnz     RX_Next                 ; Next bit?
;>>>>>>>>>> Decode of Received Byte Here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
RX_Comp     bic.w   #CCIE,&CCTL0            ; All Bits RXed, Disable Interrupt
            mov.w   #GIE,0(SP)              ; Decode Byte= Active in Mainloop
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
RX_Next     reti                            ;
;
;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect ".reset"                  ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect ".int09"                  ; Timer_A0 Vector
            .short  TA0_ISR                 ;
            .end

