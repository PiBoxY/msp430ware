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
;   MSP-FET430P120 Demo - USART0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
;
;   Description: Echo a received character, USART0 RX ISR at high-speed used
;   with ultra-low power techniques. Normal operation in LPM3, Set_DCO
;   subroutine needs to be called to configure DCO that is used for UART baud
;   generation. On a valid RX character, character echoed  back. Use start-bit
;   edge detect - URXSE - to automatically (re)enable DCO and trigger ISR. ISR
;   must make sure DCO clock source remains enabled for the UART to receive
;   full character. Software also needs to make sure a character has been
;   completely TX'ed before entering LPM3, which disables DCO.
;   Baud rate divider with 2MHz = 2MHz/9600 ~208 (00D0h)
;   ACLK = LFXT1/8 = 32768/8,  MCLK = SMCLK = UCLK0 = DCOCLK ~2MHz
;   //* An external 32kHz watch crystal on XIN XOUT is required for ACLK *//	
;
Delta       .equ    488                   ; Delta = (target DCO)/(4096) ~ 2MHz
;
;                MSP430F123(2)
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32768Hz
;          --|RST          XOUT|-
;            |                 |
;            |             P3.4|------------>
;            |                 |  9600 - 8N1
;            |             P3.5|<------------
;
;   M. Buccini / M. Raju
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0300h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupP3     bis.b   #030h,&P3SEL            ; P3.4,5 = USART0 TXD/RXD
SetupBC     bis.b   #DIVA1+DIVA0,&BCSCTL1   ; ACLK = LFXT1CLK/8
            call    #Set_DCO                ; Calibrate DCOCLK
SetupUART0  bis.b   #UTXE0+URXE0,&ME2       ; Enable USART0 TXD/RXD
            bis.b   #CHAR,&UCTL0            ; 8-bit char
            bis.b   #SSEL1+SSEL0+URXSE,&UTCTL0 ; UCLK = SMCLK, start edge
            mov.b   #0D0h,&UBR00            ; 2MHz 9600
            mov.b   #000h,&UBR10            ;
            mov.b   #000h,&UMCTL0           ; no modulation
            bic.b   #SWRST,&UCTL0           ; **Initalize USART state machine**
            bis.b   #URXIE0,&IE2            ; Enable USART0 RX interrupt
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
;-----------------------------------------------------------------------------
Set_DCO;    Subroutine: Sets DCO to selected frequency based on Delta.
;           R14 and R15 are used, ACLK = 32768/8 Timer_A clocked by DCOCLK
;-----------------------------------------------------------------------------
            clr.w   R15                     ;							
Setup_CC2   mov.w   #CM_1+CCIS_1+CAP,&CCTL2   ; CAP, ACLK
Setup_TA    mov.w   #TASSEL_2+MC_2+TACLR,&TACTL   ; SMCLK, ContMode, clr
Test_DCO    bit.w   #CCIFG,&CCTL2           ; Test capture flag
            jz      Test_DCO                ;
            bic.w   #CCIFG,&CCTL2           ; Clear capture flag
                                            ;
AdjDCO      mov.w   &CCR2,R14               ; R14 = captured SMCLK
            sub.w   R15,R14                 ; R14 = capture difference
            mov.w   &CCR2,R15               ; R15 = captured SMCLK
            cmp.w   #Delta,R14              ; Delta = SMCLK/(32768/8)
            jlo     IncDCO                  ;
            jeq     DoneDCO                 ;
DecDCO      dec.b   &DCOCTL                 ; Slow DCO with DCO and MOD
            jc      Test_DCO                ; Slower?
            cmp.b   #XT2OFF+DIVA_3,&BCSCTL1 ; Can RSEL.x be decremented?
            jz      DoneDCO                 ; jmp>DCO at slowest setting
            dec.b   &BCSCTL1                ; Decrement RSEL.x
            jmp     Test_DCO                ;
IncDCO      inc.b   &DCOCTL                 ; Speed DCO with DCO and MOD
            jnc     Test_DCO                ; Faster?
            cmp.b   #XT2OFF+DIVA_3+07h,&BCSCTL1 ; Can RSEL.x be increased?
            jz      DoneDCO                 ; jmp> DCO at fastest setting
            inc.b   &BCSCTL1                ; Increment RSEL.x
            jmp     Test_DCO                ;
DoneDCO     clr.w   &CCTL2                  ; Stop CCR2
            clr.w   &TACTL                  ; Stop timer_A
            ret                             ; Return from subroutine
                                            ;
;------------------------------------------------------------------------------
USART0RX_ISR;  Echo back RXed character, confirm TX buffer is ready first
;------------------------------------------------------------------------------
            bit.b   #URXIFG0,&IFG2          ; Interrupt from start edge?
            jnc     RX_Edge                 ; Jump--> interrupt from edge
TX1         bit.b   #UTXIFG0,&IFG2          ; USART0 TX buffer ready?
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
            .sect   ".reset"                ;
            .short  RESET                   ; POR, ext. Reset, Watchdog
            .sect   ".int07"                ;
            .short  USART0RX_ISR            ; USART0 receive
            .end
