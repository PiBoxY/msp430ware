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
;   MSP-FET430P410 Demo - SD16, Using the Integrated Temperature Sensor
;
;   Description: This example shows how to use the SD16 module's integrated
;   temperature sensor (connected to input channel 6) to measure temperature.
;   A single conversion is initiated and then the CPU is disabled by entering
;   LPM0. Once the conversion has completed, a SD16 interrupt occurs and
;   stores the result in register R5. Next, the CPU wakes up and converts
;   this value into two temperature values, one in degrees Celcius and one
;   in degrees Farenheit. Test by setting a breakpoint at the line indicated
;   below and running the program. Upon reaching the breakpoint, use the
;   debugger's register window to view degrees C in register R11 and
;   degrees F in register R9. Temperatures stored in R9 and R11 are in
;   BCD format.
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   ;* An external watch crystal between XIN & XOUT is required for ACLK *//	
;   ;* Minimum Vcc is required for SD16 module - see datasheet        *//
;   ;* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
;
;   Sensor's temperature coefficient is 1.32mV/K (from 'F42x datasheet)
;   Sensor's offset voltage ranges from -100mv to 100mV (assume 0)
;   DegC =  DegK - 273
;   DegF = (DegC * 9/5) + 32
;   Vref = 1200mV
;   Vsensor = 1.32mV * DegK + Vsensor_offset (assuming 0mv for offset)
;   Vsensor = (SD16MEM2-32768)/32768 * Vref/2
;   DegK = (SD16MEM2 - 32768)/65536 * 1200/1.36
;   DegK = (SD16MEM2 * 909)/65536 - 454
;   DegC = (SD16MEM2 * 909)/65536 - 727   ( No need to simplify further,    )
;   DegF = (SD16MEM2 * 1636)/65536 - 1276 ( take advantage of HW multiplier )
;                                         ( and simple binary division      )
;
;                 MSP430F427
;             -----------------
;          /|\|              XIN|-
;           | |                 | 32kHz
;           --|RST          XOUT|-
;             |                 |
;             |A2.6+            | ( A2.6+, A2.6- connected internally )
;             |A2.6-            | ( to SD16's temperature sensor      )
;             |                 |
;             |            VREF |---+
;             |                 |   |
;             |                 |  -+- 100nF
;             |                 |  -+-
;             |                 |   |
;             |            AVss |---+
;             |                 |
;
;   H. Grewal / S. Karthikeyan
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-----------CPU Registers Used-------------------------------------------------
;           R5 - SD12 CH2 conversion result
;           R6 - Temporary working register
;           R7 - Temporary working register
;           R8 - Temporary working register
;           R9 - Degrees Farenheit
;           R10 - Temporary working register
;           R11 - Degrees Centigrade
;           R15 - Temporary working register

;------------------------------------------------------------------------------
            .text                  ; Program Start
;------------------------------------------------------------------------------
RESET       mov     #600h,SP                ; Initialize stack pointer
StopWDT     mov     #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
            mov     #10000,R15              ;
Xtal_Wait   dec     R15                     ; Delay for 32 kHz crystal to
            jnz     Xtal_Wait		        ; stabilize
SetupSD16   mov     #SD16REFON+SD16SSEL0,&SD16CTL
                                            ; 1.2V ref, SMCLK
            bis     #SD16SNGL+SD16IE,&SD16CCTL2
                                            ; Single conv, enable interrupt
            bis.b   #SD16INCH_6,&SD16INCTL2 ; Select channel A2.6 connected to
                                            ; internal temperature sensor
            mov     #03600h,R15             ; Delay needed for 1.2V ref startup
L$1         dec     R15                     ;
            jnz     L$1                     ;
            eint                            ; Enable global interrupts
Mainloop    bis     #SD16SC,&SD16CCTL2      ; Start conversion
            bis     #CPUOFF,SR              ; Enter LPM0 (disable CPU), wait
                                            ; for conversion to complete
Cmode       mov.w   R5,&MPY                 ; Multiply SD16 result by 909
            mov.w   #038Dh,&OP2             ; Load 909 in OP2 register
            mov.w   &RESHI,R11              ; Use R11 and disregard R10
            mov.w   &RESLO,R10              ; (end result is 8-bit right shift)
            sub.w   #02D7h,R11              ; Subtract 727 from R11
bin2bcdC                                    ; Convert binary to BCD
            mov     #16,R15                 ; Load loop counter
            clr     R6                      ; Clear working register R6
            clr     R7                      ; Clear working register R7
L$2         rla     R11                     ; Left-shift R11
            dadd    R7,R7                   ; Decimally add src+dst+C
            dadd    R6,R6                   ; Decimally add src+dst+C
            dec     R15                     ; Decrement loop counter
            jnz     L$2                     ; Is loop counter = 0?
            mov     R7,R11                  ; Save DegC in BCD format in R11
Fmode       mov.w   R5,&MPY                 ; Multiply SD16 result by 1636
            mov.w   #0664h,&OP2             ; Load 1636 in OP2 register
            mov.w   &RESHI,R9               ; Use R9 and disregard R8
            mov.w   &RESLO,R8               ; (end result is 8-bit right shift)
            sub.w   #04FCh,R9               ; Subtract 1276 from R9
bin2bcdF
            mov     #16,R15                 ; Load loop counter
            clr     R6                      ; Clear working register R6
            clr     R7                      ; Clear working register R7
L$3         rla     R9                      ; Left-shift R11
            dadd    R7,R7                   ; Decimally add src+dst+C
            dadd    R6,R6                   ; Decimally add src+dst+C
            dec     R15                     ; Decrement loop counter
            jnz     L$3                     ; Is loop counter = 0?
            mov     R7,R9                   ; Save DegF in BCD format in R9
            jmp     Mainloop                ; set BREAKPOINT HERE
                                            ;
;------------------------------------------------------------------------------
SD16_ISR    ; SD16 Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &SD16IV,PC              ; Add offset to PC
            reti                            ; Vector 0: No interrupt
            jmp     SD_OV                   ; Vector 2: Overflow
            jmp     SD_CH0                  ; Vector 4: CH0 IFG
            jmp     SD_CH1                  ; Vector 6: CH1 IFG
                                            ; Vector 8: CH2 IFG
                                            ;
;-----------SD16 Channel 2 Interrupt Handler-----------------------------------
SD_CH2      mov     &SD16MEM2,R5            ; Save CH2 conversion result
            bic     #CPUOFF,0(SP)           ; Return active
SD_CH2_END  reti                            ; Return from interrupt
                                            ;
;-----------SD16 Memory Overflow Interrupt Handler-----------------------------
SD_OV       reti                            ; Return from interrupt
                                            ;
;-----------SD16 Channel 0 Interrupt Handler-----------------------------------
SD_CH0      reti                            ; Return from interrupt
                                            ;
;-----------SD16 Channel 1 Interrupt Handler-----------------------------------
SD_CH1      reti                            ; Return from interrupt
                                            ;
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .sect   ".int12"                ; SD16 Vector"
            .short  SD16_ISR                ;
            .end
