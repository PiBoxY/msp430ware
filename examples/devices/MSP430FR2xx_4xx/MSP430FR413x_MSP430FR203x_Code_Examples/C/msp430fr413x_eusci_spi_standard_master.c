//******************************************************************************
//   MSP430FR413x Demo - eUSCI_B0, SPI 4-Wire Master multiple byte RX/TX
//
//   Description: SPI master communicates to SPI slave sending and receiving
//   3 different messages of different length. SPI master will enter LPM0 mode
//   while waiting for the messages to be sent/receiving using SPI interrupt.
//   SPI Master will initially wait for a port interrupt in LPM0 mode before
//   starting the SPI communication.
//   ACLK = NA, MCLK = SMCLK = DCO 16MHz.
//
//
//                   MSP430FR4133
//                 -----------------
//            /|\ |             P1.1|-> Comms LED
//             |  |                 |
//             ---|RST          P5.4|-> Slave Reset (GPIO)
//                |                 |
//                |             P5.2|-> Data Out (UCB0SIMO)
//                |                 |
//       Button ->|P1.3         P5.3|<- Data In (UCB0SOMI)
//   Button LED <-|P1.0             |
//                |             P5.1|-> Serial Clock Out (UCB0CLK)
//                |                 |
//                |             P5.0|-> Slave Chip Select (GPIO)
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   November 2017
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h>
#include <stdint.h>

//******************************************************************************
// Pin Config ******************************************************************
//******************************************************************************

#define SLAVE_RST_OUT   P5OUT
#define SLAVE_RST_DIR   P5DIR
#define SLAVE_RST_PIN   BIT4

#define BUTTON_DIR      P1DIR
#define BUTTON_OUT      P1OUT
#define BUTTON_REN      P1REN
#define BUTTON_PIN      BIT3
#define BUTTON_IES      P1IES
#define BUTTON_IE       P1IE
#define BUTTON_IFG      P1IFG
#define BUTTON_VECTOR   PORT1_VECTOR

#define BUTTON_LED_OUT  P1OUT
#define BUTTON_LED_DIR  P1DIR
#define BUTTON_LED_PIN  BIT0

#define COMMS_LED_OUT   P1OUT
#define COMMS_LED_DIR   P1DIR
#define COMMS_LED_PIN   BIT1

#define SLAVE_CS_OUT    P5OUT
#define SLAVE_CS_DIR    P5DIR
#define SLAVE_CS_PIN    BIT0

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

uint8_t MasterType0 [TYPE_0_LENGTH] = {0x11};
uint8_t MasterType1 [TYPE_1_LENGTH] = {8, 9};
uint8_t MasterType2 [TYPE_2_LENGTH] = {'F', '4' , '1' , '9', '2', 'B'};

uint8_t SlaveType2 [TYPE_2_LENGTH] = {0};
uint8_t SlaveType1 [TYPE_1_LENGTH] = {0};
uint8_t SlaveType0 [TYPE_0_LENGTH] = {0};


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
SPI_Mode MasterMode = IDLE_MODE;

/* The Register Address/Command to use*/
uint8_t TransmitRegAddr = 0;

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

/* SPI Write and Read Functions */

/* For slave device, writes the data specified in *reg_data
 *
 * reg_addr: The register or command to send to the slave.
 *           Example: CMD_TYPE_0_MASTER
 * *reg_data: The buffer to write
 *           Example: MasterType0
 * count: The length of *reg_data
 *           Example: TYPE_0_LENGTH
 *  */
SPI_Mode SPI_Master_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

/* For slave device, read the data specified in slaves reg_addr.
 * The received data is available in ReceiveBuffer
 *
 * reg_addr: The register or command to send to the slave.
 *           Example: CMD_TYPE_0_SLAVE
 * count: The length of data to read
 *           Example: TYPE_0_LENGTH
 *  */
SPI_Mode SPI_Master_ReadReg(uint8_t reg_addr, uint8_t count);
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);
void SendUCB0Data(uint8_t val);

void SendUCB0Data(uint8_t val)
{
    while (!(UCB0IFG & UCTXIFG));              // USCI_B0 TX buffer ready?
    UCB0TXBUF = val;
}

void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++)
    {
        dest[copyIndex] = source[copyIndex];
    }
}


SPI_Mode SPI_Master_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    MasterMode = TX_REG_ADDRESS_MODE;
    TransmitRegAddr = reg_addr;

    //Copy register data to TransmitBuffer
    CopyArray(reg_data, TransmitBuffer, count);

    TXByteCtr = count;
    RXByteCtr = 0;
    ReceiveIndex = 0;
    TransmitIndex = 0;

    SLAVE_CS_OUT &= ~(SLAVE_CS_PIN);
    SendUCB0Data(TransmitRegAddr);

    __bis_SR_register(CPUOFF + GIE);              // Enter LPM0 w/ interrupts

    SLAVE_CS_OUT |= SLAVE_CS_PIN;
    return MasterMode;
}

SPI_Mode SPI_Master_ReadReg(uint8_t reg_addr, uint8_t count)
{
    MasterMode = TX_REG_ADDRESS_MODE;
    TransmitRegAddr = reg_addr;
    RXByteCtr = count;
    TXByteCtr = 0;
    ReceiveIndex = 0;
    TransmitIndex = 0;

    SLAVE_CS_OUT &= ~(SLAVE_CS_PIN);
    SendUCB0Data(TransmitRegAddr);

    __bis_SR_register(CPUOFF + GIE);              // Enter LPM0 w/ interrupts

    SLAVE_CS_OUT |= SLAVE_CS_PIN;
    return MasterMode;
}

//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initSPI()
{
    //Clock Polarity: The inactive state is high
    //MSB First, 8-bit, Master, 3-pin mode, Synchronous
    UCB0CTLW0 = UCSWRST;                       // **Put state machine in reset**
    UCB0CTLW0 |= UCCKPL | UCMSB | UCSYNC
                | UCMST | UCSSEL__SMCLK;      // 3-pin, 8-bit SPI Slave
    UCB0BRW = 0x20;
    //UCB0MCTLW = 0;
    UCB0CTLW0 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB0IE |= UCRXIE;                          // Enable USCI0 RX interrupt
}


void initGPIO()
{
    //LEDs
    COMMS_LED_DIR |= COMMS_LED_PIN;
    COMMS_LED_OUT &= ~COMMS_LED_PIN;

    BUTTON_LED_DIR |= BUTTON_LED_PIN;
    BUTTON_LED_OUT &= ~BUTTON_LED_PIN;

    // Configure SPI
    P5SEL0 |= BIT1 | BIT2 | BIT3;

    SLAVE_RST_DIR |= SLAVE_RST_PIN;
    SLAVE_RST_OUT |= SLAVE_RST_PIN;

    SLAVE_CS_DIR |= SLAVE_CS_PIN;
    SLAVE_CS_OUT |= SLAVE_CS_PIN;

    //Button to initiate transfer
    BUTTON_DIR &= ~(BUTTON_PIN);            // button input
    BUTTON_OUT |= BUTTON_PIN;               // button pull up
    BUTTON_REN |= BUTTON_PIN;               // button pull up/down resistor enable
    BUTTON_IES |= BUTTON_PIN;               // button Hi/lo edge
    BUTTON_IFG &= ~BUTTON_PIN;              // button IFG cleared
    BUTTON_IE  |= BUTTON_PIN;               // button interrupt enabled

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
// Send and receive three messages containing the example commands *************
//******************************************************************************


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    initClockTo16MHz();
    initGPIO();
    initSPI();

    SLAVE_RST_OUT &= ~SLAVE_RST_PIN;        // Now with SPI signals initialized,
    __delay_cycles(100000);
    SLAVE_RST_OUT |= SLAVE_RST_PIN;         // reset slave
    __delay_cycles(100000);                 // Wait for slave to initialize

    COMMS_LED_OUT |= COMMS_LED_PIN;
    __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
    SPI_Master_ReadReg(CMD_TYPE_2_SLAVE, TYPE_2_LENGTH);
    CopyArray(ReceiveBuffer, SlaveType2, TYPE_2_LENGTH);

    SPI_Master_ReadReg(CMD_TYPE_1_SLAVE, TYPE_1_LENGTH);
    CopyArray(ReceiveBuffer, SlaveType1, TYPE_1_LENGTH);

    SPI_Master_ReadReg(CMD_TYPE_0_SLAVE, TYPE_0_LENGTH);
    CopyArray(ReceiveBuffer, SlaveType0, TYPE_0_LENGTH);

    SPI_Master_WriteReg(CMD_TYPE_2_MASTER, MasterType2, TYPE_2_LENGTH);
    SPI_Master_WriteReg(CMD_TYPE_1_MASTER, MasterType1, TYPE_1_LENGTH);
    SPI_Master_WriteReg(CMD_TYPE_0_MASTER, MasterType0, TYPE_0_LENGTH);
    __bis_SR_register(LPM0_bits + GIE);
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
            switch (MasterMode)
            {
                case TX_REG_ADDRESS_MODE:
                    if (RXByteCtr)
                    {
                        MasterMode = RX_DATA_MODE;   // Need to start receiving now
                        //Send Dummy To Start
                        __delay_cycles(2000000);
                        SendUCB0Data(DUMMY);
                    }
                    else
                    {
                        MasterMode = TX_DATA_MODE;        // Continue to transmision with the data in Transmit Buffer
                        //Send First
                        SendUCB0Data(TransmitBuffer[TransmitIndex++]);
                        TXByteCtr--;
                    }
                    break;

                case TX_DATA_MODE:
                    if (TXByteCtr)
                    {
                      SendUCB0Data(TransmitBuffer[TransmitIndex++]);
                      TXByteCtr--;
                    }
                    else
                    {
                      //Done with transmission
                      MasterMode = IDLE_MODE;
                      __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
                    }
                    break;

                case RX_DATA_MODE:
                    if (RXByteCtr)
                    {
                        ReceiveBuffer[ReceiveIndex++] = ucb0_rx_val;
                        //Transmit a dummy
                        RXByteCtr--;
                    }
                    if (RXByteCtr == 0)
                    {
                        MasterMode = IDLE_MODE;
                        __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
                    }
                    else
                    {
                        SendUCB0Data(DUMMY);
                    }
                    break;

                default:
                    __no_operation();
                    break;
            }
            __delay_cycles(1000);
            break;
        case USCI_SPI_UCTXIFG:
            break;
        default: break;
    }
}


//******************************************************************************
// PORT1 Interrupt *************************************************************
// Interrupt occurs on button press and initiates the SPI data transfer ********
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=BUTTON_VECTOR
__interrupt void Button_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(BUTTON_VECTOR))) Button_ISR (void)
#else
#error Compiler not supported!
#endif
{
    BUTTON_LED_OUT |= BUTTON_LED_PIN;
    BUTTON_IFG &= ~BUTTON_PIN;                  // button IFG cleared
    BUTTON_IE &= ~BUTTON_PIN;
    //Initiate
    __bic_SR_register_on_exit(LPM0_bits);       // Exit LPM0
}
