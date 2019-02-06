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
;   MSP-FET430P430 Demo - USART0, 19200 UART Ultra-low Pwr Echo ISR, DCO SMCLK
;
;   Description: Echo a received character, USART0 RX ISR at high-speed used
;   with ultra-low power techniques. Normal operation is LPM3. On valid RX
;   character, character echoed back. Use start-bit edge detect - URXSE - to
;   automatically (re)enable DCO and trigger ISR. ISR must make sure DCO
;   clock source remains enabled for the UART to receive full character.
;   Software needs to make sure a character has been completely TX'ed, or RX'ed
;   before entering LPM3, which disables DCO required for the USART baud rate
;   generator. In the example, TX'ing is checked using the TXEPT bit directly.
;   RX'ing is checked using the SSEL0 clock select bit as a flag. This is
;   possible because UCLK0 = SMCLK when either both SSEL1 and SSEL0 or just
;   SSEL1 = 1. In the example, when SSEL1 = SSEL0 = 1 there is no RX'ing, and
;   LPM3 is allowed. When SSEL 1 = 1 and SSEL0 = 0, SMCLK is selected, but
;   RX'ing is active and the DCO is required, thus LPM3 is not allowed.
;   Baud rate divider with 1048576Hz= 1048576Hz/19200 = ~54.61 (036h|6Bh)
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   ;* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;                MSP430FG439
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |       P2.4/UTXD0|----------->
;            |                 | 19200 - 8N1
;            |       P2.5/URXD0|<-----------
;
;   M. Buccini / M. Mitchell
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                  ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
Delay       push.w  #0                      ; Delay to allow FLL to .set
D1          dec.w   0(SP)                   ;
            jnz     D1                      ;
            incd.w  SP                      ;
SetupP3     bis.b   #030h,&P2SEL            ; P2.4,5 = USART0 TXD/RXD
SetupUART0  bis.b   #UTXE0+URXE0,&ME1       ; Enable USART0 TXD/RXD
            bis.b   #CHAR,&UCTL0            ; 8-bit characters
            bis.b   #SSEL1+SSEL0+URXSE,&UTCTL0 ; UCLK = SMCLK, start edge
            mov.b   #036h,&UBR00            ; 1MHz 19200
            mov.b   #000h,&UBR10            ; 1MHz 19200
            mov.b   #06Bh,&UMCTL0           ; Modulation
            bic.b   #SWRST,&UCTL0           ; **Initialize USART state machine**
            bis.b   #URXIE0,&IE1            ; Enable USART0 RX interrupt
                                            ;
Mainloop    bit.b   #TXEPT,&UTCTL0          ; Confirm no TXing before --> LPM3
            jeq     Mainloop                ;
            dint                            ; Disable interrupts
            nop                             ; One clock for dint to latch
RX_Test     bit.b   #SSEL0,&UTCTL0          ; SSEL0 = 0 as flag for RX'ing
            jnc     RX_Active               ;
            bis.b   #LPM3+GIE,SR            ; Enter LPM3, interrupts enabled
            jmp     Mainloop                ;
RX_Active   bis.b   #LPM0+GIE,SR            ; Enter LPM0, interrupts enabled
            jmp     Mainloop                ;
                                            ;
;------------------------------------------------------------------------------
USART0RX_ISR;  Echo back RXed character, confirm TX buffer is ready first
;------------------------------------------------------------------------------
            bit.b   #URXIFG0,&IFG1          ; Interrupt from start edge?
            jnc     RX_Edge                 ; Jump--> interrupt from edge
TX1         bit.b   #UTXIFG0,&IFG1          ; USART0 TX buffer ready?
            jz      TX1                     ; Jump is TX buffer not ready
            mov.b   &RXBUF0,&TXBUF0         ; TX -> RXed character
            bis.b   #SSEL0,&UTCTL0          ; SSEL0 = 1, RX complete
            bic.b   #LPM3,0(SP)             ; Exit LPM3 on reti
            reti                            ;
                                            ;
RX_Edge     bic.b   #URXSE,&UTCTL0          ; Clear start edge signal
            bis.b   #URXSE,&UTCTL0          ; Enable start edge detect
            bic.b   #SSEL0,&UTCTL0          ; SSEL0 = 0 as flag for start
            bic.b   #SCG1+SCG0,0(SP)        ; Enter LPM0 on reti
            reti                            ;
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .sect   ".int09"                ; USART0 Receive Vector
            .short  USART0RX_ISR            ;
            .end
