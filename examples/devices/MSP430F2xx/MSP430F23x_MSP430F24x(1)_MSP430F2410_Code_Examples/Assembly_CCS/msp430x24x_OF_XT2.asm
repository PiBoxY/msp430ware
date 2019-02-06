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
;   MSP430x24x Demo - XT2 Oscillator Fault Detection
;
;   Description: System runs normally in LPM3 with Timer A clocked by
;   ACLK.  Basic Timer interrupt causes an exit from LPM3, and toggles P1.0.
;   If an XT2 oscillator fault occurs, NMI is requested, pausing execution and
;   flashing LED quickly until fault is resolved.  Assumed only XT2 as NMI
;   source - code does not check for other NMI sources. MCLK is buffered on 
;   P5.4. If fault occurs for crystal osc sourcing the MCLK, MCLK is 
;   automatically switched to DCO for its clock source
;   MCLK = XT2 = 8MHz normally, within ISRs MCLK = DCO freq ~1045kHz
;   //* An external 8MHx crystal is required between XT2 and XT2OUT , and
;   an external 32kHz crystal is required between XIN and XOUT.  *//	
;
;                MSP430x249
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32KHz
;          --|RST          XOUT|-
;            |                 |
;            |            XT2IN|-
;            |                 | 8MHz
;            |           XT2OUT|-
;            |                 |
;            |             P1.0|-->LED
;            |        P5.4/MCLK|-->MCLK = XT2 HFXTAL
;
;  JL Bile
;  Texas Instruments Inc.
;  May 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0500h,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

SetupBC     bic.b   #XT2OFF,&BCSCTL1        ; Activate XT2 high freq xtal
            bis.b   #XT2S_2,&BCSCTL3        ; 3  16MHz crystal or resonator

SetupOsc    bic.b   #OFIFG,&IFG1            ; Clear OSC fault flag
            mov.w   #01FFFh,R15               ; R15 = Delay
SetupOsc1   dec.w   R15                     ; Additional delay to ensure start
            jnz     SetupOsc1               ;
            bit.b   #OFIFG,&IFG1            ; OSC fault flag set?
            jnz     SetupOsc                ; OSC Fault, clear flag again
                                           
            bis.b   #SELM_2, &BCSCTL2       ; MCLK = XT2 HF XTAL (safe)
                                            ;
SetupP1     bis.b   #010h,&P5DIR            ; P5.4 output direction
            bis.b   #010h,&P5SEL            ; P5.4 option select

SetupTimer  mov.w   #CCIE,&TACCTL0          ; One-second interrupt
            mov.w   #32767,&TACCR0
SetupTA     mov.w   #TASSEL_1+MC_2,&TACTL   ; SMCLK, continuous mode

            bis.b   #001h,&P1DIR            ; P1.0 = output direction
            bic.b   #001h, &P1OUT
; An immediate Osc Fault will occur next
            bis.b   #OFIE,&IE1              ; Enable osc fault interrupt

Mainloop    bis.w   #LPM0+GIE,SR            ; Enter LPM3, enable interrupts
            xor.b   #001h,&P1OUT            ; Toggle P1.0
            jmp     Mainloop                ;
                                            ;
;------------------------------------------------------------------------------
NMI_ISR;   Only osc fault enabled, R15 used temporarily and not saved
;          Assumed LFXT1 is only source for NMI interrupt
;------------------------------------------------------------------------------
CheckOsc    bic.b   #OFIFG,&IFG1            ; Clear OSC fault flag
            xor.b   #001h,&P1OUT            ; Toggle P1.0
            mov.w   #047FFh,R15             ; R15 = Delay
CheckOsc1   dec.w   R15                     ; Time for flag to set
            jnz     CheckOsc1               ;
            bit.b   #OFIFG,&IFG1            ; OSC fault flag set?
            jnz     CheckOsc                ; OSC Fault, clear flag again
            bis.b   #OFIE,&IE1              ; Re-enable osc fault interrupt
            reti                            ; Return from interrupt
                                            ;
;------------------------------------------------------------------------------
TA0_ISR;     Exit LPM3 on reti
;------------------------------------------------------------------------------
            bic.w   #LPM3,0(SP)             ;
            reti                            ;		
                                            ;
;-----------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".reset"            ; POR, ext. Reset
            .short    RESET              ;
            .sect     ".int25"          ; Timer_A0 Vector
            .short   TA0_ISR                 ;
            .sect   ".int30"              ; NMI vector
            .short   NMI_ISR                 ;
            .end
