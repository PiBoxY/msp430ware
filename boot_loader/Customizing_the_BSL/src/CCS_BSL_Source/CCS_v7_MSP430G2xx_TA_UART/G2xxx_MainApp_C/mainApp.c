/*******************************************************************************
;   Example of an Main Application to be used in conjunction with G2xxx_BSL
;
;                MSP430G2xxx
;             -----------------
;         /|\|                 |
;          | |        P1.0/P1.6|---> LED for MAIN application
;          --|RST              |
;            |                 |
;            |      BSLPIN/P1.3|<--- Enable Pin / S2 on LaunchPad
;            |                 |
;            |                 |
;            |          rX/P1.1|<--- UART 9600 8N1
;            |          tX/P1.2|---> rx/tx swapped on LaunchPad!
;             -----------------
;
;
;   Texas Instruments
;   Built with CCS 4.2.3 and IAR 5.20.1
;
;*******************************************************************************
;  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;    Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;******************************************************************************/


#include <msp430.h>

//#define LED   BIT0                    // RED LED
#define LED   BIT6                      // GREEN LED

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  P1DIR |= LED;                         // Set P1.LED to output direction

  for (;;)
  {
    volatile unsigned int repCnt, delay;// volatile to prevent optimization


    repCnt = 4;                         // Rep Count
    while(repCnt--){
    	P1OUT ^= LED;                   // Toggle P1.LED using exclusive-OR
    	delay = 0x0FFFF;               // Delay to R15
    	while(delay){
    		delay--;
    		delay++;
    		delay--;
    	}
    }

    repCnt = 10;                        // Rep Count
    while(repCnt--){
    	P1OUT ^= LED;                   // Toggle P1.LED using exclusive-OR
    	delay = 0x0AFFF;                // Delay to R15
    	while(delay){
    		delay--;
    		delay++;
    		delay--;
    	}
    }

    repCnt = 20;                        // Rep Count
    while(repCnt--){
    	P1OUT ^= LED;                   // Toggle P1.LED using exclusive-OR
    	delay = 0x057FF;                // Delay to R15
    	while(delay){
    		delay--;
    		delay++;
    		delay--;
    	}
    }

    repCnt = 40;                        // Rep Count
    while(repCnt--){
    	P1OUT ^= LED;                   // Toggle P1.LED using exclusive-OR
    	delay = 0x02BFF;                // Delay to R15
    	while(delay){
    		delay--;
    		delay++;
    		delay--;
    	}
    }
  }
}
