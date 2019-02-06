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
;****************************************************************************
;  MSP-FET430P140 Demo - Flash In-System Programming, BlockWrite
;
;  Description: This program first copies the FlashWrite routine to RAM, then
;  erases flash seg A, then it increments all values in seg A using
;  block write mode.
;
;  Assumed default MCLK = DCO ~800kHz
;  Minimum RAM requirement = 512 bytes 
;
;  ** Set Breakpoint on JMP at end of Mainloop to avoid Stressing Flash **
;
;               MSP430F169
;            -----------------
;        /|\|              XIN|-
;         | |                 |
;         --|RST          XOUT|-
;           |                 |
;
value   .equ     R4
;
;  H. Grewal / L. Westlund
;  Texas Instruments Inc.
;  Jun 2006
;  Built with Code Composer Essentials Version: 1.0
;******************************************************************************

 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;------------------------------------------------------------------------------
            .text                           ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            clr.b   value                   ; value = value to write to flash

            CALL    #CopyRoutine            ; Copy FlashWrite Routine to RAM

Mainloop    MOV.W   #01000h,R6              ; Write pointer
            MOV     #FWKEY+FSSEL1+FN0,&FCTL2; MCLK/2 for Flash Timing Generator
            MOV     #FWKEY+ERASE,&FCTL1     ; Set Erase bit
            MOV     #FWKEY,&FCTL3           ; Clear Lock bit
            CLR     0(R6)                   ; Dummy write to erase Flash segment
            CALL    #0300h                  ; Execute FlashWrite from RAM
            inc.b   value                   ; Increment value written to Flash
            jmp     Mainloop                ; Repeat, SET BREAKPOINT HERE

;------------------------------------------------------------------------------
CopyRoutine ; Copy FlashWrite Routine to RAM
;------------------------------------------------------------------------------
            MOV.W   #WriteFlash, R5         ; Set pointer to WriteFlash Routine
            MOV.W   #0300h, R6              ; Set pointer to RAM

CR_loop     MOV.W   0(R5), 0(R6)            ; Copy word to RAM
            CMP.W   #END_wf, R5             ; Check for end of FlashWrite
            JZ      END_found               ; Jump out if at end of WriteFlash
            INCD    R5                      ; Double-increment WriteFlash ptr
            INCD    R6                      ; Double-increment RAM pointer
            JMP     CR_loop                 ; Repeat
END_found   RET                             ;

;------------------------------------------------------------------------------
WriteFlash  ; Write one block starting at 1000h.
; The location for this routine will change to 0500h when copied to RAM
;------------------------------------------------------------------------------
            MOV.W   #64,R5                  ; Use as write counter

            DINT                            ; Disable interrupts
L1          BIT     #BUSY,&FCTL3            ; Test BUSY
            JNZ L1                          ; Loop while busy
            MOV.W   #FWKEY+BLKWRT+WRT,&FCTL1; Enable block write
L2          MOV.B   value,0(R6)             ; Write location
L3          BIT     #WAIT,&FCTL3            ; Test WAIT
            JZ      L3                      ; Loop while WAIT=0
            INC     R6                      ; Point to next word
            DEC     R5                      ; Decrement write counter
            JNZ     L2                      ; End of block?
            MOV.W   #FWKEY,&FCTL1           ; Clear WRT,BLKWRT
L4          BIT     #BUSY,&FCTL3            ; Test BUSY
            JNZ     L4                      ; Loop while busy
            MOV.W   #FWKEY+LOCK,&FCTL3      ; Set LOCK
            EINT                            ; Enable interrupts
END_wf      RET                             ; Exits Routine

;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
