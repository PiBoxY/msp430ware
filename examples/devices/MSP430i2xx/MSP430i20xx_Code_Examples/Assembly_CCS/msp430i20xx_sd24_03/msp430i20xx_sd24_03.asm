; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2013, Texas Instruments Incorporated
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
;  MSP430i20xx Demo - Continuous Conversion on a Single Channel
;
;  Description: This program uses the SD24 module to perform continuous
;  conversions on a single channel. A SD24 interrupt occurs when a conversion
;    has completed.
;
;    Test by applying voltages to the input channel and setting a breakpoint
;    at the indicated line. Run program until it reaches the breakpoint.
;  Results (upper 16 bits only) are stored in RAM at 0x200 and can be viewed
;  in the Memory Browser window within the debugger.
;
;  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz, SD_CLK = 1.024MHz
;  * Ensure low_level_init.asm is included when building/running this example *
;
;  Notes: For minimum Vcc required for SD24 module - see datasheet
;          100nF cap btw Vref and AVss is recommended when using 1.2V ref
;
;               MSP430i20xx
;             -----------------
;         /|\|                |
;          | |                |
;          --|RST             |
;            |                |
;   Vin1+ -->|A0.0+      VREF |---+
;   Vin1- -->|A0.0-           |   |
;            |                |  -+- 100nF
;            |                |  -+-
;            |                |   |
;            |           AVss |---+
;
;   T. Witt
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio v5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
Ch0result   .set 0x200                      ; Ch 0 result
Num_Results .set 8                          ; Number of Results
index       .set R7                         ; Index for results
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

            .ref    init                    ; Reference external function

RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
Cal         call    #init                   ; Calibrate peripherals
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT
            mov.w   #SD24REFS,&SD24CTL      ; Internal reference
            bis.w   #SD24IE|SD24DF,&SD24CCTL0
            mov.w   #0x428,R15              ; Delay for ref to settle
Delay       dec.w   R15                     ; ~200us total
            jne     Delay
            bis.w   #SD24SC,&SD24CCTL0      ; Start conversion

Mainloop    bis.w   #LPM0|GIE,SR            ; Enter LPM0 with interrupts
            jmp     Mainloop

;-------------------------------------------------------------------------------
SD24_ISR    ; ISR for SD24
;-------------------------------------------------------------------------------
            add     &SD24IV,PC              ; Determine interrupt source
            reti                            ; No interrupt
            reti                            ; SD24OVIFG
            jmp     SD24_Ch0                ; SD24MEM0
            reti                            ; SD24MEM1
            reti                            ; SD24MEM2
            reti                            ; SD24MEM3
SD24_Ch0    mov.w   index,R15               ; Conversion completed
            rla.w   R15
            mov.w   &SD24MEM0,Ch0result(R15); Save Ch0 Results
            inc.w   index                   ; Increment index
            cmp.w   #Num_Results,index      ; 8 Results Saved?
            jne     Exit_ISR                ; No, continue
            clr.w   index                   ; Yes, clear index
            nop                             ; SET BREAKPOINT HERE
Exit_ISR    reti

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   ".int07"                ; SD24 ISR
            .short  SD24_ISR
            .end
