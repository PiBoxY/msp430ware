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
;*******************************************************************************
; MSP430i20xx Initialization Routines - low_level_init.asm
;
; The init function is called by certain code examples as the first action in
; the main code. The function affects the following modules:
;  - JTAG - JTAG is disabled
;  - TLV - A TLV checksum is performed
;  - PMM - The shared reference is calibrated to 1.16V
;  - Clock System - The DCO is calibrated to 16.384MHz
;  - SD24 - The SD24 reference voltage trimming is calibrated
;
; Any code example or application written for the MSP430i20xx which takes
; advantage of any of the affected modules is suggested to include this
; function to ensure full calibration of the application.
;
; Additionally, this initialization routine is suggested for small, short 
; code examples to ensure the JTAG will be unlocked. The i20xx series of 
; devices requires execution of 64 MCLK cycles before the JTAG can be unlocked.
; If a device does not execute 64 MCLK cycles, it cannot be accessed via JTAG.
;*******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            .def    init                    ; Export function
;-------------------------------------------------------------------------------
init        mov.w   #WDTPW|WDTCNTCL,&WDTCTL ; Feed the watchdog timer
            mov.w   #JTAG_DIS_PWD1,R15      ; Use R15 as pointer for JTAG PWD
            tst.w   0x0002(R15)             ; Test that JTAG PWD is valid
            jne     TestPWD                 ; != 0x00000000?
            tst.w   0x0000(R15)
            jeq     TestBORIFG
TestPWD     cmp.w   #-1,0x0002(R15)         ; != 0xFFFFFFFF?
            jne     DisJTAG
            cmp.w   #-1,0x0000(R15)
            jeq     TestBORIFG              ; Invalid PWD, skip to test BORIFG
DisJTAG     mov.w   #JTAGDISKEY,&SYSJTAGDIS ; Valid PWD, disable JTAG
TestBORIFG  bit.b   #BORIFG,&IFG1           ; test for BORIFG
            jeq     init_exit               ; BOR detected, exit init function
            clr.w   R15                     ; R15 is checksum
            mov.w   #TLV_START,R14          ; R14 points to TLV_Start
            mov.w   #TLV_START+2,R13        ; R13 points to TLV_Checksum
Checksum    xor.w   @R13+,R15               ; Add checksum value...
            cmp.w   #TLV_END,R13            ; ...until end of TLV
            jlo     Checksum
            inv.w   R15                     ; Checksum calculations
            inc.w   R15
            cmp.w   R15,&TLV_START          ; Check the result
            jeq     Calibrate               ; Success, calibrate TLV
            bis.w   #LPM4,SR                ; Fail, go to LPM4
Calibrate   mov.b   TLV_CAL_REFCAL1(R14),&REFCAL1   ; Calibrate REF
            mov.b   TLV_CAL_REFCAL0(R14),&REFCAL0
            mov.b   TLV_CAL_CSIRFCAL(R14),&CSIRFCAL ; Calibrate DCO (Internal Resistor)
            mov.b   TLV_CAL_CSIRTCAL(R14),&CSIRTCAL
            mov.b   TLV_CAL_CSERFCAL(R14),&CSERFCAL ; Calibrate DCO (External Resistor)
            mov.b   TLV_CAL_CSERTCAL(R14),&CSERTCAL
            mov.b   TLV_CAL_SD24TRIM(R14),&SD24TRIM ; Calibrate SD24 Trim
            bic.b   #BORIFG,&IFG1           ; Clear BORIFG
init_exit   mov.w   #WDTPW|WDTCNTCL,&WDTCTL ; Feed the watchdog timer
            ret                             ; Return
