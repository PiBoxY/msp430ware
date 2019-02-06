//*****************************************************************************
//  MSP430AFE25x Demo - SD24, Using the Integrated Temperature Sensor
//
//  Description: This example shows how to use the SD24 module's integrated
//  temperature sensor (connected to input channel 6) to measure temperature.
//  A single conversion is initiated and then the CPU is disabled by entering
//  LPM0. Once the conversion has completed, a SD24 interrupt occurs and
//  stores the result in variable "ADCresult". Next, the CPU wakes up and
//  converts this value into two temperature values, one in degrees Celcius
//  and one in degrees Farenheit. Test by setting a breakpoint at the line
//  indicated below and running the program. Upon reaching the breakpoint,
//  use the debugger's watch window to view degrees C (DegC) and degrees F
//  (DegF).
//  ACLK = n/a, MCLK = SMCLK = DCO =  ~ 1.1MHz
//  //* Minimum Vcc is required for SD24 module - see datasheet        *//
//  //* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
//
//  Sensor's temperature coefficient is 1.32mV/K (from datasheet)
//  Sensor's offset voltage ranges from -100mv to 100mV (assume 0)
//  DegC =  DegK - 273
//  DegF = (DegC * 9/5) + 32
//  Vref = 1200mV
//  Vsensor = 1.32mV * DegK + Vsensor_offset (assuming 0mv for offset)
//  Vsensor = (SD24MEM2-32768)/32768 * Vref/2
//  DegK = (SD24MEM2 - 32768)/65536 * 1200/1.36
//  DegK = (SD24MEM2 * 909)/65536 - 454
//  DegC = (SD24MEM2 * 909)/65536 - 727   ( No need to simplify further,    )
//  DegF = (SD24MEM2 * 1636)/65536 - 1276 ( take advantage of HW multiplier )
//                                        ( and simple binary division      )
//
//                MSP430AFE25x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |A2.6+            | ( A2.6+, A2.6- connected internally )
//            |A2.6-            | ( to SD24's temperature sensor      )
//            |                 |
//            |            VREF |---+
//            |                 |   |
//            |                 |  -+- 100nF
//            |                 |  -+-
//            |                 |   |
//            |            AVss |---+
//            |                 |
//
//  K. Naveen/ K. Venkat
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//*****************************************************************************
#include <msp430.h>

unsigned int ADCresult;
unsigned long DegC = 0;
unsigned long DegF = 0;

void main(void)
{
  volatile unsigned int i;                  // Use volatile to prevent removal
                                            // by compiler optimization

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  SD24CTL = SD24REFON+SD24SSEL0;            // 1.2V ref, SMCLK
  SD24CCTL2 |= SD24SNGL+SD24IE ;            // Single conv, enable interrupt
  SD24INCTL2 |= SD24INCH_6;                 // Select Channel A6
  for (i = 0; i < 0x3600; i++);             // Delay for 1.2V ref startup

  while (1)
  {
    SD24CCTL2 |= SD24SC;                    // Set bit to start conversion
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupts

    DegC = ((unsigned long)ADCresult * 909)/65536 - 727;
                                            // Calculate degrees Celcius
    DegF = ((unsigned long)ADCresult * 1636)/65536 - 1276;
                                            // Calculate degrees Farenheit
    __no_operation();                       //SET BREAKPOINT HERE
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
    break;
  case 8:                                   // SD24MEM2 IFG
    ADCresult = SD24MEM2;                   // Save CH2 results (clears IFG)
    break;
  }

   __bic_SR_register_on_exit(LPM0_bits);    // Exit LPM0
}


