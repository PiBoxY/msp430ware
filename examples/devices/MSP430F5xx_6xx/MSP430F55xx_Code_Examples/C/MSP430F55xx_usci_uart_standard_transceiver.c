//******************************************************************************
//   MSP430F552x Demo - USCI_A0, UART Echo received character
//                     (ACLK 9600/SMCLK 9600/SMCLK 115200)
//
//   Description: The device will wait in LPM0/LPM3 (based on clock source)
//   until a UART character is received.
//   Then the device will echo the received character.
//   The UART can operate using ACLK at 9600, SMCLK at 115200 or SMCLK at 9600.
//   To configure the UART mode, change the following line:
//
//      #define UART_MODE       SMCLK_11500
//      to any of:
//      #define UART_MODE       SMCLK_11500
//      #define UART_MODE       SMCLK_9600
//      #define UART_MODE       ACLK_9600
//
//   UART RX ISR is used to handle communication.
//   ACLK = 32.768kHz, MCLK = SMCLK = DCO 16MHz.
//
//
//                   MSP430F5529
//                 -----------------
//            /|\ |             P5.4|--- XIN
//             |  |                 |   |
//             ---|RST              |   32kHz
//                |                 |   |
//                |             P5.5|--- XOUT
//                |             P3.4|<------- Receive Data (UCA0RXD)
//                |             P3.3|-------> Transmit Data (UCA0TXD)
//                |                 |
//                |                 |
//
//   Nima Eskandari
//   Texas Instruments Inc.
//   April 2017
//   Built with CCS V7.0
//******************************************************************************

#include "msp430.h"
#include <stdbool.h>
#include <stdint.h>

//******************************************************************************
// UART Initialization *********************************************************
//******************************************************************************

#define SMCLK_11500     0
#define SMCLK_9600      1
#define ACLK_9600       2

#define UART_MODE       SMCLK_115200

void initUART()
{
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
#if UART_MODE == SMCLK_115200

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 16000000/(16*115200) = 8.6805
    // Fractional portion = 0.6805
    // Use Table 24-5 in Family User Guide
    UCA0BR0 = 8;                               // 16000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCOS16 | UCBRF_11 | UCBRS_0;

#elif UART_MODE == SMCLK_9600

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 16000000/(16*9600) = 104.1667
    // Fractional portion = 0.1667
    // Use Table 24-5 in Family User Guide
    UCA0BR0 = 104;                            // 16000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCOS16 | UCBRF_3 | UCBRS_0;

#elif UART_MODE == ACLK_9600

    UCA0CTLW0 |= UCSSEL__ACLK;               // CLK = ACLK
    // Baud Rate calculation
    // 32768/(9600) = 3.4133
    // Fractional portion = 0.4133
    // Use Table 24-5 in Family User Guide
    UCA0BR0 = 3;                             // 32768/9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCBRS_3;    //0x0300 is UCBRSx = 0x03

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
    P3SEL = BIT3 + BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
    P5SEL |= BIT4+BIT5;                         // Select XT1
}

void initClockTo16MHz()
{
    UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_0;                        // Set ACLK = XT1CLK
    __bis_SR_register(SCG0);                  // Disable the FLL control loop
    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_0 + 487;                   // Set DCO Multiplier for 16MHz
                                              // (N + 1) * FLLRef = Fdco
                                              // (487 + 1) * 32768 = 16MHz
                                              // Set FLL Div = fDCOCLK
    __bic_SR_register(SCG0);                  // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 16 MHz / 32,768 Hz = 500000 = MCLK cycles for DCO to settle
    __delay_cycles(500000);//
    // Loop until XT1,XT2 & DCO fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                          // Clear fault flags
    }while (SFRIFG1&OFIFG);                         // Test oscillator fault flag
}

uint16_t setVCoreUp(uint8_t level){
    uint32_t PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;

    //The code flow for increasing the Vcore has been altered to work around
    //the erratum FLASH37.
    //Please refer to the Errata sheet to know if a specific device is affected
    //DO NOT ALTER THIS FUNCTION

    //Open PMM registers for write access
    PMMCTL0_H = 0xA5;

    //Disable dedicated Interrupts
    //Backup all registers
    PMMRIE_backup = PMMRIE;
    PMMRIE &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE |
                SVSLPE | SVMHVLRIE | SVMHIE |
                SVSMHDLYIE | SVMLVLRIE | SVMLIE |
                SVSMLDLYIE
                );
    SVSMHCTL_backup = SVSMHCTL;
    SVSMLCTL_backup = SVSMLCTL;

    //Clear flags
    PMMIFG = 0;

    //Set SVM highside to new level and check if a VCore increase is possible
    SVSMHCTL = SVMHE | SVSHE | (SVSMHRRL0 * level);

    //Wait until SVM highside is settled
    while((PMMIFG & SVSMHDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMHDLYIFG;

    //Check if a VCore increase is possible
    if((PMMIFG & SVMHIFG) == SVMHIFG)
    {
        //-> Vcc is too low for a Vcore increase
        //recover the previous settings
        PMMIFG &= ~SVSMHDLYIFG;
        SVSMHCTL = SVSMHCTL_backup;

        //Wait until SVM highside is settled
        while((PMMIFG & SVSMHDLYIFG) == 0)
        {
            ;
        }

        //Clear all Flags
        PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                     SVMLVLRIFG | SVMLIFG |
                     SVSMLDLYIFG
                     );

        //Restore PMM interrupt enable register
        PMMRIE = PMMRIE_backup;
        //Lock PMM registers for write access
        PMMCTL0_H = 0x00;
        //return: voltage not set
        return false;
    }

    //Set also SVS highside to new level
    //Vcc is high enough for a Vcore increase
    SVSMHCTL |= (SVSHRVL0 * level);

    //Wait until SVM highside is settled
    while((PMMIFG & SVSMHDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMHDLYIFG;

    //Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;

    //Set SVM, SVS low side to new level
    SVSMLCTL = SVMLE | (SVSMLRRL0 * level) |
               SVSLE | (SVSLRVL0 * level);

    //Wait until SVM, SVS low side is settled
    while((PMMIFG & SVSMLDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMLDLYIFG;
    //SVS, SVM core and high side are now set to protect for the new core level

    //Restore Low side settings
    //Clear all other bits _except_ level settings
    SVSMLCTL &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 +
                 SVSMLRRL1 + SVSMLRRL2
                 );

    //Clear level settings in the backup register,keep all other bits
    SVSMLCTL_backup &=
        ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

    //Restore low-side SVS monitor settings
    SVSMLCTL |= SVSMLCTL_backup;

    //Restore High side settings
    //Clear all other bits except level settings
    SVSMHCTL &= (SVSHRVL0 + SVSHRVL1 +
                 SVSMHRRL0 + SVSMHRRL1 +
                 SVSMHRRL2
                 );

    //Clear level settings in the backup register,keep all other bits
    SVSMHCTL_backup &=
        ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

    //Restore backup
    SVSMHCTL |= SVSMHCTL_backup;

    //Wait until high side, low side settled
    while(((PMMIFG & SVSMLDLYIFG) == 0) &&
          ((PMMIFG & SVSMHDLYIFG) == 0))
    {
        ;
    }

    //Clear all Flags
    PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG
                );

    //Restore PMM interrupt enable register
    PMMRIE = PMMRIE_backup;

    //Lock PMM registers for write access
    PMMCTL0_H = 0x00;

    return true;
}

bool increaseVCoreToLevel2()
{
    uint8_t level = 2;
    uint8_t actlevel;
    bool status = true;

    //Set Mask for Max. level
    level &= PMMCOREV_3;

    //Get actual VCore
    actlevel = PMMCTL0 & PMMCOREV_3;

    //step by step increase or decrease
    while((level != actlevel) && (status == true))
    {
        if(level > actlevel)
        {
            status = setVCoreUp(++actlevel);
        }
    }

    return (status);
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
  increaseVCoreToLevel2();
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
// UART RX Interrupt ***********************************************************
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
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG & UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
