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
;  MSP430F54xA Demo - RTC_A in real time clock mode
;
;  Description: This program demonstrates the RTC mode by triggering an
;  interrupt every second and minute. This code toggles P1.0 every second.
;  This code recommends an external LFXT1 crystal for RTC accuracy.
;  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;
;                MSP430F54xA
;             -----------------
;        /|\ |              XIN|-
;         |  |                 | 32kHz
;         ---|RST          XOUT|-
;            |                 |
;            |            P1.0 |--> Toggles every second
;            |                 |
;
;  W. Goh
;  Texas Instruments Inc.
;  April 2010
;  Built with CCS version 4.2
;*******************************************************************************

    		.cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
            .global _main

;-------------------------------------------------------------------------------
            .text                           ; Assemble to Flash memory
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x5C00,SP              ; Initialize SP
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Hold WDT
            bis.b   #0x01,&P1DIR            ; P1.0 Output

            bis.b   #0x03,&P7SEL            ; Select XT1
            bic.w   #XT1OFF,&UCSCTL6        ; XT1 On
            bis.w   #XCAP_3,&UCSCTL6        ; Internal load cap

            ; Loop until XT1,XT2 & DCO stabilizes
do_while    bic.w   #XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG,&UCSCTL7
                                            ; Clear XT2,XT1,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jc      do_while

            ; Configure RTC_A
            bis.w   #RTCTEVIE + RTCRDYIE + RTCBCD + RTCHOLD + RTCMODE,&RTCCTL01
                                            ; RTC enable, BCD mode, RTC hold
                                            ; enable RTC read ready interrupt
                                            ; enable RTC time event interrupt

            mov.w   #0x2010,&RTCYEAR        ; Year = 0x2010
            mov.b   #0x4,&RTCMON            ; Month = 0x04 = April
            mov.b   #0x05,&RTCDAY           ; Day = 0x05 = 5th
            mov.b   #0x01,&RTCDOW           ; Day of week = 0x01 = Monday
            mov.b   #0x10,&RTCHOUR          ; Hour = 0x10
            mov.b   #0x32,&RTCMIN           ; Minute = 0x32
            mov.b   #0x45,&RTCSEC           ; Seconds = 0x45

            mov.w   #0x2,&RTCADOWDAY        ; RTC Day of week alarm = 0x2
            mov.b   #0x20,&RTCADAY          ; RTC Day Alarm = 0x20
            mov.b   #0x10,&RTCAHOUR         ; RTC Hour Alarm
            mov.b   #0x23,&RTCAMIN          ; RTC Minute Alarm

            bic.w   #RTCHOLD,&RTCCTL01      ; Start RTC calendar mode

            bis.b   #LPM3 + GIE,SR          ; Enter LPM3 w/ interrupt
            nop
;-------------------------------------------------------------------------------
RTC_ISR ;   RTC Interrupt Handler
;-------------------------------------------------------------------------------
            add     &RTCIV,PC
            reti                            ; No interrupts
            jmp     RTCRDYIFG_HND           ; RTCRDYIFG
            jmp     RTCEVIFG_HND            ; RTCEVIFG
            reti                            ; RTCAIFG
            reti                            ; RT0PSIFG
            reti                            ; RT1PSIFG
            reti                            ; Reserved
            reti                            ; Reserved
            reti                            ; Reserved
            reti
RTCRDYIFG_HND
            xor.b   #BIT0,&P1OUT            ; Toggle P1.0 every second
            reti
RTCEVIFG_HND
            nop                             ; Interrupts every minute
            reti
;-------------------------------------------------------------------------------
            ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int41"                ; RTC Vector
            .short  RTC_ISR
            .sect   ".reset"                ; POR, ext. Reset
            .short  RESET
            .end
            
