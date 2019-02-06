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
;***************************WDT Toggle Port Pin ******************************** 
;Toggle P1.0 using software timed by WDT ISR                                      
;                                                                                
;                                                                                
;                               +----L092---+                                    
;                               |*1      14 |                                    
;                               | 2      13 |                                    
;                               | 3      12 |                                    
;                               | 4      11 |                                    
;                               | 5      10 |                                    
;       P1.0 -Toggle Output  <- | 6       9 |                                    
;                               | 7       8 |                                    
;                               +-----------+                                    
;                                                                                
;  D.Dang/D.Archbold                                                             
;  Texas Instruments Inc.                                                        
;  D.Dang;D.Archbold;D.Szmulewicz
;  Texas Instruments Inc.
;  Built with CCS version 4.2.0
;******************************************************************************

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global main
            .text                           ; Assemble to memory
;-------------------------------------------------------------------------------
main
            mov.w   #0xF7FF,SP                ; Initialize stack pointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL ; Stop WDT  
            bis.b   #BIT0,&P1DIR            ; Set P1.0 to output direction
            bis.b   #BIT0,&P1OUT            ; Output high on P1.0
    

	
	;********************** 
	; Setup CCS             
	; ACLK = VLO            
	; MCLK = HFCLK/8        
	;********************** 
	        mov.w   #CCSKEY,&CCSCTL0        ; Unlock CCS
	        mov.w   #SELA_1,&CCSCTL4        ; Select LFCLK/VLO as the ACLK source         
            bic.w   #DIVA_5,&CCSCTL5        ; Divide fACLK by 4
	        bis.b   #0xFF,&CCSCTL0_H        ; Lock CCS
; Lock by writing to upper byte  
	
	        mov.w   #WDT_ADLY_16,&WDTCTL    ; WDT SMCLK Delay Interval specify  
            bis.b   #WDTIE,&SFRIE1          ; Enable WDT interrupt              
    

            bis.w   #LPM0+GIE,SR            ; LPM0, enable interrupts
            nop                             ; Required only for debugger
;-------------------------------------------------------------------------------
WDT_ISR;    Watchdog Timer interrupt service routine
;-------------------------------------------------------------------------------
            xor.b   #BIT0,&P1OUT            ; P1.0 = toggle
            reti                            ; Return from ISR
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
         
            .sect   ".int10"                 ; WDT_vector
            .short  WDT_ISR                   ;WDT Vector
            .end

 
