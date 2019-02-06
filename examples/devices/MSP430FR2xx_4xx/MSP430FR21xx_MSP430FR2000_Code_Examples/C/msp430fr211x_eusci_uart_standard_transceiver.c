//******************************************************************************
//   MSP430FR211x Demo - eUSCI_A0, UART Echo received character
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
//   ACLK = 32.768kHz, MCLK = SMCLK = DCO 16MHz.
//
//
//
//                   MSP430FR2111
//                 -----------------
//            /|\ |             P1.6|<-- Receive Data (UCA0RXD)
//             |  |                 |
//             ---|RST          P1.7|--> Transmit Data (UCA0TXD)
//                |                 |
//                |             P2.7|--- XIN
//                |                 |   |
//   Error LED  <-|P1.0             |  32kHz
//                |                 |   |
//                |             P2.6|--- XOUT
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

#define LED0_OUT    P1OUT
#define LED0_DIR    P1DIR
#define LED0_PIN    BIT0

#define LED1_OUT    P2OUT
#define LED1_DIR    P2DIR
#define LED1_PIN    BIT0

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
    UCA0CTLW0 |= UCSWRST;                      // Put eUSCI in reset
#if UART_MODE == SMCLK_115200

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate Setting
    // Use Table 21-5
    UCA0BRW = 8;
    UCA0MCTLW |= UCOS16 | UCBRF_10 | 0xF700;   //0xF700 is UCBRSx = 0xF7

#elif UART_MODE == SMCLK_9600

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate Setting
    // Use Table 21-5
    UCA0BRW = 104;
    UCA0MCTLW |= UCOS16 | UCBRF_2 | 0xD600;   //0xD600 is UCBRSx = 0xD6

#elif UART_MODE == ACLK_9600

    UCA0CTLW0 |= UCSSEL__ACLK;               // CLK = ACLK
    // Baud Rate calculation
    // 32768/(9600) = 3.4133
    // Fractional portion = 0.4133
    // Use Table 24-5 in Family User Guide
    UCA0BRW = 3;
    UCA0MCTLW |= 0x9200;    //0x9200 is UCBRSx = 0x92

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
    LED0_OUT &= ~LED0_PIN;
    LED0_DIR |= LED0_PIN;

    LED1_OUT &= ~LED1_PIN;
    LED1_DIR |= LED1_PIN;

    // Configure GPIO
    P1SEL1 &= ~(BIT6 | BIT7);                 // USCI_A0 UART operation
    P1SEL0 |= BIT6 | BIT7;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}

void initClockTo16MHz()
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);    // disable FLL
    CSCTL3 |= SELREF__REFOCLK;  // Set REFO as FLL reference source
    CSCTL0 = 0;                 // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);     // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;        // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;      // set to fDCOCLKDIV = (FLLN + 1)*(fFLLREFCLK/n)
                                //                   = (487 + 1)*(32.768 kHz/1)
                                //                   = 16 MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                        // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));      // FLL locked

#if UART_MODE == ACLK_9600
    P2SEL1 |= BIT6 | BIT7;                  // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag

    __bis_SR_register(SCG0);                // Disable FLL
    CSCTL3 = SELREF__XT1CLK;                // Set XT1 as FLL reference source
    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;  // set ACLK = XT1CLK = 32768Hz
                                               // DCOCLK = MCLK and SMCLK source
#else
    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;
#endif
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
    __bis_SR_register(LPM3_bits + GIE);       // Since ACLK is source, enter LPM3, interrupts enabled
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
