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
;  MSP430FR59xx Demo - ADC12, Window Comparator, 2.5V ref, 2's Complement
;
;  Description; A1 is sampled in single ch/ single conversion mode.
;  Window comparator is used to generate interrupts to
;  indicate when the input voltage goes above the High_Threshold or below the
;  Low_Threshold or is in between the high and low thresholds. TimerB0 is used
;  as an interval timer used to control the LED at P1.0 to toggle slow/fast
;  or turn off according to the ADC12 Hi/Lo/IN interupts.
;  NOTE: This example shows how to write to the ADC threshold registers when
;  2's complement mode is used.
;
;               MSP430FR5969
;            -----------------
;        /|\|              XIN|-
;         | |                 | 32kHz
;         --|RST          XOUT|-
;           |                 |
;       >---|P1.1/A1     P1.0 |--> LED
;
;   E. Chen
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio V5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupGPIO   bic.b   #BIT0,&P1OUT            ; Clear P1.0
            bis.b   #BIT0,&P1DIR            ; Set P1.0 output direction
            bis.b   #BIT1,&P1SEL1           ; Configure ADC P1.1/A1
            bis.b   #BIT1,&P1SEL0
            bis.w   #BIT4+BIT5,&PJSEL0      ; For XT1

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

SetupCS     mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #DCOFSEL_3,&CSCTL1      ; Set DCO to 8MHz
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2
            mov.w   #DIVA__1+DIVS__8+DIVM__8,&CSCTL3  ; MCLK = SMCLK = 1MHz
            bic.w   #LFXTOFF,&CSCTL4        ; Enable LFXT1

OSCFlag     bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            clr.b   &CSCTL0_H               ; Lock CS registers

SetupADC12  mov.w   #ADC12SHT0_2+ADC12ON,&ADC12CTL0 ; Sampling time, ADC12 on
            mov.w   #ADC12SHS_0+ADC12SSEL_0+ADC12CONSEQ_0+ADC12SHP,&ADC12CTL1
            bis.w   #ADC12DF,&ADC12CTL2     ; 2's complement
            mov.w   #ADC12INCH_1+ADC12VRSEL_1+ADC12WINC,&ADC12MCTL0 ; Channel 1
                                            ; reference = internal
                                            ;enable window comparator
            mov.w   #0x4CC0,&ADC12HI        ; Set thresholds for ADC12 interrupts
            mov.w   #0xE670,&ADC12LO
            mov.w   #ADC12HIIE+ADC12LOIE+ADC12INIE,&ADC12IER2

SetVREF     bit.w   #REFGENBUSY,&REFCTL0    ; Is ref gen busy?
            jnz     SetVREF                 ; Yes, wait. No, set ref
            bis.w   #REFVSEL_2+REFON,&REFCTL0 ; Select internal ref = 2.5V
                                            ; Internal Reference ON
REFSettle   bit.w   #REFGENRDY,&REFCTL0     ; Wait for reference generator
            jz      REFSettle               ; to settle
                                            ;
SetupTB0    mov.w   #CCIE,&TB0CCTL0         ; CCR0 interrupt enabled
            mov.w   #TBSSEL__ACLK+TBCLR,&TB0CTL ; ACLK, clear TBR

Mainloop    bic.w   #CCIE,&TB0CCTL0         ; CCR0 interrupt disabled
            bis.w   #ADC12ENC+ADC12SC,&ADC12CTL0 ; Start sampling/conversion
            nop                             ; 
            bis.w   #LPM0+GIE,SR            ; Enter LPM0 w/ interrupts
            nop                             ; for debugger
            jmp     Mainloop

;-------------------------------------------------------------------------------
ADC12_ISR;  ADC12 interrupt service routine
;-------------------------------------------------------------------------------
            add.w   &ADC12IV,PC             ; add offset to PC
            reti                            ; Vector  0:  No interrupt
            reti                            ; Vector  2:  ADC12MEMx Overflow
            reti                            ; Vector  4:  Conversion time overflow
            jmp     ADHI                    ; Vector  6:  ADC12HI
            jmp     ADLO                    ; Vector  8:  ADC12LO
            jmp     ADIN                    ; Vector 10:  ADC12IN
            reti                            ; Vector 12:  ADC12MEM0 Interrupt
            reti                            ; Vector 14:  ADC12MEM1
            reti                            ; Vector 16:  ADC12MEM2
            reti                            ; Vector 18:  ADC12MEM3
            reti                            ; Vector 20:  ADC12MEM4
            reti                            ; Vector 22:  ADC12MEM5
            reti                            ; Vector 24:  ADC12MEM6
            reti                            ; Vector 26:  ADC12MEM7
            reti                            ; Vector 28:  ADC12MEM8
            reti                            ; Vector 30:  ADC12MEM9
            reti                            ; Vector 32:  ADC12MEM10
            reti                            ; Vector 34:  ADC12MEM11
            reti                            ; Vector 36:  ADC12MEM12
            reti                            ; Vector 38:  ADC12MEM13
            reti                            ; Vector 40:  ADC12MEM14
            reti                            ; Vector 42:  ADC12MEM15
            reti                            ; Vector 44:  ADC12MEM16
            reti                            ; Vector 46:  ADC12MEM17
            reti                            ; Vector 48:  ADC12MEM18
            reti                            ; Vector 50:  ADC12MEM19
            reti                            ; Vector 52:  ADC12MEM20
            reti                            ; Vector 54:  ADC12MEM21
            reti                            ; Vector 56:  ADC12MEM22
            reti                            ; Vector 58:  ADC12MEM23
            reti                            ; Vector 60:  ADC12MEM24
            reti                            ; Vector 62:  ADC12MEM25
            reti                            ; Vector 64:  ADC12MEM26
            reti                            ; Vector 66:  ADC12MEM27
            reti                            ; Vector 68:  ADC12MEM28
            reti                            ; Vector 70:  ADC12MEM29
            reti                            ; Vector 72:  ADC12MEM30
            reti                            ; Vector 74:  ADC12MEM31
            reti                            ; Vector 76:  ADC12RDY
ADHI        bic.w   #ADC12HIIFG,&ADC12IFGR2 ; Clear interrupt flag
            bic.w   #MC__UP,&TB0CTL         ; Turn off timer
            mov.w   #1000-1,&TB0CCR0        ; Set timer period for slow toggle
            bis.w   #MC__UP,&TB0CTL
            bis.w   #CCIE,&TB0CCTL0         ; CCR0 interrupt enabled
            bic.w   #LPM0,0(SP)             ; Exit LPM0
            reti
ADLO        bic.w   #ADC12LOIFG,&ADC12IFGR2 ; Clear interrupt flag
            bic.w   #MC__UP,&TB0CTL         ; Turn off timer
            bic.b   #BIT0,&P1OUT            ; Turn off LED
            bic.w   #LPM0,0(SP)             ; Exit LPM0
            reti                            ;
ADIN        bic.w   #ADC12INIFG,&ADC12IFGR2 ; Clear interrupt flag
            bic.w   #MC__UP,&TB0CTL         ; Turn off timer
            mov.w   #20000-1,&TB0CCR0       ; Set timer period for slow toggle
            bis.w   #MC__UP,&TB0CTL
            bis.w   #CCIE,&TB0CCTL0         ; CCR0 interrupt enabled
            bic.w   #LPM0,0(SP)             ; Exit LPM0
            reti
;-------------------------------------------------------------------------------
TIMER0_B0_ISR;    Timer0_B7 CC0 Interrupt Service Routine
;-------------------------------------------------------------------------------
            xor.b   #BIT0,&P1OUT            ; Toggle LED P1.0
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ADC12_VECTOR            ; ADC12 Vector
            .short  ADC12_ISR               ;
            .sect   TIMER0_B0_VECTOR        ; Timer0_B7 CC0 Interrupt Vector
            .short  TIMER0_B0_ISR           ;
            .end
