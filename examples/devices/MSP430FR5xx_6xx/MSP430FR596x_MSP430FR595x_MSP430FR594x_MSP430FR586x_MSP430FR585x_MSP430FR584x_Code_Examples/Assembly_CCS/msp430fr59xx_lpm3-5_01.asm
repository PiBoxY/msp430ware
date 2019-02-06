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
;   MSP430FR59xxx Demo - RTC_B, LPM3.5, & alarm
;
;   Description: The RTC_B module is used to set the time, start RTC operation,
;   and read the time from the respective RTC registers. Software will set the
;   original time to 11:59:30 am on Friday October 7, 2011. Then the RTC will
;   be activated through software, and an alarm will be created for the next
;   minute (12:00:00 pm). The device will then enter LPM3.5 awaiting
;   the event interrupt. Upon being woken up by the event, the LED on the board
;   will be set.
;
;   ACLK = 32.768kHz, MCLK = SMCLK = DCO = ~1MHz
;
;               MSP430FR59xx
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |             P1.0|--> LED
;
;   E. Chen
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio V5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT

            cmp.w   #SYSRSTIV_LPM5WU,SYSRSTIV ; Was reset due to LPMx.5 wakeup?
            jne     RegularRST
            calla   #WakeUpLPM35            ; LMPx.5 wakeup specific init code
            nop                             ; 
            EINT
            nop
Mainloop    jmp     Mainloop                ; Forever loop after returning from RTC ISR.
RegularRST  calla   #Board_Init
            calla   #RTC_Init
            calla   #EnterLPM35
            nop

;------------------------------------------------------------------------------
Board_Init ; Init ports and clock system
;------------------------------------------------------------------------------
SetupGPIO   mov.b   #0x00,&P1OUT            ; Port configuration
            mov.b   #0x00,&P2OUT
            mov.b   #0x00,&P3OUT
            mov.b   #0x00,&P4OUT
            mov.w   #0x0000,&PJOUT

            mov.b   #0xFF,&P1DIR
            mov.b   #0xFF,&P2DIR
            mov.b   #0xFF,&P3DIR
            mov.b   #0xFF,&P4DIR
            bis.w   #BIT4+BIT5,&PJSEL0
            mov.w   #0xFFFF,PJDIR

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

            mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #DCOFSEL_0,&CSCTL1      ; Set DCO to 1MHz
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2; Set ACLK = XT1; MCLK = DCO
            mov.w   #DIVA__1+DIVS__1+DIVM__1,&CSCTL3 ; Set all dividers to 1
            bic.w   #LFXTOFF,&CSCTL4        ; Enable LFXT1

OSCFlag     bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            clr.b   &CSCTL0_H               ; Lock CS registers
;------------------------------------------------------------------------------
RTC_Init   ; Init RTC
;------------------------------------------------------------------------------
SetupRTC    mov.w   #RTCBCD+RTCHOLD+RTCTEV__MIN+RTCTEVIE,&RTCCTL01 ; BCD mode, RTC hold, enable RTC
                                            ; event interrupt for each minute
            mov.w   #0x2011,&RTCYEAR        ; Year = 0x2011
            mov.b   #0x10,&RTCMON           ; Month = 0x10 = October
            mov.b   #0x07,&RTCDAY           ; Day = 0x07 = 7th
            mov.b   #0x05,&RTCDOW           ; Day of week = 0x05 = Friday
            mov.b   #0x11,&RTCHOUR          ; Hour = 0x11
            mov.b   #0x59,&RTCMIN           ; Minute = 0x59
            mov.b   #0x30,&RTCSEC           ; Seconds = 0x30

            bic.w   #RTCHOLD,&RTCCTL01      ; Start RTC calendar mode
;------------------------------------------------------------------------------
EnterLPM35 ; Turn off PMMREG and enter LPM3.5
;------------------------------------------------------------------------------
            mov.b   #PMMPW_H,&PMMCTL0_H
            mov.b   #PMMREGOFF,&PMMCTL0_L
            nop                             ; 
            bis.w   #LPM4+GIE,SR            ; Enter LPM3.5 mode w/ interrupts enabled
            nop
;------------------------------------------------------------------------------
WakeUpLPM35 ; Reinit Device after waking up from LPM3.5
;------------------------------------------------------------------------------
            mov.b   #0x00,&P1OUT            ; Port configuration
            mov.b   #0x00,&P2OUT
            mov.b   #0x00,&P3OUT
            mov.b   #0x00,&P4OUT
            mov.w   #0x0000,&PJOUT

            mov.b   #0xFF,&P1DIR
            mov.b   #0xFF,&P2DIR
            mov.b   #0xFF,&P3DIR
            mov.b   #0xFF,&P4DIR
            bis.w   #BIT4+BIT5,&PJSEL0
            mov.w   #0xFFFF,PJDIR

            bis.w   #RTCBCD+RTCHOLD+RTCTEV__MIN+RTCTEVIE,&RTCCTL01 ; BCD mode, RTC hold, enable RTC
                                            ; event interrupt for each minute
            bic.w   #RTCHOLD,&RTCCTL01

            bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

            mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #DCOFSEL_0,&CSCTL1      ; Set DCO to 1MHz
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2; Set ACLK = XT1; MCLK = DCO
            mov.w   #DIVA__1+DIVS__1+DIVM__1,&CSCTL3 ; Set all dividers to 1
            bic.w   #LFXTOFF,&CSCTL4        ; Enable LFXT1

            bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            clr.b   &CSCTL0_H               ; Lock CS registers
;------------------------------------------------------------------------------
RTC_ISR;    RTC Interrupt Service Routine
;------------------------------------------------------------------------------
            add.w   &RTCIV,PC               ; add offset to PC
            reti                            ; Vector  0:  No interrupt
            reti                            ; Vector  2:  RTC ready
            jmp     LEDon                   ; Vector  4:  RTC interval timer
            reti                            ; Vector  6:  RTC user alarm
            reti                            ; Vector  8:  RTC prescaler 0
            reti                            ; Vector 10:  RTC prescaler 1
            reti                            ; Vector 12:  RTC Oscillator fault
            reti                            ; Vector 14:  reserved
LEDon       bis.b   #BIT0,&P1OUT            ; Turn on LED
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect   RTC_VECTOR              ; RTC Vector
            .short  RTC_ISR
            .end
