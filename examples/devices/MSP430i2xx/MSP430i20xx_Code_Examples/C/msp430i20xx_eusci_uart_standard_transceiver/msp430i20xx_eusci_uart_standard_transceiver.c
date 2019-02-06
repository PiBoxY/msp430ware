//******************************************************************************
//   MSP430i20xx Demo - eUSCI_A0, UART Echo received character
//                     (ACLK 9600/SMCLK 9600/SMCLK 115200)
//
//   Description: The device will wait in LPM0/LPM3 (based on clock source)
//   until a UART character is received.
//   Then the device will echo the received character.
//   The UART can operate using ACLK at 9600, SMCLK at 115200 or SMCLK at 9600.
//   To configure the UART mode, change the following line:
//
//      #define UART_MODE       SMCLK_115200
//      to any of:
//      #define UART_MODE       SMCLK_115200
//      #define UART_MODE       SMCLK_9600
//      #define UART_MODE       ACLK_9600
//
//   UART RX ISR is used to handle communication.
//   ACLK = 32kHz, MCLK = SMCLK = DCO 16.384MHz.
//
//
//
//                   MSP430i2040
//                 -----------------
//            /|\ |             P1.6|<-- Receive Data (UCA0RXD)
//             |  |                 |
//             ---|RST          P1.7|--> Transmit Data (UCA0TXD)
//                |                 |
//                |                 |
//                |                 |
//   Error LED  <-|P1.4             |
//                |                 |
//                |                 |
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   February 2018
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h>

//******************************************************************************
// Pin Config ******************************************************************
//******************************************************************************

#define LED_OUT     P1OUT
#define LED_DIR     P1DIR
#define LED_PIN     BIT4

//******************************************************************************
// UART Initialization *********************************************************
//******************************************************************************

#define SMCLK_115200     0
#define SMCLK_9600      1
#define ACLK_9600       2

#define UART_MODE       ACLK_9600//SMCLK_115200//

void initUART()
{
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 |= UCSWRST;                     // Put eUSCI in reset
#if UART_MODE == SMCLK_115200

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK 115200 baud
    UCA0BR0   = 142;                          // 16.384MHz/115200 = 142.222 (See UG)
    UCA0BR1   = 0;                            // Fractional portion = 0.222
    UCA0MCTLW = 0x2200;                       // UCBRSx = 0x22

#elif UART_MODE == SMCLK_9600

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK  9600 baud
    UCA0BR0   = 106;                          // 16.384MHz/9600 = 1706.667 (See UG)
    UCA0BR1   = 0;                            // UCOS16 = 1, 1706.667/16 = 106.667
    UCA0MCTLW = UCOS16 | UCBRF_11 | 0xD600;   // UCBFx = INT[0.667*16] = 11
                                              // UCBRSx = 0xD6
#elif UART_MODE == ACLK_9600

    UCA0CTLW0 |= UCSSEL__ACLK;               // CLK = ACLK
    // Baud Rate calculation
    // 32000/(9600) = 3.333
    // Fractional portion = 0.333
    // Use Table 24-5 in Family User Guide
    UCA0BRW = 3;
    UCA0MCTLW = 0x4900;    //0x4900 is UCBRSx = 0x49

#else
    # error "Please specify baud rate to 115200 or 9600"
#endif

    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initGPIO()
{
    //LEDs
    LED_OUT &= ~LED_PIN;
    LED_DIR |= LED_PIN;

    // Configure GPIO
    P1SEL0 |=   BIT2 | BIT3;                // P1.2/3 eUSCI_A Function
    P1SEL1 &= ~(BIT2 | BIT3);
}


void initClockTo16MHz()
{
    unsigned char *CSIRFCAL_CONST = (unsigned char *)0x13D4;
    unsigned char *CSIRTCAL_CONST = (unsigned char *)0x13D5;

    if (*CSIRFCAL_CONST==0xFF)      // If calibration constant erased
    {
        while(1);                   // do not load, trap CPU!!
    }

    if (*CSIRTCAL_CONST==0xFF)      // If calibration constant erased
    {
        while(1);                   // do not load, trap CPU!!
    }

    CSIRFCAL = *CSIRFCAL_CONST;     // Load internal resistor frequency cal
    CSIRTCAL = *CSIRTCAL_CONST;     // Load internal resistor temperature cal

    CSCTL0 = 0;                     // Use oscillator internal resistor
    CSCTL1 = DIVM_0 | DIVS_0;       // MCLK = SMCLK = 16.384MHz
}

//******************************************************************************
// Main ************************************************************************
// Enters LPM0 if SMCLK is used and waits for UART interrupts. If ACLK is used *
// then the device will enter LPM3 mode instead. The UART RX interrupt handles *
// the received character and echoes it.                                       *
//******************************************************************************

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

  initGPIO();
  initClockTo16MHz();
  initUART();

#if UART_MODE == ACLK_9600
    __bis_SR_register(LPM0_bits + GIE);       // Since ACLK is source, enter LPM3, interrupts enabled
#else
    __bis_SR_register(LPM0_bits + GIE);       // Since SMCLK is source, enter LPM0, interrupts enabled
#endif
  __no_operation();                         // For debugger
}

//******************************************************************************
// UART Interrupt ***********************************************************
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      UCA0TXBUF = UCA0RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
