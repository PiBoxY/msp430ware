//************************************************************************************************
// TouchProTool_Demo_UART.c
//
//! \brief   Project:
//!             MSP432 Launchpad capacitive touch display demo using UART communication module and 
//!             Touch Pro Tool.
//!
//!             This Touch Pro Tool Demo project enables a user to tune the four elements in the 
//!             wheel sensor on the 430BOOST-SENSE1 for each measurement configuration supported
//!             in the Capacitive Touch Software (CTS) Library for MSP432. The application may be 
//!             configured to display individual sensor element delta counts or detected wheel position
//!             on the Touch Pro Tool. To display delta counts, make sure that Custom_Counts is defined
//!             in the TouchProTool_Demo_UART.h header file. If Custom_Counts is not defined, 
//!             the touch position on the wheel will be displayed on the GUI.
//!
//!             Tuning can be performed for various, user selectable system clock frequencies. 
//!             To switch between measurement configurations, modify the defines in 
//!             structure.h. To change the system clock frequency, update __SYSTEM_CLOCK
//!             in TouchProTool_Demo_UART.h to a valid value.
//!         
//!             For more information, please refer to the Touch Pro GUI User Guide.
//!
//!             Note: The provided sensor element .threshold and .maxResponse values in
//!                   structure.c may not provide optimal performance when detecting wheel
//!                   position. These values may be adjusted during tuning. For more information
//!                   please refer to the CTS Library for MSP432 Programmer's Guide.
//!
//!         
//!           Developed using:
//!               CCS Version : 6.1.0.00104, w/support for GCC extensions (--gcc)
//!               IAR Embedded Workbench for ARM Version : 7.40.3.8938
//!
//!           System Configuration Diagram:
//!             I/O	  MSP-EXP432P401R                     msp432p401r
//!  		           -------------                    ---------------
//! 		          |VCC		 GND|--> GND           |               |
//! 		          |P6.0			|                  |           P1.0|-> LED1
//! 		          |P3.2			|                  |               |
//!                   |P3.3		    |                  |               |
//! 		          |P4.1			|                  |           P1.3|-> UART TX
//! 		          |P4.3		P1.6|                  |               |
//! 		          |P1.5     P1.7|                   ---------------
//!           Prox -->|P4.6     P5.0|<-- Middle Button
//!	            E1 -->|P6.5		P5.2|<-- E4
//! 	        E2 -->|P6.4  	P3.6|<-- E3
//!		               -------------
//!
//! \author B. Moore 
//!         Texas Instruments
//!         Sept. 2015
//!         Ver 1.0
//!
//************************************************************************************************

//************************************************************************************************
// Includes
//************************************************************************************************
#include "msp.h"
#include "driverlib.h"
#include "TouchProTool_Demo_UART.h"
#include "CTS_Layer.h"

//************************************************************************************************
// Defines
//************************************************************************************************

//! \brief Data word sent before each UART data packet
//!
#define DataHead 0x55AA

//! \brief Defines the number of data channels to display
//!
#ifdef Custom_Counts
#define SENSOR_NUM 4                   //1 channel for each element
#else
#define SENSOR_NUM 1                   //Only 1 channel needed to display wheel position
#endif

//************************************************************************************************
// Globals
//************************************************************************************************

//! \brief Array storing the number of counts on each
//!        element from current measurement cycle
//!
uint16_t CAPT_count[SENSOR_NUM];

//! \brief Wheel position determined from current measurement
//!
uint16_t CAPT_wheelPos;

//************************************************************************************************
// Function Prototypes
//************************************************************************************************

//! \brief Initializes WDTA, clocks and I/Os to run application
//!
void System_config(void);

//! \brief Configures eUSCI module for TX only UART
//!
void eUSCI_configUART(void);

//! \brief Sends an entire Touch Data Packet to GUI via UART
//!
void UART_sendTouchData(void);         // Cap Touch Display Data Logger

//! \brief Sends a single data byte via UART using eUSCI module
//!
void UART_sendByte(uint8_t data2send);


//! \brief Function initializes and updates baseline for wheel and monitors the changes in capacitance to
//!        determine touch or no touch.
//!
void main(void) {
#ifndef Custom_Counts
	static const int channelIndex = 0;
#endif

	//Setup system clocks
    System_config();

    // Initialize UART Communication at 9600 Baud (P1.3)
	eUSCI_configUART();

	//Calculate baseline
	TI_CAPT_Init_Baseline(&wheel);
	//Update the baseline
	TI_CAPT_Update_Baseline(&wheel,50);

	//Continuously update element or sensor data
	// and send new data to Touch Pro GUI via UART
	while(1)
	{
#ifdef Custom_Counts
		//API call that records the current element counts
		// based on changes in capacitance
		TI_CAPT_Custom(&wheel, CAPT_count);
#else
		//Gets finger position on the wheel
		CAPT_wheelPos = TI_CAPT_Wheel(&wheel);

		//Limits the No-Touch (Illegal Wheel Position) value from 65535 to 35
		// when sending via UART
		if (CAPT_wheelPos == 0XFFFF){
			CAPT_wheelPos = 35;
		}

		//Stores wheel position in data array
		CAPT_count[channelIndex] = CAPT_wheelPos;
#endif
		//Cap Touch Display Data Logger Call
		UART_sendTouchData();

#if (__SYSTEM_CLOCK == CS_48MHZ)
		// Delay between sending data if system clock too high to avoid
		// lag on Touch Pro GUI
		uint32_t i;
		for(i=0;i<50000;i++);
#endif
		// Toggle LED1
		P1OUT ^= BIT0;
	}
}

//! \brief Function sets up the DCO and SMCLK clocks and P1.0 for blinking LED1
//!
void System_config(void)
{
	// Stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;

	// Enable FPU for DCO frequency calculation
	MAP_FPU_enableModule();

#if (__SYSTEM_CLOCK == CS_1MHZ)

	// Set DCO frequency to 1MHz, source DCO/1 for MCLK  (1MHz), and
	// source DCO/1 for SMCLK (1MHz)
	MAP_CS_setDCOFrequency(CS_1MHZ);
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

#elif (__SYSTEM_CLOCK == CS_4MHZ)

	// Set DCO frequency to 4MHz, source DCO/1 for MCLK  (4MHz)
	// and source DCO/4 for SMCLK (1MHz)
	MAP_CS_setDCOFrequency(CS_4MHZ);
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);

#elif (__SYSTEM_CLOCK == CS_8MHZ)

	// Set DCO frequency to 8MHz, source DCO/1 for MCLK  (8MHz)
	// and source DCO/8 for SMCLK (1MHz)
	MAP_CS_setDCOFrequency(CS_8MHZ);
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_8);

#elif (__SYSTEM_CLOCK == CS_16MHZ)

	// 1 flash wait state (BANK0 VCORE0 max is 12 MHz)
	FLCTL_BANK0_RDCTL &= ~FLCTL_BANK0_RDCTL_WAIT_M | FLCTL_BANK0_RDCTL_WAIT_1;
	FLCTL_BANK1_RDCTL &= ~FLCTL_BANK0_RDCTL_WAIT_M | FLCTL_BANK0_RDCTL_WAIT_1;

	// Set DCO frequency to 16MHz, source DCO/1 for MCLK  (16MHz)
    // and source DCO/16 for SMCLK (1MHz)
	MAP_CS_setDCOFrequency(CS_16MHZ);
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_16);

#elif (__SYSTEM_CLOCK == CS_24MHZ)

	// 1 flash wait state (BANK0 VCORE0 max is 12 MHz)
	FLCTL_BANK0_RDCTL &= ~FLCTL_BANK0_RDCTL_WAIT_M | FLCTL_BANK0_RDCTL_WAIT_1;
	FLCTL_BANK1_RDCTL &= ~FLCTL_BANK0_RDCTL_WAIT_M | FLCTL_BANK0_RDCTL_WAIT_1;

	// Set DCO frequency to 24MHz, source DCO/1 for MCLK  (24MHz)
	// and source DCO/8 for SMCLK (3MHz)
	MAP_CS_setDCOFrequency(CS_24MHZ);
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_8);
#elif (__SYSTEM_CLOCK == CS_48MHZ)

	// Switches LDO VCORE0 to LDO VCORE1; mandatory for 48 MHz setting
	while((PCMCTL1 & PMR_BUSY));
	PCMCTL0 = PCM_CTL_KEY_VAL | AMR_1;
	while((PCMCTL1 & PMR_BUSY));

	// 2 flash wait states (BANK0 VCORE1 max is 16 MHz, BANK1 VCORE1 max is 32 MHz)
	FLCTL_BANK0_RDCTL &= ~FLCTL_BANK0_RDCTL_WAIT_M | FLCTL_BANK0_RDCTL_WAIT_2;
	FLCTL_BANK1_RDCTL &= ~FLCTL_BANK1_RDCTL_WAIT_M | FLCTL_BANK1_RDCTL_WAIT_2;

	// Set DCO frequency to 48MHz, source DCO/1 for MCLK  (48MHz)
	// and source DCO/2 for SMCLK (24MHz)
	MAP_CS_setDCOFrequency(CS_48MHZ);
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
#else
#error "Invalid system clock frequency chosen"
#endif

	// Turn off LED
	P1DIR |= BIT0;
}

//! \brief USCIA - USCI Module Configuration for UART
//!                TX ONLY!
//!                Note: Requires device with USCI
//!
void eUSCI_configUART(void)
{
	// Setup DIO pin 1.3 w/ UART Tx function
	P1SEL0 |= BIT3;

	// Put eUSCI in reset
	UCA0CTLW0 |= UCSWRST;
	// Select SMCLK as clock source
	UCA0CTLW0 |= UCSSEL__SMCLK;

	// Baud Rate calculation
#if (__SYSTEM_CLOCK == 24000000)
	UCA0BR0 = 19;                    // 3000000/9600/16
	UCA0BR1 = 0x00;
	UCA0MCTLW = 0x5500 | UCOS16 | 0x0080;
#elif (__SYSTEM_CLOCK == 48000000)
	UCA0BR0 = 156;                    // 24000000/9600/16
	UCA0BR1 = 0x00;
	UCA0MCTLW = 0x0000 | UCOS16 | 0x0040;
#else
	UCA0BR0 = 6;                    // 1000000/9600/16
	UCA0BR1 = 0x00;
	UCA0MCTLW = 0x2000 | UCOS16 | 0x0080;
#endif

	// Make sure bits are sent LSB first
	UCA0CTLW0 &= ~(UCMSB);
	// Initialize eUSCI
	UCA0CTLW0 &= ~UCSWRST;
}

//! \brief Function calculates data length and check sum. This is the
//!        Cap Touch Display tool protocol
//!
void UART_sendTouchData(void)
{
	uint8_t i = 0;
	uint16_t temp = 0;
	uint8_t data_length = 0;
	uint8_t check_sum = 0;

	//# of channels *3 + 1 (checksum)
	//1 byte for the channel number, 1 byte for high order data byte
	//and 3rd byte is for low order byte.  The plus 1 is for checksum byte.
	data_length = SENSOR_NUM * 3  + 1;

	//Store and send high byte of DataHead
	temp = DataHead;
	temp >>= 8;
	UART_sendByte((uint8_t) temp);
	check_sum = (uint8_t) temp;

	//Send low byte of DataHead
	UART_sendByte((uint8_t) DataHead);
	check_sum += (uint8_t) DataHead;

	//Send data payload length
	UART_sendByte(data_length);
	check_sum += data_length;

	for (i = 0; i < SENSOR_NUM; i++)
	{
		//Send data index
		UART_sendByte(i + 1);
		check_sum += (i + 1);

		//Store and send high byte of data
		temp = CAPT_count[i];
		temp >>= 8;
		UART_sendByte((uint8_t) temp);
		check_sum += (uint8_t) temp;

		//Send low byte
		UART_sendByte((uint8_t) CAPT_count[i]);
		check_sum += (uint8_t) CAPT_count[i];
	}

	//Send check sum
	UART_sendByte((uint8_t) check_sum);
}

//!	\brief USCIA - UART TX Function
//!    	   Note: Requires device with USCI
//! \param unsigned char data2send - byte of data to transmit
//!
void UART_sendByte(uint8_t data2send)
{
	//Wait for any previous transfer to complete
	while(!(UCA0IFG & UCTXIFG));
	//Load data into transmit buffer
	UCA0TXBUF = data2send;
}
