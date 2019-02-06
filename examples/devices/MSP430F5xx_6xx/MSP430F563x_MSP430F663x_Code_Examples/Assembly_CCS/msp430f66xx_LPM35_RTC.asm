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
;   MSP430F66x Demo - RTC_B, LPM3.5, & alarm
;
;   Description: The RTC_B module is used to set the time, start RTC operation,
;   and read the time from the respective RTC registers. Software will set the
;   original time to 11:59:45 am on Friday October 7, 2011. Then the RTC will
;   be activated through software, and an alarm will be created for the next 
;   minute (12:00:00 pm). The device will then enter LPM3.5 awaiting
;   the event interrupt. Upon being woken up by the event, the LED on the board
;   will be set. 
;
;   NOTE: This code example was created and tested using the MSP-TS430PZ100USB
;   Rev1.2 board. To ensure that LPM3.5 is entered properly, upon opening the 
;   debug window hit the "Run Free" button on the dropdown menu next to 
;   the run button. ~1.13uA should be measured on JP1. 
;
;  ;* An external watch crystal on XIN XOUT is required for ACLK *;	
;   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
;
;                MSP430F66xx
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |             P1.0|--> LED
;
;   Tyler Witt
;   Texas Instruments Inc.
;   October 2011
;   Built with Code Composer Studio V4.2
;******************************************************************************
 .cdecls C,LIST,"msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

Seconds     .set    R5
Minutes     .set    R6
Hours       .set    R7

;------------------------------------------------------------------------------
            .global _main
            .text
;------------------------------------------------------------------------------
_main
RESET       mov.w   #0x63FE,SP              ; Initialize stack pointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            cmp.w   #0x08,SYSRSTIV          ; was reset due to LPM wakeup?
            jne     Mainloop
            calla   #WkUpLPM35              ; Wakeup/reinit
            bis.b   #BIT0,&P1DIR            ; Ensure P1.0 output
            cmp.w   #0x12,Hours
            jne     LEDoff                  ; if Time != 12:00:00, blink fast
            cmp.w   #0x00,Minutes       
            jne     LEDoff                  ; if Time != 12:00:00, blink fast
            cmp.w   #0x00,Seconds       
            jne     LEDoff                  ; if Time != 12:00:00, blink fast
            jmp     LEDon                   ; if Time = 12:00:00, turn on LED
            
LEDoff      mov.w   #10000,R15
BlinkF      dec.w   R15
            jnz     BlinkF
            xor.b   #BIT0,&P1OUT
            jmp     LEDoff                  ; Again
LEDon       bis.b   #BIT0,&P1OUT
            jmp     LEDon                   ; Again

Mainloop    calla   #Board_Init
            calla   #RTC_Init
            calla   #EnterLPM35
            
            ; Code should not get here
            ; Blink LED slowly if LPM3.5 not entered properly
            calla   #WkUpLPM35
BadEntry    mov.w   #300000,R15
BlinkS      dec.w   R15
            jnz     BlinkS
            xor.b   #BIT0,&P1OUT
            jmp     BadEntry                ; Again
            nop                             ; for debugger
;------------------------------------------------------------------------------
Board_Init ; Init ports
;------------------------------------------------------------------------------
            mov.b   #0x00,&P1OUT            ; Port configuration
            mov.b   #0x00,&P2OUT
            mov.b   #0x00,&P3OUT
            mov.b   #0x00,&P4OUT
            mov.b   #0x00,&P5OUT
            mov.b   #0x00,&P6OUT
            mov.b   #0x00,&P7OUT
            mov.b   #0x00,&P8OUT
            mov.b   #0x00,&P9OUT
            mov.b   #0x00,&PJOUT
            mov.b   #0xFF,&P1DIR
            mov.b   #0xFF,&P2DIR
            mov.b   #0xFF,&P3DIR
            mov.b   #0xFF,&P4DIR
            mov.b   #0xFF,&P5DIR
            mov.b   #0xFF,&P6DIR
            mov.b   #0xFF,&P7DIR
            mov.b   #0xFF,&P8DIR
            mov.b   #0xFF,&P9DIR
            mov.b   #0xFF,&PJDIR
            
            mov.w   #0x9628,&USBKEYPID      ; Diable VUSB LDO and SLDO
            bic.w   #SLDOEN+VUSBEN,&USBPWRCTL
            mov.w   #0x9600,&USBKEYPID
            mov.w   #5000,R15               ; Delay to settle FLL
SetFLL      dec.w   R15
            jnz     SetFLL
            reta
;------------------------------------------------------------------------------
RTC_Init   ; Init RTC
;------------------------------------------------------------------------------
XT1Onb      bic.w   #XT1OFF,&UCSCTL6        ; XT1 On
            bis.w   #XCAP_3,UCSCTL6         ; Internal load cap
UnlkXT1b    bit.w   #LOCKBAK,&BAKCTL        ; Unlock XT1 pins for operation
            jz      XT1Chkb
            bic.w   #LOCKBAK,&BAKCTL
            jmp     UnlkXT1b
XT1Chkb     bic.w   #XT2OFFG+XT1LFOFFG+DCOFFG,&UCSCTL7
                                            ; Clear XT1,XT2,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscilator fault flag
            jnz     XT1Chkb                 ; If set, attempt to clear again
                                            ; If clear, continue
            
            bis.w   #RTCTEVIE+RTCBCD+RTCHOLD,&RTCCTL01 ; configure RTC_B: BCD mode,
                                                       ; RTC hold, RTC event interrupt
            mov.w   #0x2011,&RTCYEAR        ; Year = 0x2011
            mov.b   #0x10,&RTCMON           ; Month = 0x10 = October
            mov.b   #0x07,&RTCDAY           ; Day = 0x07 = 7th
            mov.b   #0x05,&RTCDOW           ; Day of week = 0x05 = Friday
            mov.b   #0x11,&RTCHOUR          ; Hour = 0x11
            mov.b   #0x59,&RTCMIN           ; Minute = 0x59
            mov.b   #0x45,&RTCSEC           ; Seconds = 0x45
            bis.b   #RTCTEV_0,&RTCCTL1      ; Set RTCTEV for 1 minute alarm
            
            bic.w   #RTCHOLD,&RTCCTL01      ; Start RTC calendar mode        

            bis.w   #XT1OFF,&UCSCTL6        ; Turn XT1 Off for entry into LPM3.5
            reta
;------------------------------------------------------------------------------
EnterLPM35 ; Turn off PMMREG and enter LPM3.5
;------------------------------------------------------------------------------
            EINT                            ; Enable interrupts
            mov.b   #PMMPW_H,&PMMCTL0_H     ; Open PMM
            bis.b   #PMMREGOFF,&PMMCTL0_L   ; Turn off PMMREG
            bis.w   #LPM4,SR                ; Enter LPM3.5
            nop
            reta
;------------------------------------------------------------------------------
WkUpLPM35 ;   Reinit Device after waking up from LPM3.5
;------------------------------------------------------------------------------
            mov.b   #PMMPW_H,&PMMCTL0_H     ; open PMM
            bic.w   #LOCKIO,&PM5CTL0        ; Lear LOCKBAK and enable ports
            clr.b   &PMMCTL0_H              ; close PMM
            
            mov.b   #0x00,&P1OUT            ; Port reconfiguration
            mov.b   #0x00,&P2OUT
            mov.b   #0x00,&P3OUT
            mov.b   #0x00,&P4OUT
            mov.b   #0x00,&P5OUT
            mov.b   #0x00,&P6OUT
            mov.b   #0x00,&P7OUT
            mov.b   #0x00,&P8OUT
            mov.b   #0x00,&P9OUT
            mov.b   #0x00,&PJOUT
            mov.b   #0xFF,&P1DIR
            mov.b   #0xFF,&P2DIR
            mov.b   #0xFF,&P3DIR
            mov.b   #0xFF,&P4DIR
            mov.b   #0xFF,&P5DIR
            mov.b   #0xFF,&P6DIR
            mov.b   #0xFF,&P7DIR
            mov.b   #0xFF,&P8DIR
            mov.b   #0xFF,&P9DIR
            mov.b   #0xFF,&PJDIR
            
XT1On       bic.w   #XT1OFF,&UCSCTL6        ; XT1 On
            bis.w   #XCAP_3,UCSCTL6         ; Internal load cap
UnlkXT1     bit.w   #LOCKBAK,&BAKCTL        ; Unlock XT1 pins for operation
            jz      XT1Chk
            bic.w   #LOCKBAK,&BAKCTL
            jmp     UnlkXT1
XT1Chk      bic.w   #XT2OFFG+XT1LFOFFG+DCOFFG,&UCSCTL7
                                            ; Clear XT1,XT2,DCO fault flags
            bic.w   #OFIFG,&SFRIFG1         ; Clear fault flags
            bit.w   #OFIFG,&SFRIFG1         ; Test oscilator fault flag
            jnz     XT1Chk                  ; If set, attempt to clear again
                                            ; If clear, continue
            ; Reconfig/start RTC and save Time
            bis.w   #RTCBCD+RTCHOLD,RTCCTL01
            mov.b   RTCSEC,Seconds
            mov.b   RTCMIN,Minutes
            mov.b   RTCHOUR,Hours  
            bic.w   #RTCHOLD,RTCCTL01          
            reta
;------------------------------------------------------------------------------
RTC_ISR ;   RTC Interrupt Service Routine
;------------------------------------------------------------------------------
            mov.b   #PMMPW_H,&PMMCTL0_H     ; open PMM
            bic.w   #LOCKIO,&PM5CTL0        ; Lear LOCKBAK and enable ports
            clr.b   &PMMCTL0_H              ; close PMM
            
            bic.b   #RTCTEVIFG,&RTCCTL0    ; Clear P1.4 IFG
            bic.w   #LPM4,0(SP)             ; Wakeup from LPM3.5
            nop
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".int42"                ; RTC_VECTOR
            .short  RTC_ISR
            .sect   ".reset"                ; MSP430 Reset Vector
            .short  RESET
            .end
