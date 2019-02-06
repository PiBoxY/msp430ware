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
;  MSP430F530x Demo - ADC10, DMA Sample A1 64x, AVcc, Repeat Single, DCO
;
;  Description: Sample A1 64 times with AVcc ref. Software writes to ADC10SC
;  to trigger sample burst. In Mainloop MSP430 waits in LPM0 to save power
;  until ADC10 conversion burst complete, DMA_IFG will force exit from LPM0
;  in Mainloop on reti. ADC10 internal oscillator times sample period (16x)
;  and conversion (13x). DMA transfers conversion result in ADC10MEM0 to array
;  ADC_Result. P1.0 set at start of conversion burst, reset on completion.
;
;                MSP430F530x
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;        >---|P6.1/A1          |
;
;  K. Chen
;  Texas Instruments Inc.
;  March 2012
;  Built with CCS Version: 5.2
;*******************************************************************************

 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

ADC_Result  .equ  0x1c00
ADC_Result_sum  .equ  R4
ADC_Result_Average  .equ  R6

            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
;-------------------------------------------------------------------------------
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT

; Configure ADC10 - pulse sample mode; software trigger
SetupADC10  mov.w   #ADC10SHT_2 | ADC10ON | ADC10MSC,&ADC10CTL0
                                            ; 16ADCclks, ADC on
            mov.w   #ADC10SHP | ADC10CONSEQ_2,&ADC10CTL1
                                            ; pulse sample mode, rpt single ch         
            mov.w   #ADC10RES,&ADC10CTL2    ; 10-bit conversion results
            mov.w   #ADC10INCH_1,&ADC10MCTL0; AVCC ref, A1
            
; Configure DMA (ADC10IFG trigger)
            mov.w   #DMA0TSEL_24,&DMACTL0   ; ADC10IFG trigger
            movx.a  #ADC10MEM0,&DMA0SA      ; Source single address
            movx.a  #ADC_Result,&DMA0DA     ; Destination array address
            mov.w   #0x40,&DMA0SZ           ; 64 conversions
            mov.w   #DMADT_4 | DMADSTINCR_3 | DMAEN | DMAIE,&DMA0CTL
                                            ; Rpt, inc dest, word access,
                                            ; enable int after 64 conversions
        
while_loop  bis.w   #ADC10ENC | ADC10SC,&ADC10CTL0
                                            ; Sampling and conversion start
            nop
            bis.w   #LPM0|GIE,SR            ; LPM0, DMA0_ISR will force exit
            nop                             ; For debug only
            
            clr.w   ADC_Result_sum          ; Clear clear accumulate register
            clr.w   R5                      ; Clear the array index
for_loop    rla.w   R5                      ; Index*2 -> indirect offset
                                            ; BREAKPOINT HERE, View ADC_Result 
                                            ; in watch window
            add.w   ADC_Result(R5),ADC_Result_sum ;
            rra.w   R5                      ; Indirect offset/2 -> index
            inc.w   R5                      ; Increment array index
            cmp.w   #0x40,R5                ; Is array index < 64 ?
            jnc     for_loop                ;

            mov.w   ADC_Result_sum,R7       ; Average of 64 conversions results
            rpt     #6                      ;          
            rrux.w  R7                      ; 
            mov.w   R7,ADC_Result_Average   ; 
            
            mov.w   #0x681,R15              ; Delay between conversions
delay       add.w   #0xFFFF,R15             ;
            jc      delay                   ; 
            jmp     while_loop              ; 
            nop                             ; 
                                                            
;-------------------------------------------------------------------------------
DMA_ISR;
;-------------------------------------------------------------------------------
            add.w   &DMAIV,PC               ; Add offset to PC
            reti                            ; Vector 0:  No interrupt
            jmp     DMA0_HND                ; Vector 2:  DMA0IFG
            reti                            ; Vector 4:  DMA1IFG
            reti                            ; Vector 6:  DMA2IFG
            reti                            ; Vector 8:  Reserved
            reti                            ; Vector 10: Reserved               
            reti                            ; Vector 12: Reserved
            reti                            ; Vector 14: Reserved
            reti                            ; Vector 16: Reserved
                        
DMA0_HND
; 64 conversions complete             
            bic.w   #ADC10ENC,&ADC10CTL0    ; Clear interrupt flag 
            bic.w   #LPM0,0(SP)             ; exit LPM
            reti
            
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   DMA_VECTOR              ; DMA Vector
            .short  DMA_ISR                 ;
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
