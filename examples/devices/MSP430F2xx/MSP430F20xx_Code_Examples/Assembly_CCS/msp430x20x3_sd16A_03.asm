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
;  MSP430F20x3 Demo - SD16A Sequence of conversions
;  The SD16A takes a sample of a single sequence of channels: A0, A1, then A2. 
;  Sampling begins with ch A0.  The 4th conversion result of each channel is 
;  stored in memory. 
;
;
;                MSP430F20x3
;             ------------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;    Vin+ -->|A0+ P1.0         |
;            |A01- = VSS       |
;            |                 |
;    Vin+ -->|A1+ P1.2         |
;            |A1- = VSS        |
;            |                 |
;    Vin+ -->|A2+ P1.4         |
;            |A2- = VSS        |
;            |                 |
;            |            VREF |---+
;            |                 |   |
;            |                 |  -+- 100nF
;            |                 |  -+-
;            |                 |   |
;            |            AVss |---+
;            |                 |
;
;
;  R. B. Elliott  / H. Grewal
;  Texas Instruments
;  April 2007
;  Built with Code Composer Essentials V2.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .text   0F800h                ; Program Reset
;-------------------------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; Initialize stackpointer

StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #DIVS_3, &BCSCTL2       ; SMCLK/8

SetupSD16   mov.w   #SD16REFON+SD16SSEL_1,&SD16CTL  ; 1.2V ref, SMCLK
            mov.b   #SD16INCH_0+SD16INTDLY_0,&SD16INCTL0 ; Set channel A0+/-
            mov.w   #SD16UNI+SD16IE+SD16SNGL,&SD16CCTL0    ; 256OSR, unipolar,
                                            ; enable interrupt, 
                                            ; single conversion
                                            
            mov.b   #SD16AE0,&SD16AE        ; P1.0 A0+, A0- = VSS

            mov.w  #03600h, R4              ; Delay for 1.2V ref startup
Delay       dec.w  R4;
            jne    Delay; 
           
            mov.w  #0h, R5                  ; R5 = ch_count
           
                                            ;				
Mainloop    bis.w   #SD16SC,&SD16CCTL0      ; Start conversion
            bis.w   #CPUOFF+GIE,SR          ; CPU off, enable interrupts
            nop                             ; Required only for debugger
            jmp     Mainloop                ; jump back to mainloop
            
                                            
;-------------------------------------------------------------------------------
SD16_ISR; 
;-------------------------------------------------------------------------------
              
            cmp.w #01h, R5;
            jlo   Case0                     ; current channel is A0
            jeq   Case1                     ; current channel is A1
            jmp   Case2                     ; current channel is A2

Case0       mov.w &SD16MEM0, R7             ; R7 = A0 conversion results
            inc.w R5                        ; ch_counter++
            
            mov.b #SD16INCH_1,&SD16INCTL0   ; Enable channel A1+/-
            mov.b #SD16AE2, &SD16AE         ; Enable external input on A1+
            jmp   Done                      ; break
   
   
Case1       mov.w &SD16MEM0, R8             ; R8 = A1 conversion results
            inc.w R5                        ; ch_counter++

            mov.b #SD16INCH_2,&SD16INCTL0   ; Enable channel A2+/-
            mov.b #SD16AE4, &SD16AE         ; Enable external input on A2+
            jmp   Done                      ; break
   

Case2       mov.w &SD16MEM0, R9             ; R9 = A2 conversion results
            mov.w #0h, R5                   ; Reset channel count (end of seq)
            mov.b #SD16AE0, &SD16AE         ; Reset external input to A0+/-
            mov.b #SD16INCH_0,&SD16INCTL0   ; Reset channel observed

Done        bic.w #LPM0,0(SP)               ; Exit LPM0
            reti                            ;		
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int07"                ; SD16 Vector
            .short  SD16_ISR                ;
            .end
