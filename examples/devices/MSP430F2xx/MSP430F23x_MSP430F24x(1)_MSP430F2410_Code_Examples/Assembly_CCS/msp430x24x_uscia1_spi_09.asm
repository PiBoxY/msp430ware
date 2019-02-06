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
;   MSP430x24x Demo - USCI_A1, SPI 3-Wire Master Incremented Data
;
;   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
;   data is sent by the master starting at 0x01. Received data is expected to
;   be same as the previous transmission.  USCI RX ISR is used to handle
;   communication with the CPU, normally in LPM0. If high, P1.0 indicates
;   valid data reception.
;   ACLK = n/a, MCLK = SMCLK = DCO ~1.045MHz, BRCLK = SMCLK/2 
;
;   Use with SPI Slave Data Echo code example. If slave is in debug mode, P3.6
;   slave reset signal conflicts with slave's JTAG; to work around,  control the
;   the Master device with CCE.  If breakpoints are inserted in Master slave
;   must stop also.
;
;                    MSP430F249
;                 -----------------
;             /|\|              XIN|-
;              | |                 |
;              --|RST          XOUT|-
;                |                 |
;                |             P3.6|-> Data Out (UCA1SIMO)
;                |                 |
;          LED <-|P1.0         P3.7|<- Data In (UCA1SOMI)
;                |                 |
;  Slave reset <-|P3.5         P5.0|-> Serial Clock Out (UCA1CLK)
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

MST_Data    .equ   R6
SLV_Data    .equ    R7
;-------------------------------------------------------------------------------
            .text                           ;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0500h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
CheckCal    cmp.b   #0xFF,&CALBC1_1MHZ      ; Check calibration constant
            jne     Load                    ; if not erased, load.         
Trap        jmp     Trap                    ; if erased do not load, trap CPU!
Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   &CALBC1_1MHZ,&BCSCTL1   ; Set DCO to 1MHz
            mov.b   &CALDCO_1MHZ,&DCOCTL    ;
SetupP1     clr.b   &P1OUT                  ; 
            bis.b   #01h,&P1DIR             ; P1 setup for LED 
SetupP3     bis.b   #020h,&P3OUT
            bis.b   #020h,&P3DIR
            bis.b   #0C0h,&P3SEL            ; P3.6,7 USCI_A1 option select
SetupP5     bis.b   #01h,&P5SEL             ; P5.0 USCI_A1 option select            
SetupSPI    bis.b   #UCCKPL+UCMSB+UCMST+UCSYNC,&UCA1CTL0;3-pin, 8-bit SPI master
            bis.b   #UCSSEL_2,&UCA1CTL1     ; SMCLK
            bis.b   #02h,&UCA1BR0           ; /2
            clr.b   &UCA1BR1                ;
            clr.b   &UCA1MCTL               ; No modulation
            bic.b   #UCSWRST,&UCA1CTL1      ; **Initialize USCI state machine**
            bis.b   #UCA1RXIE,&UC1IE        ; Enable USCI_A1 RX interrupt

            bic.b   #020h,&P3OUT            ; Now with SPI signals initialized,
            bis.b   #020h,&P3OUT            ; reset slave
            mov.w   #50,R15                 ; Wait for slave to initialize
waitForSlv  dec.w   R15                     ;
            jnz     waitForSlv              ;
                                            ;
            mov.b   #001h,MST_Data          ; Initialize data values
            mov.b   #000h,SLV_Data          ;
                                            ;
Mainloop    mov.b   MST_Data,&UCA1TXBUF     ; Transmit first character
            bis.b   #LPM0+GIE,SR            ; CPU off, enable interrupts
            nop                             ; Required for debugger only
                                            ;
;-------------------------------------------------------------------------------
USCIA1RX_ISR;       Test for valid RX and TX character
;-------------------------------------------------------------------------------
TX1         bit.b   #UCA1TXIFG,&UC1IFG      ; USCI_A1 TX buffer ready?
            jz      TX1                     ; Jump is TX buffer not ready
                                            ;
            cmp.b   SLV_Data,&UCA1RXBUF     ; Test for correct character RX'd
            jeq     Pass                    ;
Fail        bic.b   #01h,&P1OUT             ; If incorrect, clear LED
            jmp     TX2                     ;
Pass        bis.b   #01h,&P1OUT             ; If correct, light LED
TX2         inc.b   MST_Data                ; Increment master value
            inc.b   SLV_Data                ; Increment expected slave value
            mov.b   MST_Data,&UCA1TXBUF     ; Send next value
                                            ;
            mov.w   #30,R15                 ; Add time between transmissions to
cycleDelay  dec.w   R15                     ; make sure slave can keep up
            jnz     cycleDelay              ;
            reti                            ; Exit ISR
;-------------------------------------------------------------------------------
 ;          Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int17"                ; USCI_A1 Rx Vector
            .short   USCIA1RX_ISR            ;
            .sect   ".reset"                ; RESET Vector
            .short   RESET                  ;
            .end
                        
