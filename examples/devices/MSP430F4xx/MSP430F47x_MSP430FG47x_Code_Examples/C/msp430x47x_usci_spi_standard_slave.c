//******************************************************************************
//   MSP430x47x Demo - USCI_A0, SPI 3-Wire Slave multiple byte RX/TX
//
//   Description: SPI master communicates to SPI slave sending and receiving
//   3 different messages of different length. SPI slave will enter LPM0
//   while waiting for the messages to be sent/receiving using SPI interrupt.
//   ACLK = NA, MCLK = SMCLK = DCO 8MHz.
//
//
//                   MSP430FG479
//                 -----------------
//            /|\ |             RST |<- Master's GPIO (To reset slave)
//             |  |                 |
//             ---|RST          P2.4|<- Data In (UCA0SIMO)
//                |                 |
//                |             P2.5|-> Data Out (UCA0SOMI)
//            ----|XIN              |
//          32kHz |             P3.0|<- Serial Clock In (UCA0CLK)
//            ----|XOUT             |
//                |             P3.3|<- Master's GPIO (Chip Select)
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   January 2018
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h> 
#include <stdint.h>

//******************************************************************************
// Example Commands ************************************************************
//******************************************************************************

#define DUMMY   0xFF

#define SLAVE_CS_IN     P3IN
#define SLAVE_CS_DIR    P3DIR
#define SLAVE_CS_PIN    BIT3

#define CLK_IN          P3IN
#define CLK_PIN         BIT0

#define LED_OUT         P4OUT
#define LED_DIR         P4DIR
#define LED_PIN         BIT6

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
void SendUCA0Data(uint8_t val);

void SendUCA0Data(uint8_t val)
{
    while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
    UCA0TXBUF = val;
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
            SendUCA0Data(TransmitBuffer[TransmitIndex++]);
            TXByteCtr--;
            break;
        case (CMD_TYPE_1_SLAVE):                      //Send slave device time (This device's time)
            SlaveMode = TX_DATA_MODE;
            TXByteCtr = TYPE_1_LENGTH;
            //Fill out the TransmitBuffer
            CopyArray(SlaveType1, TransmitBuffer, TYPE_1_LENGTH);
            //Send First Byte
            SendUCA0Data(TransmitBuffer[TransmitIndex++]);
            TXByteCtr--;
            break;
        case (CMD_TYPE_2_SLAVE):                  //Send slave device location (This device's location)
            SlaveMode = TX_DATA_MODE;
            TXByteCtr = TYPE_2_LENGTH;
            //Fill out the TransmitBuffer
            CopyArray(SlaveType2, TransmitBuffer, TYPE_2_LENGTH);
            //Send First Byte
            SendUCA0Data(TransmitBuffer[TransmitIndex++]);
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

void initClockTo8MHz()
{
    volatile unsigned int i;

    FLL_CTL0 |= DCOPLUS | XCAP18PF;           // DCO+ set, freq = xtal x D x N+1

    // Wait for xtal to stabilize
    do
    {
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    for (i = 0x47FF; i > 0; i--);             // Time for flag to set
    }
    while ((IFG1 & OFIFG));                   // OSCFault flag still set?

    for(i=2100;i>0;i--);                      // Now with stable ACLK, wait for
                                              // DCO to stabilize.
    SCFI0 |= FN_4;                            // x2 DCO freq, 8MHz nominal DCO
    SCFQCTL = 121;                            // (121+1) x 32768 x 2 = 8 MHz
}

void initGPIO()
{
  //SPI Pins
    P2SEL |= BIT4 | BIT5;
    P3SEL |= BIT0;

  SLAVE_CS_DIR &= ~(SLAVE_CS_PIN);

  //LEDs
  LED_DIR |= LED_PIN;
  LED_OUT &= ~LED_PIN;
}

void initSPI()
{
  UCA0CTL1 = UCSWRST;                       // **Put state machine in reset**
  UCA0CTL0 |= UCCKPL + UCMSB + UCSYNC;      // 3-pin, 8-bit SPI Slave
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IFG2 = 0;
  IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt
}

//******************************************************************************
// Main ************************************************************************
// Enters LPM0 and waits for SPI interrupts. The data sent from the master is  *
// then interpreted and the device will respond accordingly                    *
//******************************************************************************

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
  while (!(CLK_IN & CLK_PIN));              // If clock sig from mstr stays low,
                                            // it is not yet in SPI mode
  initClockTo8MHz();
  initGPIO();
  initSPI();

  LED_OUT |= LED_PIN;
  __delay_cycles(10);
  LED_OUT &= ~LED_PIN;

  __bis_SR_register(LPM0_bits | GIE);       // Enter LPM0, enable interrupts
  __no_operation();
  return 0;
}


//******************************************************************************
// SPI Interrupt ***************************************************************
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    LED_OUT |= LED_PIN;
    if (IFG2 & UCA0RXIFG)
    {
        unsigned char uca0_rx_val = UCA0RXBUF;
        if (!(SLAVE_CS_IN & SLAVE_CS_PIN))
        {
            switch (SlaveMode)
            {
                  case (RX_REG_ADDRESS_MODE):
                      ReceiveRegAddr = uca0_rx_val;
                      SPI_Slave_ProcessCMD(ReceiveRegAddr);
                      break;
                  case (RX_DATA_MODE):
                      ReceiveBuffer[ReceiveIndex++] = uca0_rx_val;
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
                          SendUCA0Data(TransmitBuffer[TransmitIndex++]);
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
        }

    }
    LED_OUT &= ~LED_PIN;
}
