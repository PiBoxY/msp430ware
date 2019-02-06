//******************************************************************************
//   MSP430F673x Demo - USCI_A0, SPI 3-Wire Master multiple byte RX/TX
//
//   Description: SPI master communicates to SPI slave sending and receiving
//   3 different messages of different length. SPI master will enter LPM0 mode
//   while waiting for the messages to be sent/receiving using SPI interrupt.
//   SPI Master will initially wait for a port interrupt in LPM0 mode before
//   starting the SPI communication.
//   ACLK = NA, MCLK = SMCLK = DCO 16MHz.
//
//
//                   MSP430F6736
//                 -----------------
//            /|\ |             P1.7|-> Comms LED
//             |  |                 |
//             ---|RST          P1.5|-> Slave Chip Select (GPIO)
//                |                 |
//                |             P1.4|-> Slave Reset (GPIO)
//                |                 |
//       Button ->|P1.1         P1.3|-> Data Out (UCA0SIMO)
//   Button LED <-|P1.0             |
//                |             P1.2|<- Data In (UCA0SOMI)
//                |                 |
//                |             P1.6|-> Serial Clock Out (UCA0CLK)
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   January 2018
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
// Pin Config ******************************************************************
//******************************************************************************

#define SLAVE_CS_OUT    P1OUT
#define SLAVE_CS_DIR    P1DIR
#define SLAVE_CS_PIN    BIT5

#define SLAVE_RST_OUT   P1OUT
#define SLAVE_RST_DIR   P1DIR
#define SLAVE_RST_PIN   BIT4

#define BUTTON_DIR      P1DIR
#define BUTTON_OUT      P1OUT
#define BUTTON_REN      P1REN
#define BUTTON_PIN      BIT1
#define BUTTON_IES      P1IES
#define BUTTON_IE       P1IE
#define BUTTON_IFG      P1IFG
#define BUTTON_VECTOR   PORT1_VECTOR

#define BUTTON_LED_OUT  P1OUT
#define BUTTON_LED_DIR  P1DIR
#define BUTTON_LED_PIN  BIT0

#define COMMS_LED_OUT   P1OUT
#define COMMS_LED_DIR   P1DIR
#define COMMS_LED_PIN   BIT7

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

uint8_t MasterType0[TYPE_0_LENGTH] = {0x11};
uint8_t MasterType1[TYPE_1_LENGTH]= {8, 9};
uint8_t MasterType2[TYPE_2_LENGTH] = {'F', '4' , '1' , '9', '2', 'B'};

uint8_t SlaveType0[TYPE_0_LENGTH] = {0};
uint8_t SlaveType1[TYPE_1_LENGTH] = {0};
uint8_t SlaveType2[TYPE_2_LENGTH] = {0};


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
    while (!(UCA0IFG & UCTXIFG));              // USCI_A0 TX buffer ready?
    UCA0TXBUF = val;
}


//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initClockTo16MHz()
{
    UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
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

void initGPIO()
{
    //LEDs
    COMMS_LED_DIR |= COMMS_LED_PIN;
    COMMS_LED_OUT &= ~COMMS_LED_PIN;

    BUTTON_LED_DIR |= BUTTON_LED_PIN;
    BUTTON_LED_OUT &= ~BUTTON_LED_PIN;

    //SPI Pins
    P1SEL |= BIT2 | BIT3 | BIT6;            // Assign P1.6 to UCA0CLK and...
                                            // P1.2 UCA0SOMI P1.3 UCA0SIMO

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
    UCA0CTLW0 |= UCSWRST;                         // **Put state machine in reset**
    UCA0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB; // 3-pin, 8-bit SPI master
                                                  // Clock polarity high, MSB
    UCA0CTLW0 |= UCSSEL_2;                        // SMCLK
    UCA0BRW_L = 0x20;                             // /32
    UCA0BRW_H = 0;                                //
    UCA0MCTLW = 0;                                // No modulation
    UCA0CTLW0 &= ~UCSWRST;                        // **Initialize USCI state machine**
    UCA0IE = UCRXIE;                              // Enable USCI_A0 RX,TX interrupt

    SLAVE_CS_DIR |= SLAVE_CS_PIN;
    SLAVE_CS_OUT |= SLAVE_CS_PIN;
}


//******************************************************************************
// Main ************************************************************************
// Send and receive three messages containing the example commands *************
//******************************************************************************


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    increaseVCoreToLevel2();
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
    __no_operation();

	return 0;
}


//******************************************************************************
// SPI Interrupt ***************************************************************
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
  uint8_t uca0_rx_val = 0;
  switch(__even_in_range(UCA0IV,4))
  {
    case 0:break;                             // Vector 0 - no interrupt
    case 2:                                   // Vector 2 - RXIFG
        uca0_rx_val = UCA0RXBUF;
        switch (MasterMode)
        {
            case TX_REG_ADDRESS_MODE:
                if (RXByteCtr)
                {
                    MasterMode = RX_DATA_MODE;   // Need to start receiving now
                    //Send Dummy To Start
                    __delay_cycles(2000000);
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
        __delay_cycles(1000);
        break;
    case 4:break;                             // Vector 4 - TXIFG
    default: break;
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
  __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
}
