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
;*******************************************************************************
;  MSP430FR231x Demo - ADC, Window Comparator, 1.5V ref, Timer trigger, DCO
;
;  Description: This example works on Repeat-Single-Channel Mode.
;  A1 is sampled in every 0.5s using the TB1.1 trigger.
;  Window comparator is used to generate interrupts to indicate when the input
;  voltage goes above the High_Threshold or below the Low_Threshold or
;  is inbetween the high and low thresholds. TimerB1 is used as an interval 
;  timer used to control the LED at P1.0 to toggle slow/fast or turn off
;  according to the ADC Hi/Lo/IN interupts.
;  ACLK = XT1 = 32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
;
;               MSP430FR2311
;            -----------------
;        /|\|                 |
;         | |                 | 
;         --|RST              |
;           |                 |
;       >---|P1.1/A1     P1.0 |--> LED 
;
;  
;   Darren Lu
;   Texas Instruments Inc.
;   Oct. 2015
;   Built with Code Composer Studio v6.1
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
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
RESET       mov.w   #__STACK_END,SP                         ; Initialize stack pointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL                  ; Stop WDT

SetupP1     bis.b   #BIT0,&P1DIR                            ; P1.0 output
            bic.b   #BIT0,&P1OUT                            ; Clear P1.0
            bis.b   #BIT1,&P1SEL0                           ; ADC A1 pin
            bis.b   #BIT1,&P1SEL1
            bis.b   #BIT6|BIT7,&P2SEL1                      ; P2.6~P2.7: crystal pins
            bic.w   #LOCKLPM5,PM5CTL0                       ; Unlock I/O pins
            
SetupXT1    mov.w   #SELA__XT1CLK,&CSCTL4                   ; Set ACLK = XT1; MCLK = SMCLK = DCO
OSCFlag     bic.w   #XT1OFFG+DCOFFG,&CSCTL7                 ; Clear XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1                         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1                         ; Test oscilator fault flag
            jnz     OSCFlag
 
SetupADC    bis.w   #ADCSHT_2|ADCMSC|ADCON,&ADCCTL0         ; ADCON
            bis.w   #ADCSHP|ADCSHS_2|ADCCONSEQ_2,&ADCCTL1   ; rpt single ch; TB1.1 trigger
            bis.w   #ADCRES,&ADCCTL2                        ; 10-bit conversion results
            bis.w   #ADCINCH_1|ADCSREF_1,&ADCMCTL0          ; Vref+, A1
            mov.w   #682,&ADCHI                             ; Window Comparator Hi-threshold ~2V
            mov.w   #341,&ADCLO                             ; Window Comparator Lo-threshold ~1V
            bis.w   #ADCHIIE|ADCLOIE|ADCINIE,&ADCIE         ; Enable ADC conv complete interrupt

SetupREF    mov.b   #PMMPW_H,&PMMCTL0_H                     ; Unlock the PMM registers
            bis.w   #INTREFEN,&PMMCTL2                      ; Enable internal reference
            mov.w   #200,R15                                ; Delay ~400 cycles for reference settling
L2          dec.w   R15                                     ; Decrement R15
            jnz     L2                                      ; Delay over?
            
SetupTB0    bis.w   #CCIE,&TB0CCTL0                         ; CCR0 interrupt enabled
            bis.w   #TBCLR|TBSSEL_1,&TB0CTL                 ; ACLK, clear TBR

SetupTB1    mov.w   #3FFFh,&TB1CCR0                         ; PWM Period
            mov.w   #1FFFh,&TB1CCR1                         ; Duty cycle TB1.1
            bis.w   #OUTMOD_4,&TB1CCTL1                     ; TB0CCR1 toggle
            bis.w   #TBSSEL_1|MC_1|TBCLR,&TB1CTL            ; ACLK, up mode
            
Mainloop    bis.w   #ADCENC,&ADCCTL0                        ; Start sampling/conversion
            nop
            bis.w   #LPM3+GIE,SR                            ; Enter LPM3 w/ interrupts
            nop

;-------------------------------------------------------------------------------
ADC_ISR;      ISR for ADC
;-------------------------------------------------------------------------------
            add     &ADCIV,PC                               ; Add offset to PC
            reti                                            ; No Interrupt
            reti                                            ; Conversion result overflow
            reti                                            ; Conversion time overflow
            jmp      ADHI                                   ; A1 > 1V
            jmp      ADLO                                   ; A1 < 0.5V
            jmp      ADIN                                   ; 0.5V < A1 < 1V
            reti                                            ; ADCIFG0

ADHI        bic.w    #ADCHIIFG,&ADCIFG                      ; Clear interrupt flag
            bic.w    #MC_1,&TB0CTL                          ; Turn off timer
            mov.w    #3E7h,&TB0CCR0                         ; Set timer period for fast toggle
            bis.w    #MC_1,&TB0CTL
            reti

ADLO        bic.w    #ADCLOIFG,&ADCIFG                      ; Clear interrupt flag
            bic.w    #MC_1,&TB0CTL                          ; Turn off timer
            mov.w    #4E1Fh,&TB0CCR0                        ; Set timer period for slow toggle
            bis.w    #MC_1,&TB0CTL
            reti

ADIN        bic.w    #ADCINIFG,&ADCIFG                      ; Clear interrupt flag
            bic.w    #MC_1,&TB0CTL                          ; Turn off timer
            bic.b    #BIT0,&P1OUT
            reti

;-------------------------------------------------------------------------------
TB0_ISR;    ISR for TIMER B0
;-------------------------------------------------------------------------------
            xor.b    #BIT0,&P1OUT                           ; Toggle LED P1.0
            reti 
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
           .sect   RESET_VECTOR                             ; MSP430 RESET Vector
           .short  RESET                                    ;
           .sect   ADC_VECTOR                               ; ADC Vector
           .short  ADC_ISR                                  ;
           .sect   TIMER0_B0_VECTOR                         ; Timer_B0 Vector
           .short  TB0_ISR                                  ;
           .end
