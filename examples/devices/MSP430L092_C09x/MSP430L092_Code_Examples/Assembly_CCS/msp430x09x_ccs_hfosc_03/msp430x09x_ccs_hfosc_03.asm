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
; Description: This program demonstrates how to configure ACLK, MCLK, and SMCLK  
; to use the HF clock source.                                                    
;***************************Timer_A0******************************************** 
;  Requirements: Oscilloscope                                                    
;                                                                                
;                                                                                
;                               +----L092---+                                    
;                               |*1      14 |                                    
;                               | 2      13 |                                    
;                               | 3      12 |  -> P1.4 MCLK ~125kHz              
;                               | 4      11 |                                    
;                               | 5      10 |                                    
;     P1.0 ACLK ~32kHz  <-      | 6       9 |                                    
;     P1.1 SMCLK ~62.5kHz  <-   | 7       8 |                                    
;                               +-----------+                                    
;                                                                                
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
main        mov.w   #WDTPW + WDTHOLD,&WDTCTL ; Stop WDT  

   ; Setup Port 1 to output ACLK, SMCLK, and MCLK
   ; P1.0 = ACLK -> P1DIR.0 = 1; P1SEL0.0 = 1; P1SEL1.0 = 1;
   ; P1.1 = SMCLK-> P1DIR.1 = 1; P1SEL0.1 = 1; P1SEL1.1 = 1;
   ; P1.4 = MCLK -> P1DIR.4 = 1; P1SEL0.4 = 1; P1SEL1.4 = 1;
    
            bis.b   #BIT0 + BIT1 + BIT4,&P1DIR;
            bis.b   #BIT0 + BIT1 + BIT4,&P1SEL0;
            bis.b   #BIT0 + BIT1 + BIT4,&P1SEL1    
            
	;********************** 
	; Setup CCS             
	; ACLK = HFCLK/32     	 
      ; MCLK = HFCLK/8     	 
      ; SMCLK = HFCLK/16    	 
	;********************** 
            mov.w    #CCSKEY,&CCSCTL0       ; Unlock CCS

check       mov.w    #0,&CCSCTL7            ; Clear HF & LF OSC fault flags
            mov.w    #0,&SFRIFG1            ; Clear OFIFG
            bit.w    #OFIFG,&SFRIFG1        ; Oscillator Flag(s)?
            jnz      check    
            
            mov.w    #SELA_0 + SELM_0 +SELS_0,& CCSCTL4 ;  Select HFCLK/DCO as the source for ACLK, MCLK, and SMCLK   
            mov.w    #DIVA_5 + DIVM_3 + DIVS_4,&CCSCTL5 ; Set the Dividers for ACLK to32, MCLK to 8, and SMCLK to 16
            bis.b    #0xFF,&CCSCTL0_H       ;  Lock CCS  
   
	
        ; Lock by writing to upper byte  
	
loop        jmp     loop


 
