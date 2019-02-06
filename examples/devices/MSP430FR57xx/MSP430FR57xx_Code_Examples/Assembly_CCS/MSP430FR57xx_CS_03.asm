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
;   MSP430FR57xx Demo - Output 32768Hz crystal on XT1 and observe failsafe 
;
;   Description: Configure ACLK = LFXT1 and enter LPM3. 
;   To observe failsafe behavior short the crytsal briefly on the target board.
;   This will cause an NMI to occur. P1.0 is toggled inside the NMI ISR. 
;   Once the fault flag is cleared XT1 operation continues from 32768Hz crystal
;   Otherwise ACLK defaults to VLO (~8KHz).
;   ACLK = LFXT1 = 32kHz, MCLK = SMCLK = 4MHz
; 
;           MSP430FR57x
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |          P2.0 |--->ACLK  
;        |          P1.0 |--->LED
;
;   F.  Chen
;   Texas Instruments Inc.
;   November 2012
;   Built with Code Composer Studio V5.2.1
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

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
            clr.b   &P1OUT                  ; LED Setup
            bis.b   #BIT0,&P1DIR
            bis.b   #BIT4+BIT5,&PJSEL0      ; XT1 Setup
            mov.b   #0xA5,&CSCTL0_H
            bis.w   #DCOFSEL0+DCOFSEL1,&CSCTL1 ; Set max DCO setting
            mov.w   #SELA_0+SELS_3+SELM_3,&CSCTL2 ; ACLK = XT1; MCLK = DCO
            mov.w   #DIVA_0+DIVS_1+DIVM_1,&CSCTL3 ; set all dividers
            bis.w   #XT1DRIVE_0,&CSCTL4
            bic.w   #XT1OFF,&CSCTL4
OSCFlag     bic.w   #XT1OFFG,&CSCTL5        ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag

            bis.w   #OFIE,&SFRIE1           ; now that osc is running enable fault interrupt
            clr.b   &P2OUT                  ; output ACLK
            bis.b   #BIT0,&P2DIR
            bis.b   #BIT0,&P2SEL1
            bis.b   #BIT0,&P2SEL0
            
            nop
Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3 w/ interrupt
            nop                             ; for debug
            
;------------------------------------------------------------------------------
UNMI_ISR;   UNMI Interrupt Service Routine
;------------------------------------------------------------------------------
            ; Set a break point on the line below to observe XT1 operating from
            ; VLO when the breakpoint is hit during a crystal fault
Again       bic.w   #XT1OFFG,&CSCTL5        ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bis.b   #BIT0,&P1OUT
            mov.w   #12500,R15              ; time for flag to get set again
Wait        dec.w   R15
            jnz     Wait
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     Again
            bic.b   #BIT0,&P1OUT
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   UNMI_VECTOR             ; UNMI Vector
            .short  UNMI_ISR;
            .end
