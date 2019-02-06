//******************************************************************************
//  MSP430FR69xx Demo - eUSCI_B0 I2C Master TX bytes to Multiple Slaves
//
//  Description: This demo connects two MSP430's via the I2C bus.
//  The master transmits and receive data via I2C to / from slave addresses 0x02
//  This is a master code.
//
//  IIC_TX is to send one byte to motor board to control the rotation direction and speed.
//  The most significant 4 bit is for direction control
//						0 - stop
//						1 - anti-clockwise
//						2 - clockwise
//						3 - reset motor rotation counter
//
//						The Least significant 4 bit is to control the rotation speed of motor
//						form 0000 to 1111
//
//  IIC_RX is to receive two bytes of data from slave
//  This two bytes tell the number (int format) of rotation of motor.
//  The first byte is the low byte
//  The second byte is the upper byte of an integer
//
//  ACLK = n/a, MCLK = SMCLK =  DCO = 4MHz
//
//
//                                /|\  /|\
//                MSP430G2553    10k  10k     MSP430FR6989
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
//            |                 |          |                 |
//
//******************************************************************************


#include "msp430FR6989.h"
#include "IIC.h"

unsigned char TXData[5];
volatile unsigned char TXByteCtr;
volatile unsigned char RXByteCtr;


void Set_IIC_Timeout()
{
  TA1CTL   = TASSEL__ACLK ;
  TA1CCTL0 = CCIE;                          				// TACCR0 interrupt enabled
  TA1CCR0  = 32768;

}

void Set_IIC()
{

  Set_IIC_Timeout();
  // Configure GPIO
  P1SEL0 |= BIT6 | BIT7;                                    // I2C pins

  // Configure USCI_B0 for I2C mode
  UCB0CTLW0 = UCSWRST;                      				// put eUSCI_B in reset state
  UCB0CTLW0 |= UCMODE_3 | UCSYNC | UCMST | UCSSEL__SMCLK;
  	  	  	  	  	  	  	  	  	  	  					// I2C master mode, SMCLK
  UCB0CTLW1 |= UCASTP_2 | UCCLTO_3;            				// Automatic stop generated,
                                              	  	  	  	// after UCB0TBCNT is reached
  UCB0BRW = 0x0010;                         			    // baudrate = SMCLK / 16
  UCB0TBCNT = 0x0002;                       				// number of bytes to be received

  UCB0I2CSA = Slave_Add;

  UCB0CTLW0 &= ~UCSWRST;                    						// clear reset register
  UCB0IE |= UCTXIE | UCRXIE  | UCSTPIE | UCNACKIE | UCCLTOIE;     	// Enable interrupts

  RXByteCtr = 0;
}

void IIC_TX(unsigned char Data)
{

    UCB0TBCNT = 0x0001;                     				// number of bytes to be received

    TXByteCtr = 0;                          				// Load TX byte counter
    TXData[0] = Data;

    while (UCB0CTLW0 & UCTXSTP);            				// Ensure stop condition got sent

    UCB0CTLW0 |= UCTR | UCTXSTT;            				// I2C TX, start condition

    TA1CTL |= TACLR;
    TA1CTL |= MC_1;
    __bis_SR_register(LPM0_bits | GIE);     				// Enter LPM0 w/ interrupts
                                            				// Remain in LPM0 until all data
                                            				// is TX'd
    TA1CTL &= ~MC_1;


}

void IIC_RX()
{

	UCB0TBCNT = 0x0002;                       				// number of bytes to be received
	RXByteCtr = 0;

	while (UCB0CTL1 & UCTXSTP);            					// Ensure stop condition got sent

    UCB0CTLW0 &=  ~UCTR;
    UCB0CTLW0 |=  UCTXSTT;            						// I2C RX, start condition

    TA1CTL |= TACLR;
    TA1CTL |= MC_1;
    __bis_SR_register(LPM0_bits | GIE);     				// Enter LPM0 w/ interrupts
                                            				// Remain in LPM0 until all data
    TA1CTL &= ~MC_1;

}


#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)

{
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         				// Vector 0: No interrupts
    case USCI_I2C_UCALIFG:   break;         				// Vector 2: ALIFG

    case USCI_I2C_UCNACKIFG:                				// Vector 4: NACKIFG
    // 	UCB0CTLW0 |= UCTXSTT;                 				// resend start if NACK
    	Set_IIC();											// Reset I2C
    	__bic_SR_register_on_exit(LPM0_bits ); 				// Exit LPM0
    						 break;

    case USCI_I2C_UCSTTIFG:  break;         				// Vector 6: STTIFG

    case USCI_I2C_UCSTPIFG:
   															// Stop condition generated, I2C communication completed
    	__bic_SR_register_on_exit(LPM0_bits ); 				// Exit LPM0
    						 break;         				// Vector 8: STPIFG

    case USCI_I2C_UCRXIFG3:  break;         				// Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         				// Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         				// Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         				// Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         				// Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         				// Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:                 				// Vector 22: RXIFG0
    		{
    			Master_RXData[RXByteCtr] = UCB0RXBUF;       // Get RX data
    		    RXByteCtr++;
    		}
    						 break;

    case USCI_I2C_UCTXIFG0:                 				// Vector 24: TXIFG0
    	 UCB0TXBUF = TXData[TXByteCtr];      				// Load TX buffer
    	 TXByteCtr++;
    						 break;

    case USCI_I2C_UCBCNTIFG: break;              			// Vector 26: BCNTIFG

    case USCI_I2C_UCCLTOIFG:
    	 Set_IIC();										    // reset IIC
    	__bic_SR_register_on_exit(LPM0_bits ); 				// Exit LPM0
    						 break;      					// Vector 28: clock low timeout

    case USCI_I2C_UCBIT9IFG: break;         				// Vector 30: 9th bit


    default: break;
  }
}
