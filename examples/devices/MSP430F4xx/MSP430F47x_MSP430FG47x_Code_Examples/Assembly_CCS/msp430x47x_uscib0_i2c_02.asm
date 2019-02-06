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
;   MSP430x47x Demo - USCI_B0 I2C Master Interface to PCF8574, Read/Write
;
;   Description: I2C communication with a PCF8574 in read and write mode is
;   demonstrated. PCF8574 port P is configured with P0-P3 input, P4-P7. Read
;   P0-P3 input data is written back to Port P4-P7. This example uses the
;   RX ISR and generates an I2C restart condition while switching from
;   master receiver to master transmitter.
;   ACLK = 32kHz, MCLK = SMCLK = TACLK = BRCLK = 1MHz
;
;                               MSP430x47x
;                              -----------------
;                  /|\ /|\ /|\|              XIN|-
;                  10k 10k  | |                 | 32kHz
;       PCF8574     |   |   --|RST          XOUT|-
;       ---------   |   |     |                 |
;  --->|P0    SDA|<-|---+---->|P3.1/UCB0SDA     |
;  --->|P1       |  |         |                 |
;  --->|P2       |  |         |                 |
;  --->|P3    SCL|<-+---------|P3.2/UCB0SCL     |
;  <---|P4       |            |                 |
;  <---|P5       |            |                 |
;  <---|P6       |            |                 |
;  <---|P7       |            |                 |
;   +--|A0,A1,A2 |            |                 |
;   |  |         |            |                 |
;  \|/
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
;-------------------------------------------------------------------------------
			.text							; Program Start
;-------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer
SetupP3     bis.b   #BIT1+BIT2,&P3SEL       ; Assign P3.1 and P3.2 to I2C 
SetupP4     bis.b   #BIT1, &P5DIR
SetupUCB0   bis.b   #UCSWRST,&UCB0CTL1      ; Enable SW reset
            mov.b   #UCMST+UCMODE_3+UCSYNC,&UCB0CTL0
                                            ; I2C Master, synchronous mode
            mov.b   #UCSSEL_2+UCSWRST,&UCB0CTL1
                                            ; Use SMCLK, keep SW reset
            mov.b   #11,&UCB0BR0            ; fSCL = SMCLK/11 = 95.3kHz
            mov.b   #00,&UCB0BR1
            mov.w   #020h,&UCB0I2CSA        ; Set slave address
            bic.b   #UCSWRST,&UCB0CTL1      ; Clear SW reset, resume operation
            bis.b   #UCB0RXIE,&IE2          ; Enable RX interrupt
SetupTA     mov.w   #CCIE,&TACCTL0          ; TACCR0 interrupt enabled
            mov.w   #TASSEL_2+MC_2,&TACTL   ; SMCLK, contmode

Mainloop    bis.w   #CPUOFF+GIE,SR          ; CPU off, interrupts enabled
            bic.b   #UCTR,&UCB0CTL1         ; I2C RX
            bis.b   #UCTXSTT,&UCB0CTL1      ; I2C start condition
Mainloop_1  bit.b   #UCTXSTT,&UCB0CTL1      ; Loop until I2C STT is sent
            jc      Mainloop_1
            bis.b   #UCTR+UCTXSTT,&UCB0CTL1 ; I2C TX, start condition
            bis.w   #CPUOFF+GIE,SR          ; CPU off, interrupts enabled
Mainloop_2
            bit.b   #UCTXSTT,&UCB0CTL1      ; Loop until I2C STT is sent
            jc      Mainloop_2
            bis.b   #UCTXSTP,&UCB0CTL1      ; I2C stop condition after 1st TX
            jmp     Mainloop                ; Repeat
;-------------------------------------------------------------------------------
TA0_ISR;
;-------------------------------------------------------------------------------
            xor.b   #BIT1, &P5OUT
            bic.w   #LPM0,0(SP)           ; Exit LPM0
            reti
;-------------------------------------------------------------------------------
USCIAB0TX_ISR;      USCI_B0 Data ISR
;-------------------------------------------------------------------------------
            mov.b   &UCB0RXBUF,R4           ; Move RX data
            rlc.b   R4                      ; Rotate left 4 bits
            rlc.b   R4
            rlc.b   R4
            rlc.b   R4
            bis.b   #0fh,R4                 ; Isolate lower nibble
            mov.b   R4,&UCB0TXBUF           ; Move TX data
            bic.w   #LPM0,0(SP)           ; Exit LPM0
            reti
;------------------------------------------------------------------------------
;			Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".int06"          ; Timer_A0 Vector
            .short  TA0_ISR
            .sect   ".int08"          ; USCI_B0 I2C Data Int Vector
            .short  USCIAB0TX_ISR
            .sect   ".reset"          ; POR, ext. Reset, Watchdog
            .short  RESET
            .end
