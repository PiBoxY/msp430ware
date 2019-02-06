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
;   MSP430x11x1 Demo - Software Output 6-Bit R2R DAC
;
;   Description: Implement a 6-bit DAC using 6 P2.x outputs and an R2R
;   ladder.  A sine wave is generated using a 6-bit, 32-step sine look-up
;   table Sin_Tab.  R5 is used as a pointer into Sin_Tab.  Any MSP430
;   outputs can be used for the DAC with their bit-resolutions and
;   frequencies possible.  ACLK as an input to CCR2 is used to trigger an
;   interrupt at exacty 32768Hz.  The ISR updates the DAC using
;   Sin_Tab.  The frequency of the sine wave in this example is ISR
;   frequency/steps = 32768/32 = 1024hz.
;   ACLK = LFXT1 = 32768, MCLK = SMCLK = default DCO ~800k
;   ;* An external watch crystal on XIN XOUT is required for ACLK *//	
;
;
;                MSP430F1121
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |             P2.5|--20k--+---> 1024hz sine wave
;            |                 |      10k
;            |             P2.4|--20k--+
;            |                 |      10k
;            |             P2.3|--20k--+
;            |                 |      10k
;            |             P2.2|--20k--+
;            |                 |      10k
;            |             P2.1|--20k--+
;            |                 |      10k
;            |             P2.0|--20k--+
;            |                 |      20k
;                                      |
;                                      V
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
RESET       mov.w   #0300h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupBC     bis.b   #RSEL2+RSEL0,&BCSCTL1   ; RSEL=5
SetupP2     mov.b   #03Fh,&P2DIR            ; P2.0 - 5 output direction
            clr.w   R5                      ; Clear pointer
Setup_CC2   mov.w   #CCIS0+CM0+CAP+CCIE,&CCTL2 ; CCR2,CAP on CCxB,ACLK,int				
SetupTA     mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK, contmode
            eint                            ; Enable interrupts
                                            ;
Mainloop    jmp     Mainloop                ; Do Nothing
                                            ;
;-------------------------------------------------------------------------------
TAX_ISR;    Common ISR for CCR1-4 and overflow
;-------------------------------------------------------------------------------
            add.w   &TAIV,PC                ; Add TA interrupt offset to PC
            reti                            ; CCR0 - no source
            reti                            ; CCR1
            jmp     CCR2_ISR                ; CCR2
;            reti                            ; CCR3
;            reti                            ; CCR4
;TA_over     reti                            ; Timer_A overflow
                                            ;
CCR2_ISR    mov.b   Sin_tab(R5),&P2OUT      ; Move sine value to P2
            inc.b   R5                      ; Inc pointer
            and.b   #01Fh,R5                ; R5 = 0 - 31 only (32 bytes)
            reti                            ;
;-------------------------------------------------------------------------------
Sin_tab;    Sine 6-bit Lookup table with 32 steps
;-------------------------------------------------------------------------------
            .byte  32
            .byte  38
            .byte  44
            .byte  49
            .byte  54
            .byte  58
            .byte  61
            .byte  62
            .byte  63
            .byte  62
            .byte  61
            .byte  58
            .byte  54
            .byte  49
            .byte  44
            .byte  38
            .byte  32
            .byte  25
            .byte  19
            .byte  14
            .byte  9
            .byte  5
            .byte  2
            .byte  1
            .byte  0
            .byte  1
            .byte  2
            .byte  5
            .byte  9
            .byte  14
            .byte  19
            .byte  25
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int08"                ; Timer_AX Vector
            .short  TAX_ISR                 ;
            .end
