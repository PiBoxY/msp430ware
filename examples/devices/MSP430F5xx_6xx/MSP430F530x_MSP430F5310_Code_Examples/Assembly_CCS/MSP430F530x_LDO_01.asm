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
;  MSP430F530x Demo - Internal LDO powering device VCC and PortU
;
;  Description: This example code shows the device can be automatically 
;  powered up by the internal LDO when sufficicnt is present at the LDOI pin 
;  and how the external connection between LDOO and VCC can be used to power 
;  the device VCC.
;    
;  Note: 
;   1.To power the MSP430 device VCC via the internal LDO, connect  LDOO to  
;     VCC externally. If using TI target board (MSP-TS430RGC64B), jumper pin 
;     header JP4 
;   2.Ensure current drawn by device VCC is < ILDOO spec
;
;  ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
;
;                MSP430F530x
;             -----------------
;        /|\|              VCC|<-
;         | |                 |  |(jumper JP4 on MSP-TS430RGC64B)
;         --|RST          LDOO|--
;           |                 |
;           |             LDOI|<-- LDO Input Voltage (3.76V to 5.5V - refer to 
;           |                 |    the d/s specs for actual range)
;           |                 |                              
;           |             PU.0|--> Toggle
;           |             PU.1|--> Toggle (complementary to PU.0)
;
;  K. Chen
;  Texas Instruments Inc.
;  March 2012
;  Built with CCS Version: 5.2 
;*******************************************************************************

 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
;-------------------------------------------------------------------------------
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop WDT

; Configure PU.0 as input pins
SetupPU     mov.w   #0x9628,&LDOKEYPID      ; Enable access to config registers
            bis.w   #PUOPE | PUOUT1,&PUCTL  ; PortU output enable
                                            ; PU.0 = low and PU.1 = high
            bis.w   #LDOOVLIE,&LDOPWRCTL    ; Enable LDO overload indication 
                                            ; interrupt                                            
            mov.w   #0,&LDOKEYPID           ; Disbale access to config registers

while_loop  mov.w   #50000,R4               ; continuous loop
delay       dec.w   R4                      ; Delay
            jnz     delay                   ; 
            mov.w   #0x9628,&LDOKEYPID      ; Enable access to config register
            xor.w   #PUOUT0 | PUOUT1,&PUCTL ; XOR PU.0/1
            mov.w   #0,&LDOKEYPID           ; Disbale access to config registers
            jmp     while_loop
            nop                             ; To workaround CPU40

;-------------------------------------------------------------------------------
LDO_ISR
;-------------------------------------------------------------------------------
            bit.w   #VUOVLIFG,&LDOPWRCTL    ;
            jz      IFG_RESET               ; 
            mov.w   #0x9628,&LDOKEYPID      ; Enable access to config registers
            bic.w   #VUOVLIFG,&LDOPWRCTL    ; Software clear IFG
            mov.w   #0,&LDOKEYPID           ; Disbale access to config registers
foreverloop jmp     foreverloop             ; Over load indication; take
                                            ; necessary steps in application
                                            ; firmware
            reti                            ;
IFG_RESET   reti                            ;            

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   LDO_PWR_VECTOR          ; LDO_PWR_VECTOR
            .short  LDO_ISR                 ;
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end
                         
