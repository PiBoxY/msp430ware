//*****************************************************************************
//  MSP430AFE25x Demo - SD24, Single Conversion on a Group of Channels
//
//  Description: This program uses the SD24 module to perform a single
//  conversion on a group of channels (0 and 1). A SD24 interrupt occurs
//  when the conversions have completed. Test by applying voltages to the
//  two input channels and setting a breakpoint at the line indicated
//  below. Run program until it reaches the breakpoint, then use the
//  debugger's watch window to view the conversion results. Results for all
//  three channels are stored in the array "results". NOTE: array won't
//  contain values until after reaching breakpoint for the second time.
//  ACLK = n/a, MCLK = SMCLK = DCO =  ~ 1.1MHz
//  //* For minimum Vcc is required for SD24 module - see datasheet        *//
//  //* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
//
//               MSP430AFE25x
//            -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//   Vin1+ -->|A0.0+            |
//   Vin1- -->|A0.0-            |
//   Vin2+ -->|A1.0+            |
//   Vin2- -->|A1.0-            |
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

/* Array to store SD24 conversion results  */
unsigned int results[2];

void main(void)
{
  volatile unsigned int i;                  // Use volatile to prevent removal
                                            // by compiler optimization

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  SD24CTL = SD24REFON + SD24SSEL0;          // 1.2V ref, SMCLK
  SD24CCTL0 |= SD24GRP + SD24DF;            // Group with CH1
  SD24CCTL1 |= SD24IE + SD24DF;             // Enable interrupt
  for (i = 0; i < 0x3600; i++);             // Delay for 1.2V ref startup
  
  while (1)
  {
    SD24CCTL1 |= SD24SC;                    // SET BREAKPOINT HERE
                                            // Set bit to start conversion
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupts
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD24_VECTOR
__interrupt void SD24AISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD24_VECTOR))) SD24AISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (SD24IV)
  {
  case 2:                                   // SD24MEM Overflow
    break;
  case 4:                                   // SD24MEM0 IFG
    break;
  case 6:                                   // SD24MEM1 IFG
    results[0] = SD24MEM0;                  // Save CH0 results (clears IFG)
    results[1] = SD24MEM1;                  // Save CH1 results (clears IFG)
    break;
  case 8:                                   // SD24MEM2 IFG
    break;
  }

  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPM0
}
