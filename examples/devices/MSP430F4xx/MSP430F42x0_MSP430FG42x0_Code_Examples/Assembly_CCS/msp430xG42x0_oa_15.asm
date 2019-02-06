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
;*****************************************************************************
;  MSP430xG42x0 Demo - OA0, Offset Calibration
;
;  Description: Configure OA0 for measuring offset calibration. The DAC0 
;  module is used to provide a reference input to the non-inverting
;  terminal of the op-amp. The reference is also given to the negative input of  
;  the SD16 module (internaly using the OACAL bit). The output of the OA is 
;  connected internally to the positive input of the SD16. The differential 
;  voltage measured by the SD16 gives the offset value of the OA. 
;  ACLK = 32.768kHz, MCLK = SMCLK = default DCO 
;  ACLK = 32.768kHz, MCLK = SMCLK = default DCO
;
;                MSP430xG42x0
;             -------------------
;         /|\|                XIN|-
;          | |                   |
;          --|RST            XOUT|-
;            |                   |
;            |                   |
;            |                   |
;            |          SD16MEM0 |--> OA0 Offset voltage
;            |                   |
;
;  JL Bile
;  Texas Instruments Inc.
;  June 2008
;  Built Code Composer Essentials: v3 FET
;*******************************************************************************
 .cdecls C,LIST, "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
 
RESULTS	.equ R12
;-------------------------------------------------------------------------------
            .text                 ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #300h,SP                ; Initialize stackpointer

StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

DAC12_0     mov.w   #DAC12OPS+ DAC12IR + DAC12AMP_2 + DAC12ENC,&DAC12_0CTL
                                            ; 1x input range, enable DAC120,
                                            ; low speed/current
            mov.w   #0545h,&DAC12_0DAT      ; DAC120 ref. voltage ~ 1V
            
SetupOA     mov.b   #OAPM_1+OAP_2,&OA0CTL0  ; Select inputs, power mode
            mov.b   #OAFC_1+OACAL,&OA0CTL1  ; Mode, offset calib. enabled

SetupSD16   mov.w   #SD16REFON+SD16SSEL0,&SD16CTL ; 1.2V ref, SMCLK
            bis.w   #SD16SNGL+SD16DF+SD16BUF_2,&SD16CCTL0; Single conversion,
                                            ; 2s complement mode, buffer on
            bis.b   #01h,&SD16AE            ; Analog input enable                               
            mov.w   #03600h,R15             ; Delay needed for 1.2V ref startup
L$1         dec.w   R15                     ;
            jnz     L$1                     ;

Mainloop    bis.w   #SD16SC,&SD16CCTL0      ; Start conversion
testIFG     bit.w   #SD16IFG,&SD16CCTL0     ; Is conversion done?
            jz      testIFG                 ; No, test again
            mov.w   &SD16MEM0,RESULTS       ; Yes, save conversion result
                                            ; (automatically clears SD16IFG)
            jmp     Mainloop                ; SET BREAKPOINT HERE

;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect	".reset"            ; MSP430 RESET Vector
            .short   RESET                   ;
            .end
   
