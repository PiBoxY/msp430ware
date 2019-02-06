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
;   MSP430F20F3 Demo - SD16, Using the Integrated Temperature Sensor
;
;   Description: Use SD16 and it's integrated temperature sensor to detect
;   temperature delta. The temperature sensor output voltage is sampled
;   ~ every 240ms and compared with the last value and if more than ~0.5C
;   delta, P1.0 is set, else reset.
;   SD16 Internal VRef, Bipolar offset binary output format used.
;   Watchdog used as interval time and ISR used to start next SD16 conversion.
;   ACLK = n/a, MCLK = default DCO, SMCLK = SD16CLK = default DCO/8
;
;		 MSP430F20xx
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |A6+              |
;            |A6-          P1.0|-->LED
;
;   P.Thanigai
;   Texas Instruments Inc.
;   May 2007
;   Built with Code Composer Essentials Version: 2.0
;*******************************************************************************
ADCDeltaOn  .equ      31                     ;  ~ 0.5 Deg C delta for LED on
;   CPU Registers Used
LastADCVal  .equ      R4
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; Initialize stackpointer
            bis.b   #DIVS_3,&BCSCTL2        ; SMCLK/8
SetupWDT    mov.w   #WDT_MDLY_32,&WDTCTL    ; WDT Timer interval
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
SetupP1     bis.b   #001h,&P1DIR            ; P1.0 output
SetupSD16   mov.w   #SD16REFON+SD16SSEL_1,&SD16CTL  ; 1.2V ref, SMCLK
            mov.b   #SD16INCH_6,&SD16INCTL0       ; A6+/-
            mov.w   #SD16SNGL+SD16IE,&SD16CCTL0    ; Single conv, interrupt
                                            ;				
Mainloop    bis.w   #CPUOFF+GIE,SR          ; CPU off, enable interrupts
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
SD16_ISR;
;-------------------------------------------------------------------------------
            add.w   #ADCDeltaOn,LastADCVal  ;
            bic.b   #01h,&P1OUT             ; LED off
            cmp.w   LastADCVal,&SD16MEM0    ; Clears IFG
            jlo     SD16_ISR_1              ;
            bis.b   #01h,&P1OUT             ; LED on
SD16_ISR_1  mov.w   &SD16MEM0,LastADCVal    ; Store value
            reti                            ;		
                                            ;
;-------------------------------------------------------------------------------
WDT_ISR;
;-------------------------------------------------------------------------------
            bis.w   #SD16SC,&SD16CCTL0      ; Start conversion
            reti                            ;		
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int10"                ; Watchdog Vector
            .short  WDT_ISR                 ;
            .sect   ".int05"                ; SD16 Vector
            .short  SD16_ISR                ;            
            .end
