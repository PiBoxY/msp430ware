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
;  MSP430FR59x Demo - MPU Write protection violation - Interrupt notification
;
;  Description: The MPU segment boundaries are defined by:
;  Border 1 = 0x6000 [MPUSEGB1 = 0x0600]
;  Border 2 = 0x8000 [MPUSEGB2 = 0x0800]
;  Segment 1 = 0x4400 - 0x5FFF
;  Segment 2 = 0x6000 - 0x7FFF
;  Segment 3 = 0x8000 - 0x13FFF
;  Segment 2 is write protected. Any write to an address in the segment 2 range
;  causes a PUC. The LED toggles after accessing SYS NMI ISR.
;
;  ACLK = n/a, MCLK = SMCLK = default DCO
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
SYSNMIflag  .set    R5
ptr         .set    R6
data        .set    R7
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

SetupMPU    mov.w   #MPUPW,&MPUCTL0         ; Write PWD to acces MPU registers
            mov.w   #0x0600,&MPUSEGB1       ; B1 = 0x6000; B2 = 0x8000
            mov.w   #0x0800,&MPUSEGB2       ; Borders are assigned to segments
            bic.w   #MPUSEG2WE,&MPUSAM      ; Segment 2 is protected from write
            bic.w   #MPUSEG2VS,&MPUSAM      ; Violation select on write access
            mov.w   #MPUPW+MPUENA+MPUSEGIE,&MPUCTL0 ; Enable MPU protection
            clr.w   SYSNMIflag

            mov.w   #0x88,data
            mov.w   #0x6002,ptr             ; Cause an MPU violation by writing to segment 2
            mov.w   data,0(ptr)

            mov.w   #0x001e,R15
Delay1      dec.w   R15
            jne     Delay1

Delay2      xor.b   #BIT0,&P1OUT            ; Toggle LED
            mov.w   #0x8232,R15
L1          dec.w   R15
            jnz     L1
            tst.w   SYSNMIflag              ; Has violation occured due to Seg2
            jnz     Delay2                  ; Delay to see toggle

Mainloop    jmp     Mainloop                ; No violation - trap here
            nop

;------------------------------------------------------------------------------
SYSNMI_ISR;    System Non-maskable Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &SYSSNIV,PC             ; add offset to PC
            reti                            ; SYSSNIV_NONE:        No interrupt
            reti                            ; SYSSNIV_RES02:       SVS low_power reset
            reti                            ; SYSSNIV_UBDIFG:      FRAM Uncorrectable bit Error
            reti                            ; SYSSNIV_RES06:       Access time error
            reti                            ; SYSSNIV_MPUSEGPIFG:  MPUSEGPIFG violation
            reti                            ; SYSSNIV_MPUSEGIIFG:  MPUSEGIIFG violation
            reti                            ; SYSSNIV_MPUSEG1IFG:  MPUSEG1IFG violation
            jmp     MEM0                    ; SYSSNIV_MPUSEG2IFG:  MPUSEG2IFG violation
            reti                            ; SYSSNIV_MPUSEG3IFG:  MPUSEG3IFG violation
            reti                            ; SYSSNIV_VMAIFG:      VMAIFG
            reti                            ; SYSSNIV_JMBINIFG:    JMBINIFG
            reti                            ; SYSSNIV_JMBOUTIFG:   JMBOUTIFG
            reti                            ; SYSSNIV_CBDIFG:      FRAM Correctable Bit error
MEM0        bic.w   #MPUSEG2IFG,&MPUCTL1    ; Clear violation interrupt flag
            mov.w   #1,SYSNMIflag           ; Set flag
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   SYSNMI_VECTOR           ; System Non-maskable Interrupt Vector
            .short  SYSNMI_ISR              ;
            .end

