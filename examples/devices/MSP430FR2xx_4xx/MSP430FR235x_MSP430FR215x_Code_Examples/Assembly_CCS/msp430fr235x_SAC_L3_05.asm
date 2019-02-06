; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2016, Texas Instruments Incorporated
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
;  MSP430FR235x Demo - SAC-L3, DAC Buffer Mode
;
;  Description: Configure SAC-L3 for DAC Buffer Mode. Use the 12 bit DAC to
;  output positive ramp. The OA is set in buffer mode to improve DAC output
;  drive strength. Internal 2.5V reference is selected as DAC reference.
;  Observe the output of OA0O pin with oscilloscope.
;  ACLK = n/a, MCLK = SMCLK = default DCODIV ~1MHz.
;
;                MSP430FR235x
;             -------------------
;         /|\|                   |
;          | |                   |
;          --|RST     DAC12->OA0O|--> oscilloscope
;            |                   |
;            |                   |
;            |                   |
;            |                   |
;            |                   |
;
;   Darren Lu
;   Texas Instruments Inc.
;   Oct. 2016
;   Built with Code Composer Studio v6.2
;******************************************************************************
            .cdecls C,LIST,"msp430.h"  ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

RESET       mov.w   #__STACK_END,SP         ; Initialize stack pointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

            ; Disable the GPIO power-on default high-impedance mode
            ; to activate previously configured port settings
            bic.w   #LOCKLPM5,&PM5CTL0

            mov.b   #PMMPW_H,&PMMCTL0_H     ; Unlock the PMM registers
            mov.w   #INTREFEN+REFVSEL_2,&PMMCTL2; Enable internal 2.5V reference
PollREF     bit.w   #REFGENRDY,&PMMCTL2     ; Poll till internal reference settles
            jz      PollREF                 ;

SetPin      bis.b   #BIT1,&P1SEL0           ; Select P1.1 as OA0O function
            bis.b   #BIT1,&P1SEL1           ; OA is used as buffer for DAC

            mov.w   #DACSREF_1+DACLSEL_2+DACIE,&SAC0DAC; Select 2.5V int Vref as DAC reference
            mov.w   #0,R13
            mov.w   R13,&SAC0DAT            ; Initial DAC data
            bis.w   #DACEN,&SAC0DAC         ; Enable DAC

            mov.w   #NMUXEN+PMUXEN+PSEL_1+NSEL_1,&SAC0OA; Enable negative and positive input
            bis.w   #OAPM,&SAC0OA           ; Select low speed and low power mode
            mov.w   #MSEL_1,&SAC0PGA        ; Set OA as buffer mode
            bis.w   #SACEN+OAEN,&SAC0OA     ; Enable SAC and OA

            ; Use TB2.1 as DAC hardware trigger
            mov.w   #0x63,&TB2CCR0           ; PWM Period/2
            mov.w   #OUTMOD_6,&TB2CCTL1;    ; TBCCR1 toggle/set
            mov.w   #0x32,&TB2CCR1           ; TBCCR1 PWM duty cycle
            mov.w   #TBSSEL__SMCLK+MC_1+TBCLR,&TB2CTL ; SMCLK, up mode, clear TBR

            nop
            bis.w   #LPM3+GIE,SR            ; Enter LPM3, Enable Interrupt
            nop

;-------------------------------------------------------------------------------
SAC0_ISR;    ISR for SAC0
;-------------------------------------------------------------------------------
            add.w   &SAC0IV,PC
            reti                            ; Vector  0: No interrupt
            reti                            ; Vector  2:
            jmp     SACDACMODE              ; Vector  4: SAC DAC Interrupt


SACDACMODE  add.w  #1,R13                   ;
            and.w  #0xFFF,R13               ;
            mov.w  R13,&SAC0DAT             ; DAC12 output positive ramp
            reti

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   RESET_VECTOR            ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   SAC0_SAC2_VECTOR
            .short  SAC0_ISR
            .end
