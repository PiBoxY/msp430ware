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
;   MSP-FET430P430 Demo - DMA0, ADC12 A10 Block Xfer to Flash, Timer_B, SMCLK
;
;   Description: A 0x20 word block of data is sampled and recorded into Flash
;   from ADC12 channel 10, the integrated temperature sensor, using the
;   Record function. Timer_B CCR1 begins the ADC12 sample period, CCR0 the
;   hold and conversion start. Timer_B operates in the up mode with CCR0
;   defining the sample period. DMA0 will automatically transfer each
;   ADC12 conversion code to memory when complete. At the end of the recording
;   block, DMA0 will issue an interrupt exiting the function.
;   In this example the ADC12 sample period is 120 SMCLK cycles, and Flash
;   programming requires 105 cycles.
;   In the example the Flash block used to record ADC12 data begins at 0x1080.
;   P5.1 is toggled during DMA transfer only for demonstration purposes.
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   ;* An external watch crystal between XIN & XOUT is required for ACLK *//	
;
;   IMPORTANT - Set breakpoint to avoid stressing Flash
;
;               MSP430FG439
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
;         --|RST          XOUT|-
;           |                 |
;           |A10 (Temp)   P5.1|-->LED
;
;   G. Morton / M. Mitchell
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
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
                                            ;
SetupP5     bis.b   #02h,&P5DIR             ; Set P5.1 as output
                                            ;
SetupFMC    mov.w   #FWKEY+FSSEL_2+FN1,&FCTL2
                                            ; Flash Cntrl clk src = SMCLK/(2+1)
                                            ;
Mainloop    call    #Record                 ; DMA temp to Flash
            nop                             ; Set BREAKPOINT HERE
            jmp     Mainloop                ; Again
                                            ;
;------------------------------------------------------------------------------
Record      ; Record temperature in Flash
;------------------------------------------------------------------------------
EraseFlash  mov.w   #FWKEY+ERASE,&FCTL1     ; Set erase bit
            mov.w   #FWKEY,&FCTL3           ; Clear lock bit
            mov.w   #0,&0x1080              ; Dummy write to start Flash erase
                                            ;
SetupADC12  mov.b   #SREF_1+INCH_10,&ADC12MCTL0
                                            ; Vr+=Vref+/Vr-=AVss,
                                            ; select temp sensor as input
            mov.w   #SHS_3+CONSEQ_2,&ADC12CTL1
                                            ; Timer_B.OUT1,
                                            ; repeat-single-chan mode
            mov.w   #REF2_5V+REFON+ADC12ON+ENC,&ADC12CTL0
                                            ; 2.5V ref on, ADC12 on,
                                            ; enable conversions
            call    #VrefDly                ; Allow time for 2.5V ref to settle
            mov.w   #0,&ADC12IFG            ; Clear interrupt flags
                                            ;
SetupDMA0   mov.w   #ADC12MEM0,&DMA0SA      ; DMA source address
            mov.w   #0x1080,&DMA0DA         ; DMA destination address
            mov.w   #0x20,&DMA0SZ           ; DMA transfer size
            mov.w   #DMA0TSEL_6,&DMACTL0    ; ADC12IFG bit triggers DMA
            mov.w   #DMAONFETCH,&DMACTL1    ; Required for proper Flash write
            mov.w   #DMADSTINCR_3+DMAIE+DMAEN,&DMA0CTL
                                            ; Increment dest addr,
                                            ; enable interrupt,
                                            ; word transfer (default),
                                            ; enable DMA
                                            ;
SetupTB     mov.w   #120-1,&TBCCR0          ; Set sampling period
            mov.w   #120-30,&TBCCR1         ; Set end of sample & hold
            mov.w   #OUTMOD_7,&TBCCTL1      ; Reset/set mode
                                            ;
BeginRec    bis.b   #02h,&P5OUT             ; Set P5.1=1 (LED on)
            mov.w   #FWKEY+WRT,&FCTL1       ; Enable Flash write
            mov.w   #TBSSEL_2+MC_1+TBCLR,&TBCTL
                                            ; SMCLK, up mode, clear count
            bis.w   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
                                            ;
LockFlash   mov.w   #FWKEY,&FCTL1           ; Disable Flash write
            mov.w   #FWKEY+LOCK,&FCTL3      ; Lock Flash
                                            ;
PwrDown     bic.w   #CONSEQ_2,&ADC12CTL1    ; Stop conversion immediately
            bic.w   #ENC,&ADC12CTL0         ; Disable ADC12 conversion
            mov.w   #0,&ADC12CTL0           ; Switch off ADC12 & ref voltage
            mov.w   #0,&TBCTL               ; Disable Timer_B
            bic.b   #02h,&P5OUT             ; Set P5.1=0 (LED off)
            ret                             ; Return from subroutine
                                            ;
;------------------------------------------------------------------------------
VrefDly     ; ADC12 Voltage Reference Delay
;------------------------------------------------------------------------------
            push.w  #0FFFh                  ; Push delay on stack
D1          dec.w   0(SP)                   ; Decrement value on stack
            jnz     D1                      ; Is value = 0?, no jump
            incd.w  SP                      ; Clean up stack
            ret                             ; Return from subroutine
                                            ;
;------------------------------------------------------------------------------
DMA_ISR     ; DMA Interrupt Service Routine
;------------------------------------------------------------------------------
            bic.w   #DMAIFG,&DMA0CTL        ; Must clear DMA interrupt flag
            bic.w   #CPUOFF,0(SP)           ; Exit LPM0
            reti                            ; Return from interrupt
                                            ;
;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .sect   ".int03"                ; DMA Vector
            .short  DMA_ISR                 ;
            .end