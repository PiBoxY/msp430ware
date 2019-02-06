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
;  MSP430FR59x Demo - MPU Write protection violation - PUC
;
;  Description: The MPU segment boundaries are defined by:
;  Border 1 = 0x6000 [MPUSEGB1 = 0x0600]
;  Border 2 = 0x8000 [MPUSEGB2 = 0x0800]
;  Segment 1 = 0x4400 - 0x5FFF
;  Segment 2 = 0x6000 - 0x7FFF
;  Segment 3 = 0x8000 - 0x13FFF
;  Segment 2 is write protected. Any write to an address in the segment 2 range
;  causes a PUC. The LED toggles after reset once MPUSEG2IFG is set. A delay is included
;  so the debugger can gain access via JTAG.
;
;  ACLK = n/a, MCLK = default DCO = 1MHz
;
;
;           MSP430FR5969
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |           P1.0|-->LED
;
;   E. Chen
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio V5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
ptr         .set    R5
data        .set    R6
;------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #BIT0,&P1DIR            ; Configure P1.0 for LED

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

Delay       xor.b   #BIT0,&P1OUT            ; Toggle LED
            mov.w   #15000,R15
L1          dec.w   R15
            jnz     L1
            bit.w   #MPUSEG2IFG,&MPUCTL1    ; has reset occured due to Seg2
            jnz     Delay                   ; Delay to see toggle

SetupMPU    mov.w   #MPUPW,&MPUCTL0         ; Write PWD to acces MPU registers
            mov.w   #0x0600,&MPUSEGB1       ; B1 = 0x6000; B2 = 0x8000
            mov.w   #0x0800,&MPUSEGB2       ; Borders are assigned to segments
            bic.w   #MPUSEG2WE,&MPUSAM      ; Segment 2 is protected from write
            bis.w   #MPUSEG2VS,&MPUSAM      ; Violation select on write access
            mov.w   #MPUPW+MPUENA,&MPUCTL0  ; Enable MPU protection

            mov.w   #0x88,data
            mov.w   #0x6002,ptr             ; Cause an MPU violation by writing to segment 2
            mov.w   data,0(ptr)

Mainloop    jmp     Mainloop                ; Code never gets here
            nop

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end

