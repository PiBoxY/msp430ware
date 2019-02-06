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
;   MSP430G2x55 Demo - USCI_A0 IrDA Monitor, 8MHz SMCLK
;
;   Description: This example receives bytes through the USCI module
;   configured for IrDA mode, and sends them out as ASCII strings using the
;   Timer_A UART to a PC running a terminal software. The code can be used
;   to monitor and log an IrDA communication.
;
;   ACLK = n/a, MCLK = SMCLK = BRCLK = CALxxx_8MHZ = 8MHz
;
;              MSP430G2x55                   MSP430G2x55
;            -----------------            -----------------------
;           |                 |
;        /|\|              XIN|-         |                       |
;         | |                 |       /|\|                    XIN|-
;         --|RST          XOUT|-         |                       |
;           |                 |        --|RST                XOUT|-
;           |                 |          |                       |
;           |                 |          |                       |
;           |                 |          |                       |
;           |                 |          |               P2.4/TA2|--> 115,200 8N1
;           |                 |          |                       |    Terminal SW
;           |     P3.5/UCA0RXD|<-------  |P3.4/UCA0TXD           |
;           |     P3.4/UCA0TXD|--------> |P3.5/UCA0RXD           |
;           |                 |          |                       |
;           |                 |          |                       |
;           ------------------            -----------------------
;
;
;  William Goh
;  Texas Instruments Inc.
;  December 2012
;  Built with Code Composer Studio v5.4
;*******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
BITTIME     .set     69                     ; UART bit time = 8MHz / 115,200

TXData      .usect  ".bss",2                ; Data to transmit
TxBitCnt    .usect  ".bss",1                ; Keeps track of # bits TX'd
;------------------------------------------------------------------------------
            .text                           ; Assemble into program memory
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section
            .retainrefs                     ; Additionally retain any sections
                                            ; that have references to current
                                            ; section
;------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
CheckCal	cmp.b   #0xFF,&CALBC1_8MHZ		; Check calibration constant
            jne     SetupBCS				; if not erased, load.           
Trap        jmp     Trap					; if erased do not load, trap CPU!
SetupBCS    clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   &CALBC1_8MHZ,&BCSCTL1   ; Load 8MHz constants
            mov.b   &CALDCO_8MHZ,&DCOCTL    ;
SetupP2     bis.b   #010h,&P2SEL            ; Use P2.4 for Timer_A
            bis.b   #010h,&P2DIR            ; P2.4 output
SetupP3     bis.b   #030h,&P3SEL            ; Use P3.4/P3.5 for USCI_A0
SetupUSCI0  bis.b   #UCSWRST,&UCA0CTL1      ; Set SW Reset
            mov.b   #UCSSEL_2+UCSWRST,&UCA0CTL1
                                            ; Use SMCLK, keep SW reset
            mov.b   #52,&UCA0BR0            ; 8MHz/52=153.8KHz
            mov.b   #0,&UCA0BR1             ;
            mov.b   #UCBRF_1+UCOS16,&UCA0MCTL
                                            ; Set 1st stage modulator to 1
                                            ; 16-times oversampling mode
            mov.b   #UCIRTXPL2+UCIRTXPL0+UCIRTXCLK+UCIREN,&UCA0IRTCTL
                                            ; Pulse length = 6 half clock cyc
                                            ; Enable BITCLK16, IrDA enc/dec
            mov.b   #UCIRRXPL,&UCA0IRRCTL   ; Light = low pulse
            bic.b   #UCSWRST,&UCA0CTL1      ; Resume operation
SetupTA     mov.w   #OUT,&TACCTL2           ; TXD Idle as Mark
            mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK, continuous mode
                                            ;
Mainloop    dint                            ;
            bis.b   #UCA0RXIE,&IE2          ; Enable RX int
            bis.w   #CPUOFF+GIE,SR          ; Enter LPM0, interrupts enabled
            mov.b   R5,R6                   ; Save received character
            clrc                            ;
            rrc.b   R5                      ; R5 >>= 4
            rra.b   R5                      ;
            rra.b   R5                      ;
            rra.b   R5                      ;
Rdy4TX1     bit.w   #CCIE,&TACCTL2          ; Loop while TX is pending
            jc      Rdy4TX1                 ;
            mov.b   Nibble2ASCII(R5),R15    ;
            call    #TX_Byte                ; TX upper nibble
                                            ;
Rdy4TX2     bit.w   #CCIE,&TACCTL2          ; Loop while TX is pending
            jc      Rdy4TX2                 ;
            and.b   #0fh,R6                 ;
            mov.b   Nibble2ASCII(R6),R15    ; TX lower nibble
            call    #TX_Byte                ;
                                            ;
Rdy4TX3     bit.w   #CCIE,&TACCTL2          ; Loop while TX is pending
            jc      Rdy4TX3                 ;
            mov.b   #' ',R15                ; TX space character
            call    #TX_Byte                ;
            jmp     Mainloop                ; Again
;-------------------------------------------------------------------------------
TX_Byte;    TX the byte stored in R15 using Timer_A UART
;-------------------------------------------------------------------------------
            mov.b   #10,&TxBitCnt           ; Load Bit counter, 8 data + ST/SP
            mov.w   &TAR,&TACCR2            ; Current state of TA counter
            add.w   #BITTIME,&TACCR2        ; Some time till first bit
            bis.w   #0100h,R15              ; Add mark stop bit
            rla.w   R15                     ; Add space start bit
            mov.w   R15,&TXData             ; Load global variable
            mov.w   #OUTMOD0+CCIE,&TACCTL2  ; TXD = mark = idle
            ret                             ;
;-------------------------------------------------------------------------------
USCIRX_ISR; Read RXed character, return from LPM0
;-------------------------------------------------------------------------------
            mov.b   &UCA0RXBUF,R5           ; Get RXed character
            bic.b   #UCA0RXIE,&IE2          ; Disable RX int
            bic.w   #CPUOFF,0(SP)           ; Return active after receiption
            reti                            ; Return from ISR
;-------------------------------------------------------------------------------
TAX_ISR;    Common ISR for TACCR1-2 and overflow
;-------------------------------------------------------------------------------
            add.w   &TAIV,PC                ; Add Timer_A offset vector
            reti                            ; No interrupt
            reti                            ; TACCR1 - not used
            jmp     TACCR2_ISR              ; TACCR2
            reti                            ; Reserved
            reti                            ; Reserved
            reti                            ; Overflow - not used
;-------------------------------------------------------------------------------
TACCR2_ISR; Timer_A UART TX
;-------------------------------------------------------------------------------
            add.w   #BITTIME,&TACCR2        ; Add Offset to TACCR2
            tst.b   &TxBitCnt               ; All bits TXed?
            jnz     TX_Cont                 ; Jump if not
            bic.w   #CCIE,&TACCTL2          ; All bits TXed, disable interrupt
            reti                            ;
TX_Cont     bit.w   #01h,&TXData            ;
            jz      TX_Zero                 ;
            bic.w   #OUTMOD2,&TACCTL2       ; TX Mark
            jmp     TX_Cont2                ;
TX_Zero     bis.w   #OUTMOD2,&TACCTL2       ; TX Space
TX_Cont2    rra.w   &TXData                 ;
            dec.b   &TxBitCnt               ;
            reti                            ;
;-------------------------------------------------------------------------------
;Table for nibble-to-ASCII conversion
;-------------------------------------------------------------------------------
Nibble2ASCII .byte '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
;-------------------------------------------------------------------------------
;           Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect 	.stack
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int07"                ; USCI Vector
            .short  USCIRX_ISR              ; 
            .sect   ".int08"                ; Timer_AX Vector
            .short  TAX_ISR                 ;        
            .end
