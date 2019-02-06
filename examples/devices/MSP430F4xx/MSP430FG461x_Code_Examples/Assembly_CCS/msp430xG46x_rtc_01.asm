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
;   MSP430xG461x Demo - Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK
;
;   Description: This program toggles P5.1 by xor'ing P5.1 inside of
;   a Real Time Clock ISR. The Real Time Clock ISR is called once a minute using
;   the Alarm function provided by the RTC. ACLK used to clock basic timer.
;   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;   //* An external watch crystal between XIN & XOUT is required for ACLK *//
;
;                MSP430FG4619
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |             P5.1|-->LED
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
;-------------------------------------------------------------------------------
			.text	;Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #900,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupFLL    bis.b   #XCAP14PF,&FLL_CTL0     ; Configure load caps
SetupRTC    mov.b   #RTCBCD+RTCHOLD+RTCMODE_3+RTCTEV_0+RTCIE,&RTCCTL
                                            ; RTC enable, BCD mode,
                                            ; alarm every Minute,
                                            ; enable RTC interrupt
            ; Init time
            mov.b   #000h,&RTCSEC           ; Set Seconds
            mov.b   #000h,&RTCMIN           ; Set Minutes
            mov.b   #008h,&RTCHOUR          ; Set Hours

            ; Init date
            mov.b   #002h,&RTCDOW           ; Set DOW
            mov.b   #023h,&RTCDAY           ; Set Day
            mov.b   #008h,&RTCMON           ; Set Month
            mov.w   #02005h,&RTCYEAR        ; Set Year


            bic.b   #RTCHOLD,&RTCCTL        ; Enable RTC
SetupP5     bis.b   #002h,&P5DIR            ; P5.1 output

Mainloop    bis.w   #LPM3+GIE,SR            ; Enter LPM3, enable interrupts
            nop                             ; Required only for debugger
                                            ;
;-------------------------------------------------------------------------------
BT_ISR;     Toggle P5.1
;-------------------------------------------------------------------------------
            xor.b   #002h,&P5OUT            ; Toggle P5.1
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
;			Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect	".int16"       ; Basic Timer Vector
            .short   BT_ISR                  ;
            .sect	".reset"            	; POR, ext. Reset, Watchdog
            .short   RESET
            .end