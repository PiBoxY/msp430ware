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
;  MSP430G22x0 Demo - Basic Clock, Output Buffered clocks with preloaded DCO
;                     calibration constants for BCSCTL1 and DCOCTL.
;
;  Description: Buffer MCLK/10 on P1.2. DCO is software selectable to 1, 8, 12,
;  or 16Mhz using calibration constants in INFOA.
;
;  ACLK = VLO, MCLK = SMCLK = Selectable at 1, 8, 12 or 16Mhz
;
;               MSP430G22x0
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |             P1.2|-->MCLK/10 = DCO/10
;
;  B. Nisarga
;  Texas Instruments, Inc
;  December 2011
;  Built with CCE Version: 5.1 and IAR Embedded Workbench Version: 5.4
;******************************************************************************

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x280,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

            ; 1Mhz
            ;cmp.b   #0xFF,&CALBC1_1MHZ     ; Check calibration constant
            ;jne     Load                   ; if not erased, load.   
;Trap        jmp     Trap                   ; if erased do not load, trap CPU!
;Load       clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            ;mov.b   CALBC1_1MHZ,&BCSCTL1    ; Set Range
            ;mov.b  CALDCO_1MHZ,&DCOCTL ; Set DCO step + modulation

            ; 8Mhz
            ;cmp.b   #0xFF,&CALBC1_8MHZ     ; Check calibration constant
            ;jne     Load                   ; if not erased, load.   
;Trap        jmp     Trap                   ; if erased do not load, trap CPU!
;Load       clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            ;mov.b   CALBC1_8MHZ,&BCSCTL1    ; Set Range
            ;mov.b  CALDCO_8MHZ,&DCOCTL ; Set DCO step + modulation

            ; 12Mhz
            ;cmp.b   #0xFF,&CALBC1_12MHZ    ; Check calibration constant
            ;jne     Load                   ; if not erased, load.   
;Trap        jmp     Trap                   ; if erased do not load, trap CPU!
;Load       clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            ;mov.b   CALBC1_12MHZ,&BCSCTL1   ; Set Range
            ;mov.b  CALDCO_12MHZ,&DCOCTL    ; Set DCO step + modulation

            ; 16Mhz
            cmp.b   #0xFF,&CALBC1_16MHZ     ; Check calibration constant
            jne     Load                    ; if not erased, load.   
Trap        jmp     Trap                    ; if erased do not load, trap CPU!
Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   CALBC1_16MHZ,&BCSCTL1   ; Set Range
            mov.b   CALDCO_16MHZ,&DCOCTL    ; Set DCO step + modulation
            bis.b   #BIT2,&P1DIR            ; P1.2 output

loop        bis.b   #BIT2,&P1OUT            ; P1.2 = 1
            bic.b   #BIT2,&P1OUT            ; P1.2 = 0
            jmp     loop

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
