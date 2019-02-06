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
;  MSP430G22x0 Demo - Flash In-System Programming, Copy SegC to SegD
;
;  Description: This program first erases flash seg C, then it increments all
;  values in seg C, then it erases seg D, then copies seg C to seg D.
;  Assumed MCLK 771kHz - 1428kHz.
;  //* Set Breakpoint on NOP in the Mainloop to avoid Stressing Flash *//
;
;               MSP430G22x0
;            -----------------
;        /|\|              XIN|-
;         | |                 |
;         --|RST          XOUT|-
;           |                 |
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
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

            cmp.b   #0xFF,&CALBC1_1MHZ      ; Check calibration constant
            jne     Load                    ; if not erased, load.
Trap        jmp     Trap                    ; if erased do not load, trap CPU!

Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   CALBC1_1MHZ,&BCSCTL1    ; Set DCO to 1MHz
            mov.b   CALDCO_1MHZ,&DCOCTL
            mov.w   #FWKEY+FSSEL0+FN1,FCTL2 ; MCLK/3 for Flash Timing Generator
            clr.w   R15                     ; initialize value

loop        call    #write_SegC
            inc.b   R15                     ; Increment Value
            call    #copy_C2D
            nop                             ; SET BREAKPOINT HERE
            jmp     loop

;-------------------------------------------------------------------------------
write_SegC
;-------------------------------------------------------------------------------
            mov.w   #0x1040,R5              ; Initialize Flash pointer
            mov.w   #FWKEY,&FCTL3           ; Clear lock bit
            mov.w   #FWKEY+ERASE,&FCTL1     ; Set erase bit
            mov.w   #0,&0x1040              ; Dummy write to erase Flash segment
            mov.w   #FWKEY+WRT,&FCTL1       ; Set WRT bit for write operation

            mov.w   #0,R6                   ; Initialize counter
loop1       mov.b   R15,0(R5)               ; Write value to Flash
            inc.w   R5                      ; Increment Flash pointer
            inc.w   R6                      ; Increment counter
            cmp.w   #64,R6
            jne     loop1

            mov.w   #FWKEY,&FCTL1           ; Clear WRT bit
            mov.w   #FWKEY+LOCK,&FCTL3      ; Set LOCK bit
            ret

;-------------------------------------------------------------------------------
copy_C2D
;-------------------------------------------------------------------------------
            mov.w   #0x1040,R7              ; Initialize Flash segment C pointer
            mov.w   #0x1000,R8              ; Initialize Flash segment D pointer
            mov.w   #FWKEY,&FCTL3           ; Clear Lock bit
            mov.w   #FWKEY+ERASE,&FCTL1     ; Set Erase bit
            mov.w   #0,0(R8)                ; Dummy write to erase Flash segment D
            mov.w   #FWKEY+WRT,&FCTL1       ; Set WRT bit for write operation
            mov.w   #FWKEY,&FCTL3           ; Clear Lock bit

            mov.w   #0,R6                   ; Initialize counter
loop2       mov.w   @R7,R9
            mov.b   R9,0(R8)                ; Copy value segment C to segment D
            inc.w   R7                      ; Increment pointers
            inc.w   R8
            inc.w   R6                      ; Increment counter
            cmp.w   #64,R6
            jne     loop2

            mov.w   #FWKEY,&FCTL1           ; Clear WRT bit
            mov.w   #FWKEY+LOCK,&FCTL3      ; Set LOCK bit
            ret

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
