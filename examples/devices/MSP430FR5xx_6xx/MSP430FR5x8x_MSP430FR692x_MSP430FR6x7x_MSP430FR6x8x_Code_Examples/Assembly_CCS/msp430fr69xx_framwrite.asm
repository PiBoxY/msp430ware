; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2014, Texas Instruments Incorporated
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
;   MSP430FR69xx Demo - Long word writes to FRAM
;
;   Description: Use long word write to write to 512 byte blocks of FRAM.
;   Toggle LED after every 100 writes.
;   NOTE: Running this example for extended periods will impact the FRAM
;   endurance.
;   MCLK = SMCLK = default DCO
;
;           MSP430FR5969
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |               |
;        |          P1.0 |---> LED
;
;   E. Chen
;   Texas Instruments Inc.
;   April 2014
;   Built with Code Composer Studio V6.0
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
FRAM_write_ptr  .set R5
data            .set R6
count           .set R7
FRAM_TEST_START .set 0xD000
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupGPIO   bic.b   #BIT0,&P1OUT            ; Clear P1.0 output latch for a defined power-on state
            bis.b   #BIT0,&P1DIR            ; Set P1.0 to output direction

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

            mov.w   #0x0001,data            ; Initialize dummy data
            mov.w   #0x00,count             ; Initialize counter

Mainloop    inc.w   data
            mov.w   #FRAM_TEST_START,FRAM_write_ptr
            call    #FRAMWrite              ; Endless loop
            inc.w   count                   ; Use R14 as counter
            cmp.w   #101,count
            jnz     Mainloop
            xor.b   #01h,&P1OUT             ; Toggle LED to show 512K bytes
            clr.w   count                   ; ...have been written
            mov.w   #0x0001,data            ; After 100 times, reset dummy data
            jmp     Mainloop

Wait        mov.w   #50000,R15              ; Delay to R15
L1          dec.w   R15                     ; Decrement R15
            jnz     L1                      ; Delay over?
            jmp     Mainloop                ; Again

;------------------------------------------------------------------------------
FRAMWrite   ; Write to the FRAM
;------------------------------------------------------------------------------
            clr.w   R15                     ; Use gen purpose register as index
Start       inc.w   R15
            cmp.w   #101h,R15               ; for 128 blocks...
            jhs     Exit
            mov.w   data,0x0(FRAM_write_ptr) ; write to FRAM
            incd.w  FRAM_write_ptr
            jmp     Start
Exit        ret
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
