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
;   MSP430x11x1 Demo - Timer_A, Ultra-Low Pwr UART 9600 Echo, 32kHz ACLK
;
;   Description: Use Timer_A CCR0 hardware output modes and SCCI data latch to
;   to implement UART function @ 9600 baud. Software does not directly read and
;   write to RX and TX pins, instead proper use of output modes and SCCI data
;   latch are demonstrated. Use of these hardware features eliminates ISR
;   latency effects as hardware insures that output and input bit latching and
;   timing are perfectly synchronised with Timer_A regardless of other
;   software activity. In the Mainloop the UART function readies the UART to
;   receive one character and waits in LPM3 with all activity interrupt driven.
;   After a character has been received, the UART receive function forces exit
;   from LPM3 in the Mainloop which echos back the received character.
;
;   To demonstrate 9600 buad with a 32768 XTAL used as the buad rate generator,
;   a look-up table is used to modulate between a clock divider or 3 and 4.
;   Using modulation, a buad rate of ~9600 is achieved.
;   ACLK = TACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO ~800kHz
;   ;* An external watch crystal is required on XIN XOUT for ACLK *//	
;
;                 MSP430F1121
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |   CCI0A/TXD/P1.1|-------->
;            |                 | 9600 8N1
;            |   CCI0B/RXD/P2.2|<--------
;
RXD         .equ    004h                    ; RXD on P2.2
TXD         .equ    002h                    ; TXD on P1.1
;
;   CPU Registers Used
RXTXData .equ     R4
BitCnt   .equ     R5
;
;   Conditions for 9600 Baud SW UART, ACLK = 32768
Bitime_5    .equ    1                       ; ~0.5 bit length + small adjustment
Bitime      .equ    3                       ;
;
;   M. Buccini / Z. Albus
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;*******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize Stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupTA     mov.w   #TASSEL_1+MC_2,&TACTL   ; ACLK, continuous mode
SetupC0     mov.w   #OUT,&CCTL0             ; TXD Idle as Mark
SetupP1_2   bis.b   #TXD,&P1SEL             ; P1.1/TA0 for TXD function
            bis.b   #TXD,&P1DIR             ; TXD output on P1
            bis.b   #RXD,&P2SEL             ; P2.2/TA0 as RXD input
                                            ;
Mainloop    call    #RX_Ready               ; UART ready to RX one Byte
            bis.w   #LPM3+GIE,SR            ; Enter LPM3 w/int Until Byte RXed
            call    #TX_Byte                ; TX Back RXed Byte Received
            jmp     Mainloop                ;
                                            ;
;-------------------------------------------------------------------------------
TX_Byte;    Subroutine that will TX Byte from RXTXData Buffer
;-------------------------------------------------------------------------------
TX_1        mov.w   &TAR,&CCR0              ; Current state of TA counter
            cmp.w   &TAR,&CCR0              ; !!Prevent async capature!!
            jne     TX_1                    ;
            add.w   #Bitime,&CCR0           ; Some time till first bit
            mov.w   #OUTMOD2+OUTMOD0+CCIE,&CCTL0; TXD = Space = Start Bit
TX_Wait     bit.w   #CCIE,&CCTL0            ; Wait for TX completion
            jnz     TX_Wait                 ;
            ret                             ; Return from subroutine
                                            ;
;-------------------------------------------------------------------------------
RX_Ready;    Subroutine that Readies UART to RX Byte into RXTXData Buffer
;-------------------------------------------------------------------------------
            mov.w   #RX_Count,BitCnt        ; RX_Count --> Branch Pointer
            mov.w   #CM1+CCIS0+SCS+CAP+OUT+CCIE,&CCTL0; Neg Edge, Cap., Out
            ret                             ; Return from subroutine
                                            ;
;-------------------------------------------------------------------------------
TA0_ISR;    RXTXData Buffer holds UART Data
;-------------------------------------------------------------------------------
            add.w   #Bitime,&CCR0           ; Bitime till next bit
            br      @BitCnt+                ; Branch To Routine
                                            ;
TX_Bit1     add.w   #01h,&CCR0              ;
TX_Bit      rra.b   RXTXData                ; LSB is shifted to carry
            jc      TX_Mark                 ; Jump if bit = 1
TX_Space    bis.w   #OUTMOD2,&CCTL0         ; TX Space
            reti                            ;
TX_Comp     bic.w   #CCIE,&CCTL0            ; All Bits RX, disable interrupt
TX_Mark     bic.w   #OUTMOD2,&CCTL0         ; TX Mark
            reti                            ;
RX_Edge     bic.w   #CAP,&CCTL0             ; Switch to Compare mode
            add.w   #Bitime_5,&CCR0         ; First Databit 1.5 Bits from edge
            reti                            ;
RX_Bit1     add.w   #01h,&CCR0              ;
RX_Bit      bit.w   #SCCI,&CCTL0            ; Get bit waiting in SCCI
            rrc.b   RXTXData                ; Store received bit
            reti                            ;
;>>>>>>>>>> Decode of Received Byte Here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
RX_Comp     bic.w   #CCIE,&CCTL0            ; All Bits RXed, Disable Interrupt
            mov.w   #GIE,0(SP)              ; Decode Byte= Active in Mainloop
            reti                            ;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            .align 2
RX_Count    .short  RX_Edge                 ; Special for TA
            .short  RX_Bit                  ; RX First Data Bit
            .short  RX_Bit1                 ;
            .short  RX_Bit                  ;
            .short  RX_Bit                  ;
            .short  RX_Bit1                 ;
            .short  RX_Bit                  ;
            .short  RX_Bit1                 ;
            .short  RX_Bit                  ;
            .short  RX_Comp                 ; RX Complete, process RX data
TX_Count    .short  TX_Bit                  ; TX First Data Bit
            .short  TX_Bit1                 ;
            .short  TX_Bit                  ;
            .short  TX_Bit                  ;
            .short  TX_Bit1                 ;
            .short  TX_Bit                  ;
            .short  TX_Bit1                 ;
            .short  TX_Bit                  ;
            .short  TX_Mark                 ; TX Stop Bit= Mark
TX_End      .short  TX_Comp                 ; TX Complete and Complete
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int09"                ; Timer_A0 Vector
            .short  TA0_ISR                 ;
            .end

