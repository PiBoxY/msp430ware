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
;   MSP430x11x1 Demo - Comp_A, Slope ADC to Detect Temp Lvl, .set P1.0 > 25C
;
;   Description: Comparator_A is used to detect a resistance threshold.
;   Discharge times of a 0.1uf capacitor through a 10k-NTC (25C) and 10k ohm
;   reference resistor are compared. If the NTC discharge time is lower
;   than the 10k reference, P1.0 is set. If the NTC discharge time is higher
;   than reference, P1.0 is reset. The LED is ON if the temperature is greater
;   than 25C.
;   ACLK = n/a, MCLK = SMCLK = default DCO ~800k
;
;                MSP430F1121
;             -----------------
;        /|\ |              XIN|-
;         |  |                 |
;         ---|RST          XOUT|-
;            |                 |
;      +-10k-|P2.0             |
;      |     |                 |	
;      +-NTC-|P2.1             |
;      |     |                 |
;      +-----|P2.3         P1.0|-->LED
;      |     |                 |
;     ===.1uf|                 |
;      |     |                 |			
;      ------|VSS
;
;            NTC = 10k @25c, (P/N 271-110A Radio Shack)
;
Ref         .equ   001h                     ; P2.0 = Reference
Sensor      .equ   002h                     ; P2.1 = Sensor
;
;   M. Buccini / Z. Albus
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;*******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            bic.b   #001h,&P1OUT            ; P1.0 reset
            bis.b   #001h,&P1DIR            ; P1.0 output direction
            bic.b   #Sensor+Ref,&P2OUT      ; Sensor, Ref reset
            bis.b   #Sensor+Ref,&P2DIR      ; Sensor, Ref output
            mov.b   #008h, &CAPD            ; Disconnect port pins
            mov.b   #CARSEL+CAREF0+CAON,&CACTL1   ; -Comp = 0.25*Vcc -  on
            mov.b   #P2CA0,&CACTL2                ; +Comp = P2.3
            mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK, contmode
            eint                            ; Enable interrupts
                                            ;		
Mainloop    mov.b   #Sensor,R14             ; R14 = discharge pin
            call    #Measure                ;
            mov.w   R14,R15                 ; R15 = Sensor discharge time			
            mov.b   #Ref,R14                ; R14 = discharge pin
            call    #Measure                ;
            cmp.w   R14,R15                 ; R14 = Reference discharge time
            jlo     ON                      ; jump if R15 < R14 ( >25c )
;Display Modes                              ;
OFF         bic.b   #001h,&P1OUT            ; LED off - Reset P1.0
            jmp     Mainloop                ;
ON          bis.b   #001h,&P1OUT            ; LED on  - .set P1.0
            jmp     Mainloop                ;
                                            ;
;-------------------------------------------------------------------------------
Measure;    Input:   R14 = Sensor or Ref P2.x bit location
;           Output:  R14 = Sensor or Ref discharge time
;-------------------------------------------------------------------------------
Charge;     Capacitor is charged via Ref
            bis.b   #Ref,&P2OUT             ; Ref .set
            bis.b   #Ref,&P2DIR             ; Ref output
            mov.w   &TAR,&CCR1              ; CCR1 = TAR
            add.w   #5000,&CCR1             ; CCR1 ~ TAR+5ms (5tu)
            mov.w   #CCIE,&CCTL1            ; Comp,interrupt
            bis.w   #LPM0,SR                ; Wait for CCR1 interrupt				
            bic.b   #Ref,&P2DIR             ; Ref = HiZ, Charge complete
            bic.b   #Ref,&P2OUT             ; Ref = Reset						
Discharge;  Measure Discharge Time
            mov.w   #CM_2+CCIS_1+CAP+CCIE,&CCTL1    ; Neg, CCIB,Cap,interrupt
            push    &TAR                    ; TOS = TAR at SOC
            bis.b   R14,&P2DIR              ; Temp = Sensor or REF
            bis.w   #LPM0,SR                ; Wait for CCR1 interrupt				
            mov.w   &CCR1,R14               ; R14 = TAR (CCR1) at EOC
            sub.w   @SP+,R14                ; R14 = discharge time
            bic.b   #Sensor+Ref,&P2DIR      ; Disable Sensor or Ref
            clr.w   &CCTL1                  ; Disable CCTL1
            ret                             ;
                                            ;
;-------------------------------------------------------------------------------
TAX_ISR;    Common ISR for CCR1-4 and overflow
;-------------------------------------------------------------------------------
            add.w   &TAIV,PC                ; Add TA interrupt offset to PC
            reti                            ; CCR0 - no source
            jmp     CCR1_ISR                ; CCR1
;            reti                            ; CCR2
;            reti                            ; CCR3
;            reti                            ; CCR4
;TA_over     reti                            ; Timer_A overflow
                                            ;
CCR1_ISR    bic.w   #LPM0,0(SP)             ; Exit LPM0 on reti
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int08"                ; Timer_AX Vector
            .short  TAX_ISR                 ;
            .end
