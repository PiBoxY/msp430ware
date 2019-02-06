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
;  MSP430G2230 Demo - ADC10, Sample A10 Temp and Convert to oC and oF
;
;  Description: A single sample is made on A10 with reference to internal
;  1.5V Vref. Software sets ADC10SC to start sample and conversion - ADC10SC
;  automatically cleared at EOC. ADC10 internal oscillator/4 times sample
;  (64x) and conversion. In Mainloop MSP430 waits in LPM0 to save power until
;  ADC10 conversion complete, ADC10_ISR will force exit from any LPMx in
;  Mainloop on return. Temperaure in oC stored in memory address 0x200, oF in 0x202.
;  Uncalibrated temperature measured from device to device will vary with
;  slope and offset - please see datasheet.
;  ACLK = VLO, MCLK = SMCLK = default DCO ~1.2MHz, ADC10CLK = ADC10OSC/4
;
;                MSP430G2230
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |A10              |
;
;  B. Finch
;  Texas Instruments, Inc
;  May 2012
;  Built with CCS Version: 5.2.0.00069
;******************************************************************************

 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-------------------------------------------------------------------------------
            .global _main
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x280,SP               ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            mov.b   #0x1B,&P1REN            ; Terminate unavailable Port1 pins 
            bis.b   #LFXT1S_2,&BCSCTL3      ; Select VLO as low freqency clock

            mov.w   #INCH_10+ADC10DIV_3,&ADC10CTL1  ; Temp Sensor ADC10CLK/4
            mov.w   #SREF_1+ADC10SHT_3+REFON+ADC10ON+ADC10IE,&ADC10CTL0
            mov.w   #30,&TA0CCR0            ; Delay to allow Ref to settle
            bis.w   #CCIE,&TA0CCTL0         ; Compare-mode interrupt
            mov.w   #TASSEL_2+MC_1,&TA0CTL  ; TACLK = SMCLK, Up mode.
            bis.w   #LPM0+GIE,SR            ; Wait for delay
            bic.w   #CCIE,&TA0CCTL0         ; Disable timer Interrupt
            bic.w   #GIE,SR                 ; Disable interrupts

loop        bis.w   #ENC+ADC10SC,&ADC10CTL0 ; Sampling and conversion start
            bis.w   #CPUOFF+GIE,SR          ; LPM0 with interrupts enabled
            call    #TempC                  ; Transform voltage to temperature
            call    #BIN2BCD4               ; R13 = TempC = 0000 - 0145 BCD
            mov.w   R13,&0x0200             ; addr 0x200 = temp oC
            call    #TempF                  ; Transform voltage to temperature
            call    #BIN2BCD4               ; R13 = TempF = 0000 - 0292 BCD
            mov.w   R13,&0x0202             ; addr 0x202 = temp oF
            jmp     loop

;-------------------------------------------------------------------------------
TempC
;-------------------------------------------------------------------------------
            mov.w   &ADC10MEM,R12           ;
            mov.w   #423,R11                ; C
            call    #MULTIPLY               ;
            bic.w   #0x00FF,R14             ; /1024
            add.w   R15,R14                 ;
            swpb    R14                     ;
            rra.w   R14                     ;
            rra.w   R14                     ;
            mov.w   R14,R12                 ;
            sub.w   #278,R12                ; C
            ret                             ;

;-------------------------------------------------------------------------------
TempF
;-------------------------------------------------------------------------------
            mov.w   &ADC10MEM,R12           ;
            mov.w   #761,R11                ; F
            call    #MULTIPLY               ;
            bic.w   #0x00FF,R14             ; /1024
            add.w   R15,R14                 ;
            swpb    R14                     ;
            rra.w   R14                     ;
            rra.w   R14                     ;
            mov.w   R14,R12                 ;
            sub.w   #468,R12                ; F
            ret                             ;

;-------------------------------------------------------------------------------
BIN2BCD4
;-------------------------------------------------------------------------------
            mov.w   #16,R15                 ; Loop Counter
            clr.w   R13                     ; 0 -> RESULT LSD
BIN1        rla.w   R12                     ; Binary MSB to carry
            dadd.w  R13,R13                 ; RESULT x2 LSD
            dec.w   R15                     ; Through?
            jnz     BIN1                    ; Not through
            ret


;-------------------------------------------------------------------------------
MULTIPLY
;-------------------------------------------------------------------------------
            clr.w   R14                     ; 0 -> LSBs result
            clr.w   R15                     ; 0 -> MSBs result
MACU        clr.w   R13                     ; MSBs multiplier
            mov.w   #1,R10                  ; bit test register
MPY2        bit.w   R10,R11                 ; test actual bit
            jz      MPY1                    ; IF 0: do nothing
            add.w   R12,R14                 ; IF 1: add multiplier to result
            addc.w  R13,R15                 ;
MPY1        rla.w   R12                     ; multiplier x 2
            rlc.w   R13                     ;
            rla.w   R10                     ; next bit to test
            jnc     MPY2                    ; if bit in carry: finished
            ret                             ; Return from subroutine


;-------------------------------------------------------------------------------
ADC10_ISR
;-------------------------------------------------------------------------------
            bic.w   #CPUOFF,0(SP)           ; Exit active CPU
            reti                            ; Vector 0:  No interrupt


;-------------------------------------------------------------------------------
TIMERA0_ISR
;-------------------------------------------------------------------------------
            clr.w   &TACTL                  ; Clear Timer_A control registers
            bic.w   #CPUOFF,0(SP)           ; Exit active CPU
            reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .sect   ADC10_VECTOR            ; ADC10 isr vector
            .short  ADC10_ISR
            .sect   TIMERA0_VECTOR          ; Timer_A0 Vector
            .short  TIMERA0_ISR
            .end
