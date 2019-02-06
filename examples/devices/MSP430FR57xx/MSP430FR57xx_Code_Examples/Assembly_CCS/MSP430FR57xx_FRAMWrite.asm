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
;   MSP430FR57xx Demo - Long word writes to FRAM
;
;   Description: Use long word write to write to 512 byte blocks of FRAM.
;   Toggle LED after every 100 writes.
;   NOTE: Running this example for extended periods will impact the FRAM
;   endurance. 
;   ACLK = VLO, MCLK = SMCLK = 4MHz
; 
;           MSP430FR57x
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |               |  
;        |          P1.0 |---> LED
;
;   Tyler Witt
;   Texas Instruments Inc.
;   September 2011
;   Built with Code Composer Studio V4.2
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
FRAM_write_ptr  .set    R5
data            .set    R6
count           .set    R7
FRAM_TEST_START .set    0xCA00
;------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #0xA5,&CSCTL0_H
            bis.w   #DCOFSEL0+DCOFSEL1,&CSCTL1 ; Set max DCO setting
            mov.w   #SELA_0+SELS_3+SELM_3,&CSCTL2 ; ACLK = VLO
            mov.w   #DIVA_0+DIVS_3+DIVM_3,&CSCTL3 ; MCLK = SMCLK = DCO/2
            bis.w   #REFTCOFF,&REFCTL0      ; Turn off temp sensor
            bic.w   #REFON,&REFCTL0
            bis.b   #BIT0,&P1DIR            ; Turn on LED
            mov.w   #0x1111,data            ; Initialize dummy data
            mov.w   #00h,count              ; Initialize counter

Mainloop    inc.w   data
            mov.w   #FRAM_TEST_START,FRAM_write_ptr
            call    #FRAMWrite              ; Endless loop
            inc.w   count                   ; Use R14 as counter
            cmp.w   #101,count
            jnz     Mainloop
            xor.b   #01h,&P1OUT             ; Toggle LED to show 512K bytes
            clr.w   count                   ; ...have been written
            mov.w   #0x1111,data            ; After 100 times, reset dummy data
            jmp     Mainloop
            
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
 
