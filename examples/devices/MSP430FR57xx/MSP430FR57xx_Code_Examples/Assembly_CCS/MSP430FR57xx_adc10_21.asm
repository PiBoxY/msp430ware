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
;  MSP430FR57xx Demo - ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO
;
;  Description; A1 is sampled in every 0.5s using the TA0.1 trigger in single 
;  ch/conversion mode. Window comparator is used to generate interrupts to
;  indicate when the input voltage goes above the High_Threshold or below the 
;  Low_Threshold or is inbetween the high and low thresholds. TimerB0 is used 
;  as an interval timer used to control the LED at P1.0 to toggle slow/fast 
;  or turn off according to the ADC10 Hi/Lo/IN interupts.
;  ADC10 is configured in pulse sample mode and is triggered by the TA0.0
;
;               MSP430FR5739
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
;         --|RST          XOUT|-
;           |                 |
;       >---|P1.1/A1     P1.0 |--> LED 
;
;   Tyler Witt
;   Texas Instruments Inc.
;   September 2011
;   Built with Code Composer Studio V4.2
;*******************************************************************************
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
SetupXT1    bis.w   #030h,&PJSEL0
            mov.b   #0xA5,&CSCTL0_H
            bis.w   #DCOFSEL0+DCOFSEL1,&CSCTL1 ; Set max DCO setting
            mov.w   #SELA_0+SELS_3+SELM_3,&CSCTL2 ; set ACLK = XT1; MCLK = DCO
            mov.w   #DIVA_0+DIVS_3+DIVM_3,&CSCTL3 ; set all drivers
            bis.w   #XT1DRIVE_0,&CSCTL4
            bic.w   #XT1OFF,&CSCTL4
OSCFlag     bic.w   #XT1OFFG,&CSCTL5        ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            cmp.w   #001h,&OFIFG            ; Test oscillator fault flag
            jz      OSCFlag
SetupADC10  mov.w   #ADC10SHT_2+ADC10ON,&ADC10CTL0;
            mov.w   #ADC10SHP+ADC10SHS_1+ADC10CONSEQ_2,&ADC10CTL1   ; rpt single ch; TA0.1 trigger
            mov.w   #ADC10RES,&ADC10CTL2    ; 10-bit conversion results
            mov.w   #ADC10INCH_1+ADC10SREF_1,&ADC10MCTL0 ; Vref+, A1
            mov.w   #332h,&ADC10HI          ; Window Comparator Hi-threshold ~2V
            mov.w   #199h,&ADC10LO          ; Window Comparator Lo-threshold ~1V
SetVREF     cmp.w   #001h,&REFGENBUSY       ; Is ref gen busy?
            jz      SetVREF                 ; Yes, wait. No, set ref
            mov.w   #REFVSEL_2+REFON,&REFCTL0 ; Select internal ref = 2.5V
                                            ; Internal Reference ON
Delay       mov.w   #80,&TA0CCR0            ; Delay to allow Ref to settle
            bis.w   #CCIE,&TA0CCTL0         ; Compare-mode interrupt.
            mov.w   #MC_1+TASSEL_2,&TA0CTL  ; up mode, SMCLK
            nop
            bis.w   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
            nop
            bic.w   #CCIE,&TA0CCTL0          ; Disable timer interrupt
SetupP1     bis.b   #001h,&P1DIR            ; P1.0 output
            bic.b   #001h,&P1OUT            ; Clear P1.0
SetupTB0    mov.w   #CCIE,&TB0CCTL0         ; CCRO interrupt enabled
            mov.w   #TBSSEL_1+TBCLR,&TB0CTL ; ACLK, clear TBR
            mov.w   #3FFFh,&TA0CCR0         ; PWM Period
            mov.w   #1FFFh,&TA0CCR1         ; Dutycycle TA0.1
            bis.w   #OUTMOD_4,&TA0CCTL1     ; TA0CCR1 toggle
            mov.w   #TASSEL_1+MC_1+TACLR,&TA0CTL ; ACLK, up mode
            
Mainloop    mov.w   #ADC10HIIE+ADC10LOIE+ADC10INIE,&ADC10IE ; Interrupt enable
            bis.w   #ADC10ENC,&ADC10CTL0    ; Start sampling/conversion
            nop
            bis.w   #LPM3+GIE,SR            ; Enter LPM3 w/ interrupts
            nop

;-------------------------------------------------------------------------------
TA0_ISR;    ISR for TA0CCR0
;-------------------------------------------------------------------------------
            clr.w   &TA0CTL                 ; Clear Timer_A control registers
            bic.w   #LPM0,0(SP)             ; Exit LPM0, interrupts enabled
            reti                            ;
;-------------------------------------------------------------------------------
ADC10_ISR;  ISR for ADC10
;-------------------------------------------------------------------------------
            add     &ADC10IV,PC             ; Add offset to PC
            reti                            ; No Interrupt
            reti                            ; Conversion result overflow
            reti                            ; Conversion time overflow
            jmp     ADHI                    ; A1 > 2V
            jmp     ADLO                    ; A1 < 1V
            jmp     ADIN                    ; 1V < A1 < 2V
            reti                            ; ADC10IFG0
ADHI        bic.w   #ADC10HIIFG,&ADC10IFG   ; Clear interrupt flag
            bic.w   #MC_1,&TB0CTL           ; Turn off timer
            mov.w   #1F3h,&TB0CCR0          ; Set timer period for slow toggle
            bis.w   #MC_1,&TB0CTL
            reti
ADLO        bic.w   #ADC10LOIFG,&ADC10IFG   ; Clear interrupt flag
            bic.w   #MC_1,&TB0CTL           ; Turn off timer
            bic.b   #001h,&P1OUT
            reti                            ;
ADIN        bic.w   #ADC10INIFG,&ADC10IFG   ; Clear interrupt flag
            bic.w   #MC_1,&TB0CTL           ; Turn off timer
            mov.w   #270Fh,&TB0CCR0         ; Set timer period for slow toggle
            bis.w   #MC_1,&TB0CTL
            reti                            ;
;-------------------------------------------------------------------------------
TB0_ISR;    ISR for TIMER B0
;-------------------------------------------------------------------------------
            xor.b   #001h,&P1OUT            ; Toggle LED P1.0
            reti                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ADC10_VECTOR            ; ADC10 Vector
            .short  ADC10_ISR               ;
            .sect   TIMER0_A0_VECTOR        ; Timer_A0 Vector
            .short  TA0_ISR                 ;
            .sect   TIMER0_B0_VECTOR        ; Timer_B0 Vector
            .short  TB0_ISR                 ;
            .end
