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
;*****************************************************************************
;  Description: Use Timer CCRx units and overflow to generate
;  independent timing intervals. For demonstration, TA0CCR1 and TA0CCR2
;  output units are optionally selected with port pins P1.5 and P1.6
;  in toggle mode. As such, these pins will toggle when respective TA0CCRx
;  registers match the TA0R counter. Interrupts are also enabled with all
;  TA0CCRx units, software loads offset to next interval only - as long as
;  the interval offset is added to TACCRx, toggle rate is generated in
;  hardware. Timer0_A1 overflow ISR is used to toggle P1.0 with software.
;  Proper use of the TA0IV interrupt vector generator is demonstrated.
;
;***************************Timer_A0******************************************** 
;                                                                                
;                                                                                
;                                                                                
;              +----L092---+                                                     
;              |*1      14 |     -> P1.6 TA0.1 output                            
;              | 2      13 |     -> P1.5 TA0.2 output                            
;              | 3      12 |                                                     
;              | 4      11 |                                                     
;              | 5      10 |                                                     
;              | 6       9 |                                                     
; SMCLK out<-  | 7       8 |                                                     
;              +-----------+                                                     
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
 
; Setup Port 1 to output for Timer A0 and SMCLK 
; P1.1 = SMCLK -> P1DIR.1 = 1; P1SEL0.1 = 1; P1SEL1.1 = 1;
; P1.5 = TA0.2 -> P1DIR.5 = 1; P1SEL0.5 = 1; P1SEL1.5 = 1; 500Hz
; P1.6 = TA0.1 -> P1DIR.6 = 1; P1SEL0.6 = 1; P1SEL1.6 = 0; 50Hz
  
            bis.b   #BIT1 + BIT5 + BIT6,&P1SEL0 ;
            bis.b   #BIT1 + BIT5,&P1SEL1 ;
            bis.b   #BIT0 + BIT1 + BIT5 + BIT6,&P1DIR ;  
    
   ;************************* 
   ; Setup CCS                
   ; SMCLK = HFCLK/1  ~1MHz   
   ;************************* 
            mov.w    #CCSKEY,&CCSCTL0       ; Unlock CCS

check       mov.w    #0,&CCSCTL7            ; Clear HF & LF OSC fault flags
            mov.w    #0,&SFRIFG1            ; Clear OFIFG
            bit.w    #OFIFG,&SFRIFG1        ; Oscillator Flag(s)?
            jnz      check  
            mov.w    #SELS_0 + SELM_0,& CCSCTL4 ; Select HFCLK/DCO as the source for SMCLK   
            mov.w    #DIVS_0 + DIVM_0,&CCSCTL5  ; Set the Dividers for SMCLK to 1
            bis.b    #0xFF,&CCSCTL0_H       ;  Lock CCS
   
; Lock by writing to upper byte 
  
            mov.w    #OUTMOD_4 + CCIE,&TA0CCTL0 ; TA0CCR0 toggle, interrupt enabled
            mov.w    #OUTMOD_4 + CCIE,&TA0CCTL1 ; TA0CCR1 toggle, interrupt enabled
            mov.w    #OUTMOD_4 + CCIE,&TA0CCTL2 ; TA0CCR2 toggle, interrupt enabled
            mov.w    #TASSEL_2 +  MC_2 + TAIE,&TA0CTL ; SMCLK, Contmode, int enabled

            bis.w    #LPM0+GIE,SR           ; LPM0, enable interrupts
            nop                             ; Required only for debugger



; Timer0_A0 interrupt service routine
;-------------------------------------------------------------------------------
TA0_ISR                                     ; Toggle P1.0 Output
;-------------------------------------------------------------------------------
            add.w  #200,&TA0CCR0            ; Add Offset to TA0CCR0
            reti 
            reti                           

; Timer0_A1 Interrupt Vector (TAIV) handler

;-------------------------------------------------------------------------------
TAX_ISR;    Common ISR
;-------------------------------------------------------------------------------
            add.w   &TA0IV,PC               ; Add Timer_A offset vector
            add.w   #1000,&TA0CCR1          ; Add Offset to TA0CCR1
            reti                              
            add.w   #10000,&TA0CCR2         ; Add Offset to TA0CCR2
            reti                            ; 
            reti                            ; Reserve
            reti                            ; Reserve
            reti                            ; Reserve
            reti                            ; Reserve
            xor.b   #001h,&P1OUT            ; overflow -> Toggle P1.0
            reti                            ; Return from overflow ISR

;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int07"                ; TIMER0_A0_VECTOR
            .short  TA0_ISR                 ;  
            .sect	".int06"        		; Timer_AX Vector
            .short	TAX_ISR                 ;
            .end
