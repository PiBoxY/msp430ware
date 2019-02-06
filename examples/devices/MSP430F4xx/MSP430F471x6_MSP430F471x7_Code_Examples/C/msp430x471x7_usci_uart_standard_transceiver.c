//******************************************************************************
//   MSP430x471xx Demo - USCI_A0, UART Echo received character
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
//                   MSP430F47197
//                 -----------------
//            /|\ |             P2.6|--- XIN
//             |  |                 |   |
//             ---|RST              |   32kHz
//                |                 |   |
//                |             P2.7|--- XOUT
//                |             P2.5|<------- Receive Data (UCA0RXD)
//                |             P2.4|-------> Transmit Data (UCA0TXD)
//                |                 |
//                |                 |
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   January 2018
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h>
#include <stdint.h>

//******************************************************************************
// UART Initialization *********************************************************
//******************************************************************************

#define SMCLK_115200     0
#define SMCLK_9600      1
#define ACLK_9600       2

#define UART_MODE       SMCLK_115200//ACLK_9600//

void SendUCA0Data(uint8_t data)
{
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    UCA0TXBUF = data;
}
void initUART()
{
#if UART_MODE == SMCLK_115200
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 138;                            // 16MHz 115200
    UCA0BR1 = 0;                              // 16MHz 115200
    UCA0MCTL = UCBRS_7;                       // Modulation UCBRSx = 7
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IFG2 &= ~(UCA0RXIFG);
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
#elif UART_MODE == SMCLK_9600
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 16MHz 9600
    UCA0BR1 = 0;                              // 16MHz 9600
    UCA0MCTL = UCBRS_0 + UCOS16 + UCBRF_3;    // Modulation UCBRSx = 0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IFG2 &= ~(UCA0RXIFG);
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
#elif UART_MODE == ACLK_9600
    UCA0CTL1 |= UCSSEL_1;                     // ACLK
    UCA0BR0 = 3;                              // 32768Hz 9600
    UCA0BR1 = 0;                              // 32768Hz 9600
    UCA0MCTL = UCBRS_3;                       // Modulation UCBRSx = 3
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IFG2 &= ~(UCA0RXIFG);
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
#else
    #error "Select UART Baud Rate of 115200 or 9600"
#endif
}

//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initClockTo16MHz()
{
    FLL_CTL0 |= DCOPLUS + XCAP11PF;           // DCO+ set, freq = xtal x D x N+1
    SCFI0 = FLLD_4+FN_4;                      // x4 DCO freq, 16MHz nominal DCO
    SCFQCTL = 121;                            // (121+1) x 32768 x 4 = 16 MHz
}

void initGPIO()
{
    P2SEL = BIT4 | BIT5;                      // P2.5 = RXD, P2.4=TXD
}

//******************************************************************************
// Main ************************************************************************
// Enters LPM0 if SMCLK is used and waits for UART interrupts. If ACLK is used *
// then the device will enter LPM3 mode instead. The UART RX interrupt handles *
// the received character and echoes it.                                       *
//******************************************************************************

void main()
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    initClockTo16MHz();
    initGPIO();
    initUART();

#if UART_MODE == ACLK_9600
    __bis_SR_register(LPM3_bits + GIE);       // Since ACLK is source, enter LPM3, interrupts enabled
#else
    __bis_SR_register(LPM0_bits + GIE);       // Since SMCLK is source, enter LPM0, interrupts enabled
#endif
}

//******************************************************************************
// UART RX Interrupt ***********************************************************
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (IFG2 & UCA0RXIFG)
    {
        uint8_t rx_val = UCA0RXBUF; //Must read UCxxRXBUF to clear the flag
        SendUCA0Data(rx_val);
    }
}
