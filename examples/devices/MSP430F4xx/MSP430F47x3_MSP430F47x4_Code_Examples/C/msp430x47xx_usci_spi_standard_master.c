//******************************************************************************
//   MSP430x47xx Demo - USCI_A0, SPI 3-Wire Master multiple byte RX/TX
//
//   Description: SPI master communicates to SPI slave sending and receiving
//   3 different messages of different length. SPI master will enter LPM0 mode
//   while waiting for the messages to be sent/receiving using SPI interrupt.
//   SPI Master will initially wait for a port interrupt in LPM0 mode before
//   starting the SPI communication.
//   ACLK = NA, MCLK = SMCLK = DCO 16MHz.
//
//
//                   MSP430F4794
//                 -----------------
//            /|\ |             P5.3|-> Comms LED
//             |  |                 |
//             ---|RST          P5.2|-> Slave Reset (GPIO)
//                |                 |
//                |             P2.4|-> Data Out (UCA0SIMO)
//       Button ->|P1.3             |
//   Button LED <-|P5.1         P2.5|<- Data In (UCA0SOMI)
//                |                 |
//            ----|XIN          P3.0|-> Serial Clock Out (UCA0CLK)
//          32kHz |                 |
//            ----|XOUT         P3.3|-> Slave Chip Select (GPIO)
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   January 2018
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h>
#include <stdint.h>

//******************************************************************************
// Pin Config ******************************************************************
//******************************************************************************

#define SLAVE_CS_OUT    P3OUT
#define SLAVE_CS_DIR    P3DIR
#define SLAVE_CS_PIN    BIT3

#define SLAVE_RST_OUT   P5OUT
#define SLAVE_RST_DIR   P5DIR
#define SLAVE_RST_PIN   BIT2

#define BUTTON_DIR      P1DIR
#define BUTTON_OUT      P1OUT
#define BUTTON_REN      P1REN
#define BUTTON_PIN      BIT3
#define BUTTON_IES      P1IES
#define BUTTON_IE       P1IE
#define BUTTON_IFG      P1IFG
#define BUTTON_VECTOR   PORT1_VECTOR

#define BUTTON_LED_OUT  P5OUT
#define BUTTON_LED_DIR  P5DIR
#define BUTTON_LED_PIN  BIT1

#define COMMS_LED_OUT   P5OUT
#define COMMS_LED_DIR   P5DIR
#define COMMS_LED_PIN   BIT3

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
void SendUCA0Data(uint8_t val);

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
    SendUCA0Data(TransmitRegAddr);

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
    SendUCA0Data(TransmitRegAddr);

    __bis_SR_register(CPUOFF + GIE);              // Enter LPM0 w/ interrupts

    SLAVE_CS_OUT |= SLAVE_CS_PIN;
    return MasterMode;
}

void SendUCA0Data(uint8_t val)
{
    while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
    UCA0TXBUF = val;
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
  //LEDs
  COMMS_LED_DIR |= COMMS_LED_PIN;
  COMMS_LED_OUT &= ~COMMS_LED_PIN;

  BUTTON_LED_DIR |= BUTTON_LED_PIN;
  BUTTON_LED_OUT &= ~BUTTON_LED_PIN;

  //SPI Pins
  P2SEL |= BIT4 | BIT5;
  P3SEL |= BIT0;

  SLAVE_RST_DIR |= SLAVE_RST_PIN;
  SLAVE_RST_OUT |= SLAVE_RST_PIN;

  SLAVE_CS_DIR |= SLAVE_CS_PIN;
  SLAVE_CS_OUT |= SLAVE_CS_PIN;

  //Button to initiate transfer
  BUTTON_DIR &= ~(BUTTON_PIN);            // button input
  BUTTON_OUT |= BUTTON_PIN;               // button pull up
  BUTTON_REN |= BUTTON_PIN;               // button pull up/down resistor enable
  BUTTON_IES |= BUTTON_PIN;               // button Hi/lo edge
  BUTTON_IE  |= BUTTON_PIN;               // button interrupt enabled
  BUTTON_IFG &= ~BUTTON_PIN;              // button IFG cleared
}

void initSPI()
{
  //Clock Polarity: The inactive state is high
  //MSB First, 8-bit, Master, 3-pin mode, Synchronous
  UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 |= 0x20;                          // /32
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation must be cleared for SPI
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt
}

//******************************************************************************
// Main ************************************************************************
// Send and receive three messages containing the example commands *************
//******************************************************************************

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  initClockTo16MHz();
  initGPIO();
  initSPI();

  SLAVE_RST_OUT &= ~SLAVE_RST_PIN;          // Now with SPI signals initialized,
  __delay_cycles(100000);
  SLAVE_RST_OUT |= SLAVE_RST_PIN;           // reset slave
  __delay_cycles(100000);                   // Wait for slave to initialize

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
}

//******************************************************************************
// SPI Interrupt ***************************************************************
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIA0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (IFG2 & UCA0RXIFG)
    {
        uint8_t uca0_rx_val = UCA0RXBUF;
        switch (MasterMode)
        {
            case TX_REG_ADDRESS_MODE:
                if (RXByteCtr)
                {
                    MasterMode = RX_DATA_MODE;   // Need to start receiving now
                    //Send Dummy To Start
                    __delay_cycles(2000000);//2000);
                    SendUCA0Data(DUMMY);
                }
                else
                {
                    MasterMode = TX_DATA_MODE;        // Continue to transmision with the data in Transmit Buffer
                    //Send First
                    SendUCA0Data(TransmitBuffer[TransmitIndex++]);
                    TXByteCtr--;
                }
                break;

            case TX_DATA_MODE:
                if (TXByteCtr)
                {
                  SendUCA0Data(TransmitBuffer[TransmitIndex++]);
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
                    ReceiveBuffer[ReceiveIndex++] = uca0_rx_val;
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
                    SendUCA0Data(DUMMY);
                }
                break;

            default:
                __no_operation();
                break;
        }
        __delay_cycles(1000);//50);
    }
}


//******************************************************************************
// Button Port Interrupt *************************************************************
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
    __bic_SR_register_on_exit(LPM0_bits);      // Exit LPM0
}

