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
;   Description: Output several Analog Signals using the APOOL DAC. A triangle   
;   waveform is created on the ouput of the DAC.  A timer interrupt is used to   
;   change the value of the DAC.                                                 
;                                                                                
;***************************A_POOL DAC ***************************************** 
;                                                                                
;                                                                                
;              +----L092---+                                                     
;              |*1      14 |                                                     
;              | 2      13 |                                                     
;              | 3      12 |                                                     
;              | 4      11 |                                                     
;              | 5      10 |                                                     
;              | 6       9 |                                                     
;              | 7       8 |  -> AOUT- Analog Output Provided                    
;              +-----------+                                                     
;                                                                                
;  D.Dang/D.Archbold/D.Szmulewicz
;  Texas Instruments Inc.
;  Built with CCS version 4.2.0
;******************************************************************************

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .data                           ; RAM variables
;------------------------------------------------------------------------------
            .bss    Output,1
;-------------------------------------------------------------------------------
            .global main
            .text                           ; Assemble to memory
;-------------------------------------------------------------------------------
main        mov.w   #WDTPW + WDTHOLD,&WDTCTL ; Stop WDT                        
            mov.w   #0x00, &Output          ; Initialize Output to 0x00
; Setup of TimerA0 Capture Compare Register 0    
            mov.w   #CCIE,&TA0CCTL0         ; TA0CCRO Interupt Enable 
            mov.w   #1,&TA0CCR0 
   
;Setup of the A-POOL module for DAC output
            mov.w   #DBON+CONVON+APREFON,&APCNF ; Configure A-POOL elements
            bis.w   #ODEN+OSEL,&APCTL       ; A-POOL Comparator/Saturation Based Stop Enable and Running 
    
; Setup of TimerA0 Control Register   
            mov.w   #TASSEL_2 + MC_1 + TACLR,&TA0CTL ; SMCLK, Up Mode
            bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupts enabled
            nop                             ; Needed for debugger 

; Timer0_A0 interrupt service routine
;-------------------------------------------------------------------------------
TA0_ISR                                     
;-------------------------------------------------------------------------------
            cmp     #0xFF,&APINT             ; Check for Max Value of Analog Output 256mV
            jl      increase_val            ; Branch to Increase the Value of the Output by 1mv
            mov.b   #0,&Output              ; Re-initialize the output value
            mov.b   #0,&APINT                ; When max value has been reached, restart with output of 1mv
            jmp     return
increase_val:
            mov.b   &Output,&APINT
            inc     &Output 
return      reti                            ; Return from ISR  

;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int07"                ; TIMER0_A0_VECTOR
            .short  TA0_ISR                   
            .end
