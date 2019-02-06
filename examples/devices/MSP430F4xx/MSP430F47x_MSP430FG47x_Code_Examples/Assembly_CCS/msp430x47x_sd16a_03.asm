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
;    MSP430x47x Demo - SD16_A, Continuous Conversion on a Single Channel
;
;   Description: This program uses the SD16_A module to perform continuous
;   conversions on a single channel. A SD16_A interrupt occurs whenever a
;   conversion has completed. Test by applying a voltage to CH0
;   (A0+, A0-) and setting a breakpoint at the "nop" instruction as
;   indicated below. Run program until it reaches the breakpoint, then
;   use the debugger's memory window to view the conversion results.
;   Results (upper 16 bits only) for CH0 are stored
;   at RAM addresses 0x200 - 0x20E.
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   //* An external watch crystal between XIN & XOUT is required for ACLK *//	
;   //* Minimum Vcc is required for SD16_A module - see datasheet        *//
;   //* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
;
;                MSP430x47x
;              -----------------
;          /|\|              XIN|-
;           | |                 | 32kHz
;           --|RST          XOUT|-
;             |                 |
;     Vin+ -->|A0+              |
;     Vin- -->|A0-              |
;             |                 |
;             |            VREF |---+
;             |                 |   |
;             |                 |  -+- 100nF
;             |                 |  -+-
;             |                 |   |
;             |            AVss |---+
;             |                 |
;
;   P.Thanigai
;   Texas Instruments Inc.
;   September 2008
;   Built with Code Composer Essentials Version: 3.0
;******************************************************************************
 .cdecls C,LIST, "msp430.h" 
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

;-----------RAM Variables------------------------------------------------------
Results     .equ     0200h                   ; Coversion results table

;-----------CPU Registers Used-------------------------------------------------
;           R5 - Table index
;           R15 - Temporary working register

;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP                ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
Port6Con    bis.b   #BIT0+BIT1,&P6SEL        ; Configure SD16 pins

SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
            mov.w   #10000,R15              ;
Xtal_Wait   dec.w   R15                     ; Delay for 32 kHz crystal to
            jnz     Xtal_Wait		        ; stabilize

SetupSD16   mov.w   #SD16REFON+SD16SSEL0,&SD16CTL  ; 1.2V ref, SMCLK
            bis.w   #SD16IE,&SD16CCTL0      ; Continuous conv, enable interrupt
            bis.b   #SD16INTDLY_0,&SD16INCTL0  ; 4th sample generates int.
            clr.w   R5                      ; Initialize table index
            mov.w   #03600h,R15             ; Delay needed for 1.2V ref startup
L$1         dec.w   R15                     ;
            jnz     L$1                     ;

Mainloop    bis.w   #SD16SC,&SD16CCTL0      ; Start conversion
            bis.w   #CPUOFF+GIE,SR          ; Enter LPM0 (disable CPU)
                                            ;
;------------------------------------------------------------------------------
SD16A_ISR    ; SD16_A Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &SD16IV,PC              ; Add offset to PC
            reti                            ; Vector 0: No interrupt
            jmp     SD_OV                   ; Vector 2: Overflow
            jmp     SD_CH0                  ; Vector 4: IFG
                                            ;
;-----------SD16_A Interrupt Handler-------------------------------------------
SD_CH0      mov.w   &SD16MEM0,Results(R5)   ; Move result, IFG is reset
            incd    R5                      ; Increment results table index
            and     #0Eh,R5                 ; Modulo pointer
            cmp.w   #0000h,R5               ; Does index = 0?
            jne     SD_CH0_END              ; No, return from interrupt
            nop                             ; SET BREAKPOINT HERE
SD_CH0_END  reti                            ; Return from interrupt
                                            ;
;-----------SD16_A Memory Overflow Interrupt Handler---------------------------
SD_OV       reti                            ; Return from interrupt

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;  
            .sect   ".int07"            ; SD16 Vector
            .short  SD16A_ISR               ;            
            .end

