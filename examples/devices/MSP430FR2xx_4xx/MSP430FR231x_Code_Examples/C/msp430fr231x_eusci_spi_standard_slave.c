//******************************************************************************
//   MSP430FR231x Demo - eUSCI_B0, SPI 4-Wire Slave multiple byte RX/TX
//
//   Description: SPI master communicates to SPI slave sending and receiving
//   3 different messages of different length. SPI slave will enter LPM0
//   while waiting for the messages to be sent/receiving using SPI interrupt.
//   ACLK = NA, MCLK = SMCLK = DCO 16MHz.
//
//
//                   MSP430FR2311
//                 -----------------
//            /|\ |             RST |<- Master's GPIO (To reset slave)
//             |  |                 |
//             ---|RST          P2.4|<- Data In (UCB0SIMO)
//                |                 |
//                |             P2.5|-> Data Out (UCB0SOMI)
//                |                 |
//                |             P2.3|<- Serial Clock In (UCB0CLK)
//                |                 |
//                |             P2.2|<- Master's GPIO (UCB0STE)
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   February 2018
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

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
// Example Commands ************************************************************
//******************************************************************************

#define DUMMY   0xFF

/* CMD_TYPE_X_SLAVE are example commands the master sends to the slave.
 * The slave will send example SlaveTypeX buffers in response.
 *
 * CMD_TYPE_X_MASTER are example commands the master sends to the slave.
 * The slave will initialize itself to receive MasterTypeX example buffers.
 * */

#define CMD_TYPE_0_SLAVE              0
#define CMD_TYPE_1_SLAVE              1
#define CMD_TYPE_2_SLAVE              2

#define CMD_TYPE_0_MASTER              3
#define CMD_TYPE_1_MASTER              4
#define CMD_TYPE_2_MASTER              5

#define TYPE_0_LENGTH              1
#define TYPE_1_LENGTH              2
#define TYPE_2_LENGTH              6

#define MAX_BUFFER_SIZE     20

/* MasterTypeX are example buffers initialized in the master, they will be
 * sent by the master to the slave.
 * SlaveTypeX are example buffers initialized in the slave, they will be
 * sent by the slave to the master.
 * */

uint8_t MasterType2 [TYPE_2_LENGTH] = {0};
uint8_t MasterType1 [TYPE_1_LENGTH] = { 0, 0};
uint8_t MasterType0 [TYPE_0_LENGTH] = { 0};

uint8_t SlaveType2 [TYPE_2_LENGTH] = {'A', 'B', 'C', 'D', '1', '2'};
uint8_t SlaveType1 [TYPE_1_LENGTH] = {0x15, 0x16};
uint8_t SlaveType0 [TYPE_0_LENGTH] = {12};

//******************************************************************************
// General SPI State Machine ***************************************************
//******************************************************************************

typedef enum SPI_ModeEnum{
    IDLE_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    TIMEOUT_MODE
} SPI_Mode;

/* Used to track the state of the software state machine*/
SPI_Mode SlaveMode = RX_REG_ADDRESS_MODE;

/* The Register Address/Command to use*/
uint8_t ReceiveRegAddr = 0;

/* ReceiveBuffer: Buffer used to receive data in the ISR
 * RXByteCtr: Number of bytes left to receive
 * ReceiveIndex: The index of the next byte to be received in ReceiveBuffer
 * TransmitBuffer: Buffer used to transmit data in the ISR
 * TXByteCtr: Number of bytes left to transfer
 * TransmitIndex: The index of the next byte to be transmitted in TransmitBuffer
 * */
uint8_t ReceiveBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t RXByteCtr = 0;
uint8_t ReceiveIndex = 0;
uint8_t TransmitBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t TXByteCtr = 0;
uint8_t TransmitIndex = 0;

/* Initialized the software state machine according to the received cmd
 *
 * cmd: The command/register address received
 * */
void SPI_Slave_ProcessCMD(uint8_t cmd);

/* The transaction between the slave and master is completed. Uses cmd
 * to do post transaction operations. (Place data from ReceiveBuffer
 * to the corresponding buffer based in the last received cmd)
 *
 * cmd: The command/register address corresponding to the completed
 * transaction
 */
void SPI_Slave_TransactionDone(uint8_t cmd);
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);
void SendUCB0Data(uint8_t val);

void SendUCB0Data(uint8_t val)
{
    while (!(UCB0IFG & UCTXIFG));              // USCI_B0 TX buffer ready?
    UCB0TXBUF = val;
}

void SPI_Slave_ProcessCMD(uint8_t cmd)
{
    ReceiveIndex = 0;
    TransmitIndex = 0;
    RXByteCtr = 0;
    TXByteCtr = 0;

    switch (cmd)
    {
        case (CMD_TYPE_0_SLAVE):                        //Send slave device id (This device's id)
            SlaveMode = TX_DATA_MODE;
            TXByteCtr = TYPE_0_LENGTH;
            //Fill out the TransmitBuffer
            CopyArray(SlaveType0, TransmitBuffer, TYPE_0_LENGTH);
            //Send First Byte
            SendUCB0Data(TransmitBuffer[TransmitIndex++]);
            TXByteCtr--;
            break;
        case (CMD_TYPE_1_SLAVE):                      //Send slave device time (This device's time)
            SlaveMode = TX_DATA_MODE;
            TXByteCtr = TYPE_1_LENGTH;
            //Fill out the TransmitBuffer
            CopyArray(SlaveType1, TransmitBuffer, TYPE_1_LENGTH);
            //Send First Byte
            SendUCB0Data(TransmitBuffer[TransmitIndex++]);
            TXByteCtr--;
            break;
        case (CMD_TYPE_2_SLAVE):                  //Send slave device location (This device's location)
            SlaveMode = TX_DATA_MODE;
            TXByteCtr = TYPE_2_LENGTH;
            //Fill out the TransmitBuffer
            CopyArray(SlaveType2, TransmitBuffer, TYPE_2_LENGTH);
            //Send First Byte
            SendUCB0Data(TransmitBuffer[TransmitIndex++]);
            TXByteCtr--;
            break;
        case (CMD_TYPE_0_MASTER):
            SlaveMode = RX_DATA_MODE;
            RXByteCtr = TYPE_0_LENGTH;
            break;
        case (CMD_TYPE_1_MASTER):
            SlaveMode = RX_DATA_MODE;
            RXByteCtr = TYPE_1_LENGTH;
            break;
        case (CMD_TYPE_2_MASTER):
            SlaveMode = RX_DATA_MODE;
            RXByteCtr = TYPE_2_LENGTH;
            break;
        default:
            __no_operation();
            break;
    }
}


void SPI_Slave_TransactionDone(uint8_t cmd)
{
    switch (cmd)
    {
        case (CMD_TYPE_0_SLAVE):                        //Slave device id was sent(This device's id)
            break;
        case (CMD_TYPE_1_SLAVE):                      //Slave device time was sent(This device's time)
            break;
        case (CMD_TYPE_2_SLAVE):                  //Send slave device location (This device's location)
            break;
        case (CMD_TYPE_0_MASTER):
            CopyArray(ReceiveBuffer, MasterType0, TYPE_0_LENGTH);
            break;
        case (CMD_TYPE_1_MASTER):
            CopyArray(ReceiveBuffer, MasterType1, TYPE_1_LENGTH);
            break;
        case (CMD_TYPE_2_MASTER):
            CopyArray(ReceiveBuffer, MasterType2, TYPE_2_LENGTH);
            break;
        default:
            __no_operation();
            break;
    }
}

void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++)
    {
        dest[copyIndex] = source[copyIndex];
    }
}

//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initSPI()
{
    //Clock Polarity: The inactive state is high
    //MSB First, 8-bit, Master, 4-pin mode, Synchronous
    UCB0CTLW0 = UCSWRST;                       // **Put state machine in reset**
    UCB0CTLW0 |= UCCKPL | UCMSB | UCSYNC | UCMODE_2;    // 4-pin, 8-bit SPI Slave
    UCB0CTLW0 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB0IE |= UCRXIE;                          // Enable USCI0 RX interrupt
}

void initGPIO()
{
    //LEDs
    LED0_OUT &= ~LED0_PIN;
    LED0_DIR |= LED0_PIN;

    LED1_OUT &= ~LED1_PIN;
    LED1_DIR |= LED1_PIN;

    // Configure GPIO
    P2SEL0 |= BIT2 | BIT3 | BIT4 | BIT5;      // Configure SPI pins

    SYSCFG2 |= USCIBRMP;                      // Configure SPI to use P2.2-2.5

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
}


//******************************************************************************
// Main ************************************************************************
// Enters LPM0 and waits for SPI interrupts. The data sent from the master is  *
// then interpreted and the device will respond accordingly                    *
//******************************************************************************

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initClockTo16MHz();
    initGPIO();
    initSPI();


    __bis_SR_register(LPM0_bits + GIE);                 // Enter LPM0, enable interrupts
    __no_operation();
    return 0;
}

//******************************************************************************
// SPI Interrupt ***************************************************************
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uint8_t ucb0_rx_val = 0;
    switch(__even_in_range(UCB0IV, USCI_SPI_UCTXIFG))
    {
        case USCI_NONE: break;
        case USCI_SPI_UCRXIFG:
            ucb0_rx_val = UCB0RXBUF;
            UCB0IFG &= ~UCRXIFG;
            switch (SlaveMode)
            {
                case (RX_REG_ADDRESS_MODE):
                    ReceiveRegAddr = ucb0_rx_val;
                    SPI_Slave_ProcessCMD(ReceiveRegAddr);
                    break;
                case (RX_DATA_MODE):
                    ReceiveBuffer[ReceiveIndex++] = ucb0_rx_val;
                    RXByteCtr--;
                    if (RXByteCtr == 0)
                    {
                        //Done Receiving MSG
                        SlaveMode = RX_REG_ADDRESS_MODE;
                        SPI_Slave_TransactionDone(ReceiveRegAddr);
                    }
                    break;
                case (TX_DATA_MODE):
                    if (TXByteCtr > 0)
                    {
                        SendUCB0Data(TransmitBuffer[TransmitIndex++]);
                        TXByteCtr--;
                    }
                    if (TXByteCtr == 0)
                    {
                        //Done Transmitting MSG
                        SlaveMode = RX_REG_ADDRESS_MODE;
                        SPI_Slave_TransactionDone(ReceiveRegAddr);
                    }
                    break;
                default:
                    __no_operation();
                    break;
                }
            break;
        case USCI_SPI_UCTXIFG:
            break;
        default: break;
    }
}
