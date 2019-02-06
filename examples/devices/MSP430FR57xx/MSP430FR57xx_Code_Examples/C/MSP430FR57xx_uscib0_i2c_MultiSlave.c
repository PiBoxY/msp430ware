/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR57xx Demo - USCI_B0 I2C 4 Hardware I2C slaves
//
//  Description: This demo connects two MSP430's via the I2C bus. 
//  This code configures the MSP430 USCI to be addressed as 4 independent I2C
//  slaves. Each slave has its owm interrupt flag and data variable to store
//  incoming data. 
//  Use with MSP430FR57xx_uscib0_i2c_Master_MultiSlave.c
//  ACLK = n/a, MCLK = SMCLK = default DCO = ~1.045MHz
//
//                                /|\  /|\
//                MSP430FR5739    10k  10k     MSP430FR5739
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
//            |                 |          |                 |
//            |                 |          |                 |
//
//   P. Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with CCS V4 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

#include <msp430.h>

unsigned char RXData0=0;  
unsigned char RXData1=0;
unsigned char RXData2=0;
unsigned char RXData3=0;

int main(void)

{
	
    WDTCTL = WDTPW + WDTHOLD;

    // Init SMCLK = MCLk = ACLK = 1MHz
    CSCTL0_H = 0xA5;
    CSCTL1 |= DCOFSEL0 + DCOFSEL1;          // Set max. DCO setting = 8MHz
    CSCTL2 = SELA_3 + SELS_3 + SELM_3;      // set ACLK = MCLK = DCO
    CSCTL3 = DIVA_3 + DIVS_3 + DIVM_3;      // set all dividers to 1MHz

    // Configure Pins for I2C
    P1SEL1 |= BIT6 + BIT7;                  // Pin init
    // eUSCI configuration
    UCB0CTLW0 |= UCSWRST ;	            //Software reset enabled
    UCB0CTLW0 |= UCMODE_3  + UCSYNC;	    //I2C mode, sync mode
    UCB0I2COA0 = 0x0A + UCOAEN;   	    //SLAVE0 own address is 0x0A+ enable
    UCB0I2COA1 = 0x0B + UCOAEN;   	    //SLAVE1 own address is 0x0B+ enable	
    UCB0I2COA2 = 0x0C + UCOAEN;   	    //SLAVE2 own address is 0x0C+ enable	
    UCB0I2COA3 = 0x0D + UCOAEN;   	    //SLAVE3 own address is 0x0D+ enable	

    UCB0CTLW0 &=~UCSWRST;	            //clear reset register
    UCB0IE |=  UCRXIE0 + UCRXIE1+ UCRXIE2 + UCRXIE3;//receive interrupt enable	
	
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
    __no_operation();	 
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR 
__interrupt void USCIB0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCIB0_ISR (void)
#else
#error Compiler not supported!
#endif

{

   switch(__even_in_range(UCB0IV,0x1E))
    {
      case 0x00: break;                     // Vector 0: No interrupts break;
      case 0x02: break;                     // Vector 2: ALIFG break;
      case 0x04: break;                     // Vector 4: NACKIFG break;
      case 0x06: break;                     // Vector 6: STTIFG break;
      case 0x08: break;                     // Vector 8: STPIFG break;
      case 0x0a:                            // SLAVE3
        RXData3 = UCB0RXBUF; 
        break;                              // Vector 10: RXIFG3 break;
      case 0x0c: break;                     // Vector 14: TXIFG3 break;
      case 0x0e:                            // SLAVE2
        RXData2 = UCB0RXBUF;
        break;                              // Vector 16: RXIFG2 break;
      case 0x10: break;                     // Vector 18: TXIFG2 break;
      case 0x12:                            // SLAVE1
        RXData1 = UCB0RXBUF;
        break;                              // Vector 20: RXIFG1 break;
      case 0x14: break;                     // Vector 22: TXIFG1 break;
      case 0x16:                            // SLAVE0
        RXData0 = UCB0RXBUF;                // Get RX data
        break;                              // Vector 24: RXIFG0 break;
      case 0x18: break;                     // Vector 26: TXIFG0 break;
      case 0x1a: break;                     // Vector 28: BCNTIFG break;
      case 0x1c: break;                     // Vector 30: clock low timeout break;
      case 0x1e: break;                     // Vector 32: 9th bit break;
      default: break;	
    }
	
}	


