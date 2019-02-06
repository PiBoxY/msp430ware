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
;  MSP430FR231x Demo - Timer0_B3 Capture of ACLK
;
;  Description; Capture a number of periods of the ACLK clock and store them in an array.
;  When the set number of periods is captured the program is trapped and the LED on
;  P1.0 is toggled. At this point halt the program execution read out the values using
;  the debugger.
;  ACLK = REFOCLK = 32kHz, MCLK = SMCLK = default DCODIV = 1MHz.
;
;                MSP430FR2311
;             -----------------
;         /|\|                 |
;          | |                 |
;          --|RST              |
;            |                 |
;            |             P1.1|-->ACLK
;            |             P1.0|-->LED
;
;
;   Darren Lu
;   Texas Instruments Inc.
;   July 2015
;   Built with Code Composer Studio v6.1
;******************************************************************************

            .cdecls C,LIST,"msp430.h"                       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                                  ; Make stack linker segment ?known?
                
            .global _main
            .text                                           ; Assemble to Flash memory
            .retain                                         ; Ensure current section gets linked
            .retainrefs
_main
            
RESET       mov.w   #__STACK_END,SP                         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL                  ; Stop WDT

            bis.b   #BIT0,&P1DIR                            ; P1.0 LED
            bis.b   #BIT0,&P1OUT

            bis.b   #BIT1,&P1SEL1                           ; Set as ACLK pin, second function
            bis.b   #BIT1,&P1DIR                            ; output ACLK pin
            bic.w   #LOCKLPM5,&PM5CTL0                      ; Unlock I/O pin

            bis.w   #CM_1|CCIS_1|SCS|CAP|CCIE,&TB0CCTL0     ; Capture rising edge,
                                                            ; Use CCI0B=ACLK,
                                                            ; Synchronous capture,
                                                            ; Enable capture mode,
                                                            ; Enable capture interrupt
    
            bis.w   #TBSSEL_2|MC_2|TBCLR,&TB0CTL            ; Use SMCLK as clock source,
                                                            ; Start timer in continuous mode
            mov.w   #0,R15
            nop
Mainloop    bis.w   #LPM0+GIE,SR                            ; Enter LPM0 w/ interrupt
            nop                                             ; For debugger

;-------------------------------------------------------------------------------
TIMER0_B0_ISR;    ISR for TB0CCR0
;-------------------------------------------------------------------------------
            bic.w   &TB0CCTL0,&CCIFG;
            mov.w   &TB0CCR0,0X2000(R15)
            add.w   #0x2,R15
            cmp.w   #0x002A,R15
            jlo     L1
Toggle      xor.b   #BIT0,&P1OUT
            mov.w   #50000,R14
L2          dec.w   R14
            jnz     L2
            jmp     Toggle
L1          reti
            
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   RESET_VECTOR                            ; MSP430 RESET Vector
            .short  RESET                                   
            .sect   TIMER0_B0_VECTOR                        ; Timer_A0 Vector
            .short  TIMER0_B0_ISR
            .end
