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
;*******************************************************************************
; MSP430x530x Demo - Port Mapping Port4; Single runtime configuration
;
; Description:  Port4 is port mapped to output TimerB digital signals.
; TBCCR1-TBCCR6 generate different PWM DutyCycles and have been output on Port4
; pins. TBCCR0 generates a square wave of freq ACLK/2^10 ~32Hz.
; MCLK = SMCLK = default DCO; ACLK = REFO~32kHz;                           
;                                                       
;                 MSP430x530x
;             ------------------                        
;         /|\|                  |                       
;          | |                  |                       
;          --|RST               |                                   
;            |     P4.0(TB0CCR0)|--> ACLK/2^10 ~ 32Hz                
;            |     P4.1(TB0CCR1)|--> 25%   
;            |     P4.2(TB0CCR2)|--> 50% 
;            |     P4.3(TB0CCR3)|--> 62.5%                 
;            |     P4.4(TB0CCR4)|--> 75%                
;            |     P4.5(TB0CCR5)|--> 87.5%                 
;            |     P4.6(TB0CCR6)|--> 94% 
;            |     P4.7(PM_None)|--> DVSS
;                 
;  K. Chen
;  Texas Instruments Inc.
;  March 2012
;  Built with CCS Version: 5.2
;*******************************************************************************

 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
;-------------------------------------------------------------------------------
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT

            calla   #Port_Mapping           ;

; Setup Port Pins            
            mov.b   #0xFF,&P4DIR            ; P4.0 - P4.7 output
            mov.b   #0xFF,&P4SEL            ; P4.0 - P4.6 Port Map functions

; Setup TB0
            mov.w   #OUTMOD_4,&TB0CCTL0     ; CCR0 Toggle
            mov.w   #256,&TB0CCR0           ; PWM Period/2
            mov.w   #OUTMOD_6,&TB0CCTL1     ; CCR1 toggle/set
            mov.w   #192,&TB0CCR1           ; CCR1 PWM duty cycle
            mov.w   #OUTMOD_6,&TB0CCTL2     ; CCR2 toggle/set
            mov.w   #128,&TB0CCR2           ; CCR2 PWM duty cycle
            mov.w   #OUTMOD_6,&TB0CCTL3     ; CCR3 toggle/set
            mov.w   #96,&TB0CCR3            ; CCR3 PWM duty cycle
            mov.w   #OUTMOD_6,&TB0CCTL4     ; CCR4 toggle/set
            mov.w   #64,&TB0CCR4            ; CCR4 PWM duty cycle
            mov.w   #OUTMOD_6,&TB0CCTL5     ; CCR5 toggle/set
            mov.w   #32,&TB0CCR5            ; CCR5 PWM duty cycle
            mov.w   #OUTMOD_6,&TB0CCTL6     ; CCR6 toggle/set
            mov.w   #16,&TB0CCR6            ; CCR6 PWM duty cycle
            mov.w   #TBSSEL_1 | MC_3,&TB0CTL; ACLK/2, up-down mode

            bis.w   #LPM3,SR                ; Enter LPM3
            nop

;-------------------------------------------------------------------------------
Port_Mapping
;-------------------------------------------------------------------------------
            dint                            ; Disable Interrupts before altering 
                                            ; Port Mapping registers
            nop                             ;
            mov.w   #0x2D52,&PMAPKEYID      ; Enable Write-access to modify port 
                                            ; mapping registers
            mov.w   #0x2,&PMAPCTL           ; Allow reconfiguration during 
                                            ; runtime
            mov.w   #P4MAP0,R4              ; Point to P4MAP0
            clr.w   R5                      ; Clear the array index
for_loop    mov.b   P4mapping(R5),0(R4)     ; 
            inc.w   R4                      ; Increment pointer 
            inc.w   R5                      ; Increment array index
            cmp.w   #8,R5                   ; Is array index < 8 ? 
            jne     for_loop                ;

            clr.w   &PMAPKEYID              ; Disable Write-Access to modify 
                                            ; port mapping registers
            nop
            eint                            ; Re-enable all interrupts                                
            nop
            reta              
;-------------------------------------------------------------------------------
P4mapping
;-------------------------------------------------------------------------------
            .byte   PM_TB0CCR0A
            .byte   PM_TB0CCR1A
            .byte   PM_TB0CCR2A
            .byte   PM_TB0CCR3A
            .byte   PM_TB0CCR4A
            .byte   PM_TB0CCR5A
            .byte   PM_TB0CCR6A
            .byte   PM_NONE
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
