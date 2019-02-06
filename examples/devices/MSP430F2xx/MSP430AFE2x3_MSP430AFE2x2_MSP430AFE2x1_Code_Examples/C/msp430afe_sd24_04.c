//*****************************************************************************
//  MSP430AFE25x Demo - SD24, Single Conversion on Single Channel Polling IFG
//
//  Description: This program uses the SD24 module to perform a single
//  conversion on a single channel. The SD24 interrupt flag for CH2 is polled
//  to determine when a conversion has completed. Test by applying a voltage
//  to CH2 (A2.0+, A2.0-) and setting a breakpoint at the line indicated
//  below. Run program until it reaches the breakpoint, then use the
//  debugger's register window to view the conversion results. The conversion
//  result for CH2 is stored in R12.
//  ACLK = n/a, MCLK = SMCLK = DCO =  ~ 1.1MHz
//  //* For minimum Vcc required for SD24 module - see datasheet        *//
//  //* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
//
//               MSP430AFE25x
//            -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//    Vin+ -->|A2.0+            |
//    Vin- -->|A2.0-            |
//            |                 |
//            |            VREF |---+
//            |                 |   |
//            |                 |  -+- 100nF
//            |                 |  -+-
//            |                 |   |
//            |            AVss |---+
//            |                 |
//
//  K. Naveen
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//*****************************************************************************
#include <msp430.h>

unsigned int results;

void main(void)
{
  volatile unsigned int i;                  // Use volatile to prevent removal
                                            // by compiler optimization

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
                                           
  SD24CTL = SD24REFON + SD24SSEL0;          // 1.2V ref, SMCLK
  SD24CCTL2 |= SD24SNGL;                    // Single conv
  for (i = 0; i < 0x3600; i++);             // Delay for 1.2V ref startup

  while (1)
  {
    SD24CCTL2 |= SD24SC;                    // Set bit to start conversion
    while ((SD24CCTL2 & SD24IFG)==0);       // Poll interrupt flag for CH2
    results = SD24MEM2;                     // Save CH2 results (clears IFG)
    __no_operation();                       // SET BREAKPOINT HERE
  }
}
