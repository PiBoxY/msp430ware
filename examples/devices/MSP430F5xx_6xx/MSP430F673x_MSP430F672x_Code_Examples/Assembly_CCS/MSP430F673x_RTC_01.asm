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
;  MSP430F673x Demo - RTC_C, Calendar Mode with Time Event and Alarm Interrupts
;
;  Description: This program demonstrates the RTC_C mode by triggering an
;  interrupt every second and minute. This code toggles P1.0 every second.
;  This code recommends an external LFXT1 crystal for RTC accuracy.
;  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
;
;                MSP430F673x
;             -----------------
;        /|\ |              XIN|-
;         |  |                 | 32kHz
;         ---|RST          XOUT|-
;            |                 |
;            |            P1.0 |--> Toggles every second
;            |                 |
;
;   W. Goh
;   Texas Instruments Inc.
;   January 2011
;   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
;******************************************************************************
            .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
Seconds     .equ    R5
Minutes     .equ    R6
Hours       .equ    R7
;-------------------------------------------------------------------------------
            .global _main
;-------------------------------------------------------------------------------
_main
RESET       mov.w   #0x3C00,SP              ; Initialize stackpointer
            mov.w   #WDTPW | WDTHOLD,&WDTCTL; Stop WDT
XT1On       bic.w   #XT1OFF,&UCSCTL6        ; XT1 On
            bis.w   #XCAP_3,UCSCTL6         ; Internal load cap
XT1Chk      bic.w   #XT2OFFG | XT1LFOFFG | DCOFFG,&UCSCTL7
                                            ; Clear XT1,XT2,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscilator fault flag
            jnz     XT1Chk                  ; If set, attempt to clear again
                                            ; If clear, continue
            bic.b   #BIT0,&P1OUT            ; Clear LED to start
            bis.b   #BIT0,&P1DIR            ; P1.0 Output

            mov.b   #RTCKEY_H,&RTCCTL0_H    ; Unlock RTC_C module
            bis.b   #RTCTEVIE | RTCAIE | RTCRDYIE,&RTCCTL0_L
                                            ; Enable RTC time event, alarm event,
                                            ; read ready interrupt
            bis.b   #RTCBCD | RTCHOLD,&RTCCTL1
                                            ; RTC enable BCD mode, RTC hold

            mov.w   #0x2011,&RTCYEAR        ; Year = 0x2011
            mov.b   #0x12,&RTCMON           ; Month = 0x12 = December
            mov.b   #0x05,&RTCDAY           ; Day = 0x05 = 5th
            mov.b   #0x03,&RTCDOW           ; Day of week = 0x03 = Wednesday
            mov.b   #0x24,&RTCHOUR          ; Hour = 0x12
            mov.b   #0x59,&RTCMIN           ; Minute = 0x57
            mov.b   #0x55,&RTCSEC           ; Seconds = 0x36

            mov.w   #0x03,&RTCADOWDAY       ; RTC Day of week alarm = 0x2
            mov.b   #0x22,&RTCADAY          ; RTC Day Alarm = 0x22
            mov.b   #0x23,&RTCAHOUR         ; RTC Hour Alarm
            mov.b   #0x45,&RTCAMIN          ; RTC Minute Alarm

            bic.b   #RTCHOLD,&RTCCTL1       ; Start RTC calendar mode
            mov.b   #0x00,&RTCCTL0_H        ; Lock RTC_C module

Mainloop    bis.w   #LPM3 | GIE,SR          ; Enter LPM3 w/ interrupts
            nop                             ;
;------------------------------------------------------------------------------
RTC_ISR ;   RTC Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &RTCIV,PC
            reti                            ; Vector  0: No interrupt
            reti                            ; Vector  2: RTCOFIFG
            jmp     RTCReady                ; Vector  4: RTCRDYIFG
            jmp     RTCEvIfg                ; Vector  6: RTCEVIFG
            jmp     RtcAifg                 ; Vector  8: RTCAIFG
            reti                            ; Vector 10: RT0PSIFG
            reti                            ; Vector 12: RT1PSIFG
            reti                            ; Vector 14: Reserved
            reti                            ; Vector 16: Reserved
RTCReady    xor.b   #BIT0,P1OUT             ; Toggle LED every second
            mov.b   RTCSEC,Seconds          ; Read all associated time registers
            mov.b   RTCMIN,Minutes
            mov.b   RTCHOUR,Hours
            reti
RTCEvIfg    nop
            reti
RtcAifg     nop
            reti
;-------------------------------------------------------------------------------
            ; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   RESET_VECTOR            ; POR, ext. Reset
            .short  RESET
            .sect   RTC_VECTOR              ; RTC Interrupt Vector
            .short  RTC_ISR
            .end
