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
;  MSP-FET430P140 Demo - DMA0, ADC12 A10 Block Xfer to Flash, TBCCR1, DCO
;
;  Description: A 0x20 word block of data is sampled and recorded into Flash
;  from the ADC12 channel 10 the integrated temperature sensor using the
;  Record() function. Timer_B CCR1 begins the ADC12 sample period, CCR0 the hold
;  and conversion start. Timer_B operates in the up mode with CCR0 defining the
;  sample period. DMA0 will automatically transfer each ADC12 conversion code
;  to memory when complete. At the .end of the recording block, DMA0 will issue
;  an interrupt existing the function.
;  In this example the ADC12 sample period is 100 SMCLK cycle, and Flash
;  programming requires 70 cycles.
;  In the example the Flash block use to record the ADC12 data begins at 0x1080.
;  P1.0 is toggled durring DMA transfer only for demonstration purposes.
;  ACLK = n/a, MCLK = SMCLK = default DCO ~ 800k
;  //* set Breakpoint on NOP in the Mainloop to avoid Stressing Flash *//	
;  //* MSP430F169 Device Required *//
;
;               MSP430F169
;            -----------------
;        /|\|              XIN|-
;         | |                 |
;         --|RST          XOUT|-
;           |                 |
;           |A10          P1.0|-->LED
;
;
;   H. Grewal / L. Westlund
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
            mov     #FWKEY+FSSEL_2+FN0,&FCTL2 ; Clk src = SMCLK / 2 (~400KHz)
            bis.b   #01h,&P1DIR             ; .set P1.0 to output direction
Mainloop    call    #record
                    nop                     ; Needed only for debugger
            jmp     Mainloop
record      mov.w   #01080h,R5              ;
            mov.w   #FWKEY+ERASE,&FCTL1     ; Erase bit = 1
Erase_SegA  mov.w   #FWKEY,&FCTL3           ; Lock = 0
            mov.w   #0,&01080h              ; Dummy write to erase Flash segment
            mov.b   #SREF_1+INCH_10,&ADC12MCTL0 ; Channel A10, Vref+
            mov     #0h,&ADC12IFG
            mov     #SHS_3+CONSEQ_2,&ADC12CTL1 ; S&H TB.OUT1, rep. single chan
            mov.w   #REF2_5V+REFON+ADC12ON+ENC,&ADC12CTL0 ; VRef ADC12 on, enabled
            mov     #03600h,R15             ; Delay for needed ref start-up.
L$1         dec     R15                     ; See datasheet for details.
            jnz     L$1                     ;
            mov     #100,&TBCCR0            ; Init TBCCR0 w/ sample prd
            mov     #70,&TBCCR1             ; Trigger for ADC12 SC
            mov     #OUTMOD_7,&TBCCTL1      ; Reset OUT1 on EQU1, .set on EQU0
SetupDMAx   mov.w   #ADC12MEM0, &DMA0SA     ; Src address = ADC12 module
            mov.w   #1080h,&DMA0DA          ; Dst address = RAM memory
            mov.w   #020h,&DMA0SZ           ; Size in words
            mov.w   #DMA0TSEL_6,&DMACTL0    ; ADC12IFGx triggers DMA0
            mov.w   #DMAONFETCH,&DMACTL1    ; Required for proper Flash Write
            mov.w   #DMADSTINCR_3+DMAIE+DMAEN,&DMA0CTL; Config
            bis.b   #01h,&P1OUT             ; Start recording and enter LPM0
            mov     #TBSSEL_2+MC_1+TBCLR,&TBCTL ;SMCLK, clear TBR, up mode
            mov.w   #FWKEY+WRT,&FCTL1       ; Write bit = 1
            bis.b   #CPUOFF+GIE,SR          ; Enter LPM0, enable interrupts
            mov.w   #FWKEY,&FCTL1           ; Write bit = 0
            mov.w   #FWKEY+LOCK,&FCTL3      ; Lock = 1
            bic     #CONSEQ_2,&ADC12CTL1    ; Stop conversion immediately
            bic     #ENC,&ADC12CTL0         ; Disable ADC12 conversion
            mov     #0h, &ADC12CTL0         ; Switch off ADC12 & ref voltage
            mov     #0h, &TBCTL             ; Disable Timer_B
            bic.b   #01h,&P1OUT             ; Clear P1.0 (LED Off)
            ret
;------------------------------------------------------------------------------
DMA_ISR;    Common ISR for DMA/DAC12
;------------------------------------------------------------------------------
            bic     #DMAIFG,&DMA0CTL        ; Clear DMA0 interrupt flag
            bic     #LPM0,0(SP)             ; Exit LPMx, interrupts enabled
            reti                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
             .sect   ".reset"               ; POR, ext. Reset, Watchdog, Flash
             .short  RESET                  ;
             .sect   ".int00"               ; DMA Vector
             .short  DMA_ISR                ;
             .end

