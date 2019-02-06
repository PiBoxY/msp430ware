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
;   MSP-FET430P140 Demo - DMA2, Rpt'd Blk to DAC1, 8-Bit Sine, TBCCR2, DCO
;
;   Description: DMA2 is used to transfer a sine look-up table byte-by-byte
;   as a repeating block to DAC12_1. The effect is a sine wave output. Timer_B
;   operates in upmode with CCR2 loading DAC12_1 on the rising edge, CCR2_CCIFG
;   triggering the next DMA2 transfer. Use external reference on VeRef+.
;   ACLK = n/a, MCLK = SMCLK = TBCLK = default DCO ~ 800kHz
;   ;* MSP430F169 Device Required *//
;
;                MSP430F169
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;   Ext Ref->|VeRef+  DAC1/P6.7|--> ~ 1kHz sine wave output
;
;
;   M. Buccini / L. Westlund
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
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
;SetupADC12  mov.w   #REF2_5V+REFON,&ADC12CTL0       ; Internal 2.5V ref
SetupDMA2   mov.w   #Sin_tab,&DMA2SA        ; Source block address
            mov.w   #DAC12_1DAT,&DMA2DA     ; Destination single address
            mov.w   #020h,&DMA2SZ           ; Block size
            mov.w   #DMA2TSEL_2,&DMACTL0    ; DAC12IFG trigger
            mov.w   #DMASBDB+DMADT_4+DMASRCINCR_3+DMAEN,&DMA2CTL; Rpt, inc src, word-word
SetupDAC12  mov.w   #DAC12SREF_2+DAC12RES+DAC12LSEL_3+DAC12IR+DAC12AMP_5+DAC12ENC,&DAC12_1CTL
                                            ;
SetupC1     mov.w   #OUTMOD_7,&TBCCTL2      ; Reset/set
            mov.w   #01,&TBCCR2             ; PWM Duty Cycle	
SetupC0     mov.w   #025-1,&TBCCR0          ; Clock period of CCR0
SetupTB     mov.w   #TBSSEL_2+MC_1,&TBCTL   ; SMCLK, up mode
                                            ;
Mainloop    bis.b   #CPUOFF,SR              ; Enter LPM0
            nop                             ; Needed only for debugger
                                            ;
;------------------------------------------------------------------------------
; 8-bit Sine Lookup table with 32 steps
;------------------------------------------------------------------------------
Sin_tab     .byte  128, 152, 176, 198, 218, 234, 245, 253
            .byte  255, 253, 245, 234, 218, 198, 176, 152
            .byte  128, 103,  79,  57,  37,  21,  10,   2
            .byte  0,   2,  10,  21,  37,  57,  79, 103

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
             .sect   ".reset"               ; POR, ext. Reset, Watchdog, Flash
             .short  RESET                  ;
             .end

