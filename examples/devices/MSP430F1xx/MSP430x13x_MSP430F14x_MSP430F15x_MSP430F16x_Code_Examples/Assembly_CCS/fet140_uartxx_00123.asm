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
;   MSP-FET430P140 Demo - USART0/1, UART 19200-115200 Router, XT2 HF XTAL SMCLK
;
;   Description: This program will route characters between USART0 @19200
;   to USART1 @115200.  Receive interrupt service used.  USART1 running
;   at a higher baud rate could overrun USART0 running at a slower baud.
;   ACLK = MCLK = SMCLK = UCLK1 = XT2 = 8MHz
;   Baud rate divider with 8MHz XTAL @19200 = 8MHz/19200 = 417 (01A0h)
;   Baud rate divider with 8MHz XTAL @115200 = 8MHz/115200 = 69.44(0045 2Ch)
;   //* An external 8MHz XTAL on X2IN X2OUT is required for ACLK *//	
;   //* Min Vcc required varies with MCLK frequency - refer to datasheet *//	
;
;                      MSP430F149
;                   -----------------
;               /|\|             X2IN|-
;                | |                 | 8MHz
;                --|RST         X2OUT|-
;                  |                 |
;   +------------->|P3.7 ------->P3.4|------------>
;   | 8N1 - 115200 |                 | 19200 - 8N1
;   +--------------|P3.6<------- P3.5|<------------
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
;------------------------------------------------------------------------------
            .text                           ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
            call    #Init_Sys               ;
                                            ;
Mainloop    jmp     Mainloop                ; Do nothing
                                            ;
;------------------------------------------------------------------------------
Init_Sys;   Initalize MSP430 system
;------------------------------------------------------------------------------
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP3     bis.b   #0F0h,&P3SEL            ; Select USART1 TXD/RXD
SetupBC     bic.b   #XT2OFF,&BCSCTL1        ; XT2on
SetupOsc    bic.b   #OFIFG,&IFG1            ; Clear OSC fault flag
            mov.w   #0FFh,R15               ; R15 = Delay
SetupOsc1   dec.w   R15                     ; Additional delay to ensure start
            jnz     SetupOsc1               ;
            bit.b   #OFIFG,&IFG1            ; OSC fault flag set?
            jnz     SetupOsc                ; OSC Fault, clear flag again
            bis.b   #SELM_2+SELS,BCSCTL2    ; MCLK = SMCLK = XT2
                                            ;
SetupUART0  bis.b   #UTXE0+URXE0,&ME1       ; Enable USART0 TXD/RXD
            bis.b   #CHAR,&UCTL0            ; 8-bit characters
            mov.b   #SSEL1,&UTCTL0          ; UCLK = SMCLK
            mov.b   #0A0h,&UBR00            ; 19200 @ 8MHz
            mov.b   #001h,&UBR10            ;
            clr.b   &UMCTL0                 ; No modulation
            bic.b   #SWRST,&UCTL0           ; **Initalize USART state machine**
            bis.b   #URXIE0,&IE1            ; Enable USART0 RX interrupt
                                            ;
SetupUART1  bis.b   #UTXE1+URXE1,&ME2       ; Enable USART1 TXD/RXD
            bis.b   #CHAR,&UCTL1            ; 8-bit characters
            mov.b   #SSEL1,&UTCTL1          ; MCLK, SMCLK?
            mov.b   #045h,&UBR01            ; 115200 @ 8MHz
            mov.b   #000h,&UBR11            ;
            mov.b   #02Ch,&UMCTL1           ; Modulation
            bic.b   #SWRST,&UCTL1           ; **Initialize USART state machine**
            bis.b   #URXIE1,&IE2            ; Enable USART1 RX interrupt
                                            ;
            eint                            ; General enable interrupts
            ret                             ;
                                            ;
;------------------------------------------------------------------------------
USART0_ISR; Echo back RXed character, confirm TX buffer is ready first
;------------------------------------------------------------------------------
TX1         bit.b   #UTXIFG0,&IFG1          ; USART0 TX buffer ready?
            jz      TX1                     ; Jump is TX buffer not ready
            mov.b   &RXBUF0,&TXBUF1         ; TX -> RXed character
            reti                            ;
                                            ;
;------------------------------------------------------------------------------
USART1_ISR;  Echo back RXed character, confirm TX buffer is ready first
;------------------------------------------------------------------------------
TX2         bit.b   #UTXIFG1,&IFG2          ; USART1 TX buffer ready?
            jz      TX2                     ; Jump is TX buffer not ready
            mov.b   &RXBUF1,&TXBUF0         ; TX -> RXed character
            reti                            ;
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ;
            .short  RESET                   ; POR, ext. Reset, Watchdog
            .sect   ".int09"                ;
            .short  USART0_ISR              ; USART0 Receive
            .sect   ".int03"                ;
            .short  USART1_ISR              ; USART1 Receive
            .end
