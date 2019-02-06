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
;   MSP430FR57xx Demo - Enters LPM4
;
;   Description: Device enters LPM4
; 
;           MSP430FR57x
;         ---------------
;     /|\|               |
;      | |               |
;      --|RST            |
;        |               |
;        |               |  
;        |          P1.0 |---> Disconnect JP3 for power meas.
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
            clr.b   &P1DIR
            clr.b   &P1OUT
            mov.b   #0xFF,&P1REN
            
            clr.b   &P2DIR
            clr.b   &P2OUT
            mov.b   #0xFF,&P2REN
            
            clr.b   &P3DIR
            clr.b   &P3OUT
            mov.b   #0xFF,&P3REN
            
            clr.b   &P4DIR
            clr.b   &P4OUT
            mov.b   #0xFF,&P4REN
            
            mov.b   #0xA5,&CSCTL0_H
            bis.w   #DCOFSEL0+DCOFSEL1,&CSCTL1 ; Set max DCO setting
            mov.w   #SELA_1+SELS_3+SELM_3,&CSCTL2 ; set ACLK = VLO; MCLK = DCO
            mov.w   #DIVA_0+DIVS_1+DIVM_1,&CSCTL3 ; set all dividers
            mov.w   #XT1OFF+XT2OFF,&CSCTL4
            bic.w   #XT1OFFG+XT2OFFG,&CSCTL5    
            
            clr.w   &PJDIR
            clr.w   &PJOUT
            mov.w   #0xFF,&PJREN
            
            bis.w   #REFTCOFF,&REFCTL0      ; Turn off Temp sensor
            bic.w   #REFON,&REFCTL0

Mainloop    bis.w   #LPM4,SR                ; Go into LPM3 w/ interrupts
            
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
 
