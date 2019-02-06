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
;  MSP430F530x Demo - Single-Byte Flash In-System Programming, Copy SegC to SegD
;
;  Description: This program first erases flash seg C, then it increments all
;  values in seg C, then it erases seg D, then copies seg C to seg D. Starting
;  addresses of segments defined in this code: Seg C-0x1880, Seg D-0x1800.
;  ACLK = REFO = 32kHz, MCLK = SMCLK = default DCO 1048576Hz
;  //* Set Breakpoint on NOP in the Mainloop to avoid Stressing Flash *//
;
;                MSP430F530x
;            -----------------
;        /|\|              XIN|-
;         | |                 |
;         --|RST          XOUT|-
;           |                 |
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

value  .equ  R4

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
            clr.w   value                   ; initialize value

            call    #write_SegC             ; Write segment C, increment value
            call    #copy_C2D               ; Copy segment C to D
Mainloop    jmp     Mainloop                ; Loop forever, SET BREAKPOINT HERE
            
;-------------------------------------------------------------------------------
write_SegC ;Input = value, holds value to write to Seg C, R5 used as working reg
;-------------------------------------------------------------------------------
            mov.w   #0x1880,R5              ; Initialize Flash pointer
            dint                            ; 5xx Workaround: Disable global
                                            ; interrupt while erasing. Re-Enable
                                            ; GIE if needed
            nop
            mov.w   #FWKEY,&FCTL3           ; Clear Lock bit
            mov.w   #FWKEY|ERASE,&FCTL1     ; Set Erase bit
            mov.w   #0,0(R5)                ; Dummy write to erase Flash seg
            mov.w   #FWKEY|WRT,&FCTL1       ; Set WRT bit for write operation
wrt_SegC_L1 mov.b   value,0(R5)             ; Write value to flash
            inc.w   R5
            inc.b   value;
            cmp.w   #0x1900,R5
            jne     wrt_SegC_L1
            mov.w   #FWKEY,&FCTL1           ; Clear WRT bit
            mov.w   #FWKEY|LOCK,&FCTL3      ; Set LOCK bit
            ret                             ;
;-------------------------------------------------------------------------------
copy_C2D ;  Copy Seg C to Seg D, R5 used as working reg.
;-------------------------------------------------------------------------------
            mov.w   #0x1880,R5              ; Initialize Flash segment C ptr
            dint                            ; 5xx Workaround: Disable global
                                            ; interrupt while erasing. Re-Enable
                                            ; GIE if needed
            nop
Erase_SegD  mov.w   #FWKEY,&FCTL3           ; Clear Lock bit
            mov.w   #FWKEY|ERASE,&FCTL1     ; Set Erase bit
            mov.w   #0,&0x1800              ; Dummy write to erase Flash seg D
            mov.w   #FWKEY|WRT,&FCTL1       ; Set WRT bit for write operation
wrt_C2D_L2  mov.b   @R5+,-129(R5);
            cmp.w   #0x1900,R5
            jne     wrt_C2D_L2
            mov.w   #FWKEY,&FCTL1           ; Clear WRT bit
            mov.w   #FWKEY|LOCK,&FCTL3      ; Set LOCK bit
            ret                             ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
