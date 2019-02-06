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
;   MSP430xG461x Demo - DMA0/1, Rpt'd Blk to DAC12_0/1, Sin/Cos, TACCR1, XT2
;
;   Description: DMA0 and DMA1 are used to transfer a sine and cos look-up
;   table word-by-word as a repeating block to DAC12_0 and DAC12_1. The effect
;   is sine and cos wave outputs. Timer_A operates in upmode with TACCR1
;   loading DAC12_0 amd DAC12_1 on rising edge and DAC12_OIFG triggering next
;   DMA transfers. DAC12_0 and DAC12_1 are grouped for jitter-free operation.
;   ACLK= n/a, MCLK = SMCLK = TACLK = XT2 = 8MHz
;   //* An external 8MHz XTAL on X2IN X2OUT is required for XT2CLK *//
;
;                MSP430xG461x
;             -----------------
;         /|\|            XT2IN|-
;          | |                 | 8MHz
;          --|RST        XT2OUT|-
;            |                 |
;            |        DAC0/P6.6|--> ~ 10kHz sine wave
;            |        DAC1/P6.7|--> ~ 10kHz cos wave
;
;  JL Bile
;  Texas Instruments Inc.
;  June 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.text	;Program Start
;-----------------------------------------------------------------------------
RESET       mov.w   #850,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupHF     bic.b   #XT2OFF,&FLL_CTL1       ; Clear bit = high freq xtal on
ClearFlag   bic.b   #OFIFG,&IFG1            ; Clear osc fault flag
            mov.w   #0F000h,R15             ; Move delay time to register 15
HF_Wait     dec.w   R15                     ; Delay for xtal to start, FLL lock
            jnz     HF_Wait                 ; Loop if delay not finished
            bit.b   #OFIFG,&IFG1            ; Test osc fault flag
            jnz     ClearFlag               ; If not loop again
                                            ;
            bis.b   #SELM1+SELS,&FLL_CTL1   ; MCLK = XT2

SetupADC12  mov.w   #REF2_5V+REFON,&ADC12CTL0 ; Internal 2.5V ref
            mov.w   #13600,&TACCR0          ; Delay to allow Ref to settle
            bis.w   #CCIE,&TACCTL0          ; Compare-mode interrupt.
            mov.w   #TACLR+MC_1+TASSEL_2,&TACTL; up mode, SMCLK
            bis.w   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
            bic.w   #CCIE,&TACCTL0          ; Disable timer interrupt
            dint                            ; Disable Interrupts

SetupDMAx   mov.w   #DMA0TSEL_5+DMA1TSEL_5,&DMACTL0 ; DAC12IFG triggers
            movx.a  #Sin_tab,&DMA0SA        ; Source block address
            movx.a  #DAC12_0DAT,&DMA0DA     ; Destination single address
            mov.w   #020h,&DMA0SZ           ; Block size
            mov.w   #DMADT_4+DMASRCINCR_3+DMAEN,&DMA0CTL; Rpt, inc src, word-word
            movx.a  #Cos_tab,&DMA1SA        ; Source block address
            movx.a  #DAC12_1DAT,&DMA1DA     ; Destination single address
            mov.w   #020h,&DMA1SZ           ; Block size
            mov.w   #DMADT_4+DMASRCINCR_3+DMAEN,&DMA1CTL; Rpt, inc src, word-word
SetupDAC12x mov.w   #DAC12LSEL_2+DAC12IR+DAC12AMP_5+DAC12IFG+DAC12ENC+DAC12GRP,&DAC12_0CTL
            mov.w   #DAC12LSEL_2+DAC12IR+DAC12AMP_5+DAC12IFG+DAC12ENC,&DAC12_1CTL

SetupC1     mov.w   #OUTMOD_3,&TACCTL1      ; TACCR1 set/reset
            mov.w   #01,&TACCR1             ; TACCR1 PWM Duty Cycle
SetupC0     mov.w   #025-1,&TACCR0          ; Clock period of TACCR0
SetupTA     mov.w   #TASSEL_2+MC_1,&TACTL   ; SMCLK, contmode

Mainloop    bis.b   #CPUOFF,SR              ; Enter LPM0
            jmp     Mainloop
;-------------------------------------------------------------------------------
TIMERA0_ISR;
;-------------------------------------------------------------------------------
            clr.w   &TACTL                  ; Clear Timer_A control registers
            bic.w   #LPM0,0(SP)             ; Exit LPMx, interrupts enabled
            reti                            ;
;-------------------------------------------------------------------------------
; 12-bit Sine Lookup table with 32 steps
;-------------------------------------------------------------------------------
Sin_tab     .word 2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056
            .word 4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447
            .word 2048, 1648, 1264,  910,  600,  345,  156,   39
            .word    0,   39,  156,  345,  600,  910, 1264, 1648
;-------------------------------------------------------------------------------
; 12-bit Cosine Lookup table with 32 steps
;-------------------------------------------------------------------------------
Cos_tab     .word 1648, 1264,  910,  600,  345,  156,   39,    0
            .word   39,   56,  345,  600,  910, 1264, 1648, 2048
            .word 2447, 2831, 3185, 3495, 3750, 3939, 4056, 4095
            .word 4056, 3939, 3750, 3495, 3185, 2831, 2447, 2048
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect    ".int22"          ; Timer_A0 Vector
            .short   TIMERA0_ISR
            .sect	 ".reset"            ; POR, ext. Reset, Watchdog
            .short   RESET
            .end
