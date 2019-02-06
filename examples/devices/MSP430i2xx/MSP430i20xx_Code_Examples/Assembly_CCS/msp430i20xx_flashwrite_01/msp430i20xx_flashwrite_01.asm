; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2013, Texas Instruments Incorporated
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
;  MSP430i20xx Demo - Flash In-System Programming, Information Segment
;
;  Description: Unlock the Information Segment, overwrite a number of values
;    within it, then re-lock the Information Segment when writes are complete.
;
;  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;  Notes: Set breakpoint on NOP in main loop to avoid stressing flash
;
;  Warning: DO NOT ERASE ENTIRE INFORMATION SEGMENT, important values (TLV, etc)
;             are contained within this segment
;
;               MSP430i20xx
;             -----------------
;         /|\|                |
;          | |                |
;          --|RST             |
;            |                |
;            |                |
;            |                |
;
;   T. Witt
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio v5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
			
			.ref    init                    ; Reference external function

RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
Cal         call    #init                   ; Calibrate peripherals
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            mov.w   #FWKEY|FSSEL0|FN1|FN3|FN5,&FCTL2 ; MCLK/42
            call    #write_InfoSeg          ; Write a block of the info seg

Mainloop    nop                             ; SET BREAKPOINT HERE
            jmp     Mainloop                ; Again
                                            ;

;-------------------------------------------------------------------------------
write_InfoSeg    ; Write a block of the Info Seg in Flash
;-------------------------------------------------------------------------------
            mov.w   #0x1000,R15             ; Initialize Flash Pointer
            bit.w   #LOCKSEG,&FCTL3         ; Segment Locked?
            jz      Unlocked
            mov.w   #FWKEY|LOCKSEG,&FCTL3   ; Clear LOCKSEG bit
Unlocked    mov.w   #FWKEY|WRT,&FCTL1       ; Set WRT bit for write operation
            mov.w   #0x10,R14               ; Set length of block to write
Write       mov.w   #0xA5A5,0x0(R15)        ; Write to flash
            incd.w  R15                     ; Increment pointer
            dec.w   R14                     ; Decrement block length
            jnz     Write                   ; Done writing?
            mov.w   #FWKEY,&FCTL1           ; Yes, Clear WRT BIT
            mov.w   #FWKEY|LOCKSEG,&FCTL3   ; Set LOCKSEG bit
            ret

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .end
