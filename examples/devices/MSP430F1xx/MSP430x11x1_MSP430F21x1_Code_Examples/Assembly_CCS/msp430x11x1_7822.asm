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
;   MSP430x11x1 Demo - Software Interface to Read ADS7822
;
;   Description: This program will read an ADS7822 ADC using software and
;   store the 12-bit digital code in ADCData (R11), buffering 16 samples in
;   RAM @ 200h.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~800k
;   ;* VCC must be 2.7v+ for ADS7822, add CAPs as required *//
;
;                      MSP430F1121
;                    -----------------
;                /|\|              XIN|-
;                 | |                 |
;                 --|RST          XOUT|-
;                   |                 |
;     ADS7822       |                 |
;    ----------     |                 |
;   |        CS|<---|P2.0             |
; ~>|+In DCLOCK|<---|P2.1             |
;   |      Dout|--->|P2.3             |
;
Pointer  .equ     R10
ADCData  .equ     R11
Counter  .equ     R12
CS          .equ     001h                   ; P2.0 - Chip Select
DCLOCK      .equ     002h                   ; P2.1 - Clock
Dout        .equ     008h                   ; P2.3 - Data Out
;
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
RESET       mov.w   #0300h,SP               ; Initialize stack
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer
SetupP2     mov.b   #CS,&P2OUT              ; /CS set, - P2.x reset
            bis.b   #CS+DCLOCK,&P2DIR       ; /CS and DCLOCK outputs
                                            ;
Mainloop    call    #Meas_ADC_16            ; Call subroutine
            jmp     Mainloop                ; Repeat
                                            ;
;-------------------------------------------------------------------------------
Meas_ADC_16; Subroutine to call  Meas_7822 16x, results stored in RAM @200h
;           Pointer (R10) is used.
;-------------------------------------------------------------------------------
            mov.w   #200h,Pointer           ; Buffer @ 200h
Meas_Loop   call    #Meas_7822              ;
            mov.w   ADCData,0(Pointer)      ; Store ADC code
            incd.w  Pointer                 ;
            cmp.w   #0220h,Pointer          ; End of 16 word (32 byte) buffer
            jne     Meas_Loop               ;
            ret                             ; Return from subroutine

;-------------------------------------------------------------------------------
Meas_7822;  Subroutine to read ADS7822, data is shifted into ADCData (R11),
;           Counter (R12) is used as a bit counter.
;-------------------------------------------------------------------------------
            mov.w   #15,Counter             ; 2 smpl+null+12 data bits
            clr.w   ADCData                 ; Clear data buffer
            bic.b   #CS,&P2OUT              ; /CS reset, enable ADC
ADC_Loop    bis.b   #DCLOCK,&P2OUT          ; (4) Clock high
            bit.b   #Dout,&P2IN             ; (4) Dout -> C (carry)
            rlc.w   ADCData                 ; (1) C -> ADCData
            bic.b   #DCLOCK,&P2OUT          ; (4) Clock low
            dec.w   Counter                 ; (1) All bits shifted in?
            jnz     ADC_Loop                ; (2) If not --> ADC_Loop
            bis.b   #CS,&P2OUT              ; /CS set, disable ADC
            and.w   #0FFFh,ADCData          ; Keep only databits
            ret                             ; Return from subroutine
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
