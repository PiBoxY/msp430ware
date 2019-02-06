//******************************************************************************
//  MSP430G2xx3 Demo - USCI_B0 I2C Slave RX single bytes from MSP430 Master
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave. This is the slave code. The interrupt driven
//  data receiption is demonstrated using the USCI_B0 RX interrupt.
//  ACLK = n/a, MCLK = SMCLK = default DCO = 1.0MHz
//
//
//                                /|\  /|\
//               MSP430G2xx3      10k  10k     MSP430G2xx3
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P1.7/UCB0SDA|<-|---+->|P1.7/UCB0SDA  XIN|-
//            |                 |  |      |                 |
//           -|XOUT             |  |      |             XOUT|-
//            |     P1.6/UCB0SCL|<-+----->|P1.6/UCB0SCL     |
//            |                 |         |                 |
//******************************************************************************
#include <msp430.h>
#include <msp430g2553.h>
#include "IIC.h"

unsigned char TXctr;

extern void set_motor_direction(unsigned char direction);
extern void set_motor_speed(unsigned char speed);
extern volatile unsigned int current_speed, last_speed;
extern volatile unsigned char current_direction;


void Setup_IIC_Slave()
{
	P1SEL |= BIT6 + BIT7;			// Assign I2C pins to USCI_B0
	P1SEL2|= BIT6 + BIT7;			// Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;			// Enable SW reset
	UCB0CTL0 = UCMODE_3 + UCSYNC;	// I2C Slave, synchronous mode
	// UCB0CTL1 |= UCSSEL_3;
	UCB0I2COA = Slave_Add;			// Own Address is Slave_Add
	UCB0CTL1 &= ~UCSWRST;			// Clear SW reset, resume operation
	IE2 |= UCB0RXIE | UCB0TXIE;		// Enable RX interrupt
	// UCB0I2CIE |= UCSTPIE;
	UCB0I2CIE =UCSTPIE | UCSTTIE;

	rotate_counter = 0x0000;
	RXData = 0x00;
	TXctr = 0;
}

// USCI_B0 Data ISR
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	if (IFG2&UCB0RXIFG)
	{
		RXData = UCB0RXBUF;

		if (RXData==0x30)
		{
			rotate_counter = 0x000;
		}
		else
		{
			set_motor_speed(RXData & 0x0F);
			set_motor_direction(RXData >> 4);
		
			IE1		&= ~WDTIE;
		}
	}	// Get RX data

	if (IFG2&UCB0TXIFG)
	{
		if (TXctr == 1)
		{
			UCB0TXBUF = (char)(rotate_counter >> 8);
			TXctr++;
		}

		if (TXctr == 0)
		{
			UCB0TXBUF = (char)(rotate_counter & 0xff);
			TXctr++;
		}

	}
}

// USCI_B0 State ISR
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	if ((UCB0STAT&UCSTPIFG) || (UCB0STAT&UCSTTIFG))
	{
		UCB0STAT &= ~(UCSTPIFG + UCSTTIFG);				// Clear interrupt flags
		TXctr = 0;										// reset TX byte counter
	}
}
