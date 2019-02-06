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
;  MSP430FR57xx Demo - USCI_A0 External Loopback test
;
;  Description: This demo connects TX to RX of the MSP430 UART 
;  The example code shows proper initialization of registers 
;  and interrupts to receive and transmit data.
;  ACLK = BRCLK = XT1 = 32.768Khz, MCLK = SMCLK = default DCO = ~1MHz
;
;                                
;                MSP430FR5739 
;                       
;             -----------------   
;       RST -|     P2.0/UCA0TXD|----|
;       |----|XIN              |    |
;      32kHz-|                 |    |
;       |----|XOUT P2.1/UCA0RXD|----|
;            |                 |    
;
;   F.  Chen
;   Texas Instruments Inc.
;   November 2012
;   Built with Code Composer Studio V5.2.1
;******************************************************************************
 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
i           .set    R5
RXData      .set    R6
TXData      .set    R7
check       .set    R8
;------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment known
            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs
;------------------------------------------------------------------------------
_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
            bis.b   #BIT4+BIT5,&PJSEL0
            mov.b   #0xA5,&CSCTL0_H
            bis.w   #DCOFSEL0+DCOFSEL1,&CSCTL1 ; Set max DCO setting
            mov.w   #SELA_0+SELS_3+SELM_3,&CSCTL2 ; ACLK = XT1; MCLK = DCO
            mov.w   #DIVA_0+DIVS_3+DIVM_3,&CSCTL3 ; set all dividers
            bis.w   #XT1DRIVE_0,&CSCTL4
            bic.w   #XT1OFF,&CSCTL4
OSCFlag     bic.w   #XT1OFFG,&CSCTL5            ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            cmp.w   #001h,&OFIFG            ; Test oscillator fault flag
            jz      OSCFlag
            
            clr.w   RXData                  ; Initialize variables
            clr.w   TXData
            clr.w   check
            bis.b   #BIT0+BIT1,&P2SEL1      ; Configure UART pins
            bic.b   #BIT0+BIT1,&P2SEL0
            bis.w   #UCSWRST,&UCA0CTL1      ; Configure UART 0
            mov.w   #UCSSEL_1,&UCA0CTL1     ; Set ACLK = 32768 as UCBRCLK
            mov.b   #3,&UCA0BR0             ; 9600 baud
            bis.w   #0x5300,&UCA0MCTLW      ; 32768/9600 - INT(32768/9600)= 0.41
                                            ; UCBRSx value = 0x53 (See UG)
            clr.b   &UCA0BR1
            bic.w   #UCSWRST,&UCA0CTL1      ; release from reset
            bis.w   #UCRXIE,&UCA0IE         ; enable RX interrupt
            nop
            bis.w   #GIE,SR                 ; Enable interrupt   
            nop         
Mainloop    inc.w   TXData                  ; increment TX data
            mov.w   TXData,&UCA0TXBUF       ; Load data onto buffer
Wait        cmp.b   #0x1,check
            jnz     Wait
            clr.w   check
            jmp     Mainloop
            nop                             ; remain in LPM0
            
;------------------------------------------------------------------------------
USCI_ISR ;  USCI Interrupt Service Routine
;------------------------------------------------------------------------------
            bic.w   #UCRXIFG,&UCA0IFG       ; Clear interrupt
            mov.w   &UCA0RXBUF,RXData       ; Clear buffer
Compare     cmp.w   TXData,RXData           ; Check value
            jnz     Compare
            mov.w   #1,check        
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   USCI_A0_VECTOR          ; USCI_A0_VECTOR
            .short  USCI_ISR                ;
            .end
