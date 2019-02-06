/*
 * This code is a reference design for rotational flow meter with 2 LC sensors
 * An I2C cable is used to link up the Main board and motor board.
 * An Auto process to will be started once the boards are powered up.
 * The main board will undergo Auto TSM setting, and optimize the reference voltage setting for DAC.
 *
 * The number of rotation  detected by ESI will be displayed in the lower digits of the LCD,
 * while the upper digits will display the rotation number from motor board. The data is sent via the I2C.
 *
 * When measuring the current consumption, remove all jumpers of the main board except for the jumper of GND above J401.
 * and tap the current meter to the jumper 3V3.
 * To measure the current consumption of ESI, without LCD and I2C,
 * press the black button on the main board and remove the I2C cable.
 *
 * To turn on the LCD again, press the black button to toggle it.
 *
 * Texas Instruments
 *
 * Author: Thomas Kot
 * Date  : July 2014
 *
 *
 */


#include "msp430fr6989.h"
#include "LCD.h"
#include "ScanIF.h"
#include "ESI_ESIOSC.h"
#include "IIC.h"

#define Time_out  8192      					// 2 sec for time out of Recalibration
#define Time_to_Recal 8192  					// 2 sec for testing use, 40960 for 10 sec

extern 	unsigned char  Status_flag ;
char Power_measure = 0;
signed int  test_status = 0;
unsigned char ReCal_Flag ;

int  rotation_counter = 0;

unsigned int Record_INT1=0;
unsigned int Record_INT2=0;
unsigned int Record_INT3=0;
unsigned int Record_INT4=0;


void Set_Clock(void);
void Port_Init(void);
void Set_Timer_A(void);
void Check_debug(void);
void Disable_all_IE(void);
void Enable_all_IE(void);

void Port_Init()
{
/*
 * This is to set the I/O port in the initialization.
 * The power consumption will be 0.8uA in LPM4 after setting.
 *
 */
	P1DIR=0xFF;
	P1OUT=0;

	P1DIR &= ~BIT2;
	P1REN |= BIT2;
	P1OUT |= BIT2;

	P2DIR=0xFF;
	P2OUT=0;
	P3DIR=0xFF;
	P3OUT=0;
	P4DIR=0xFF;
	P4OUT=0;
	P5DIR=0xFF;
	P5OUT=0;
	P6DIR=0xFF;
	P6OUT=0;
	P7DIR=0xFF;
	P7OUT=0;
	P8DIR=0xFF;
	P8OUT=0;
	P9DIR=0x00;
	P9OUT=0;
	P10DIR=0xFF;
	P10OUT=0;
	PJDIR=0xFF;
	PJOUT=0;

	CECTL3 = 0xFF00;

	PM5CTL0 &= ~LOCKLPM5;;        				// In initialization, the I/Os are configured before unlocking the I/O ports

	PMMCTL0_H = 0xA5;
//	PMMCTL0_L |= PMMREGOFF;
	PMMCTL0_L &= ~SVSHE;
	PMMCTL0_H = 0xEE;

}

void Set_Clock(void)
{
// use external 32KHz crystal
// MClock and SMClock from DCO 4MHz


      PJSEL0 |=  BIT4 + BIT5;					// using LFXTCLK
	  CSCTL0 = 0XA500;                          // Write in the password for system clock setting
	  CSCTL1 = 0x0006;                          // 4MHz for DCO
	  CSCTL2 = 0x0033;                          // ACLK from LFXTCLK, SMCLK and MCLK from DCO
	  CSCTL3 = 0x0000;							// ACLK div by 1, SMCLK div by 1, MCLK div by 1
	  CSCTL4 = 0x0148;							// HFXTOFF, LFXTDRIVE = 1, VLO off, SMCLK on

	  	 do
	  	  {
	  	    CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
	  	    SFRIFG1 &= ~OFIFG;
	  	  }while (SFRIFG1&OFIFG);               // Test oscillator fault flag

}


void Set_Timer_A(void)
{
/*  This is the timer for triggering the run time re-calibration
 *  2 sec is set for demonstration purpose
 *  The actual timer setting depends on the application.
 */

	TA0CTL = TASSEL0 + ID0 + ID1 + TACLR; 		//Aclk divided by 8;
	TA0CCR0 = Time_to_Recal;              		// 2 sec for testing use
	TA0CCTL0 = CCIE; 					  		// INT enable, INT flag cleared
//	TA0CTL |= MC0;
}


void Disable_all_IE(void)
{
	Record_INT1 =  ESIINT1;
	Record_INT2 = (TA0CCTL0&CCIE) ;
	Record_INT3 = (UCB0IE &(UCTXIE | UCRXIE  | UCSTPIE | UCNACKIE));

	ESIINT1 = 0x0000;
	TA0CCTL0 &= ~CCIE;
	UCB0IE   &= ~(UCTXIE | UCRXIE  | UCSTPIE | UCNACKIE);

}

void Enable_all_IE(void)
{
   ESIINT1 = Record_INT1;
   TA0CCTL0 |= Record_INT2;
   UCB0IE   |= Record_INT3;

}

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;					// disable Watchdog

	Port_Init();
	Set_Clock();

	P1IES |= BIT2;								// Set P1.2 as key input
	P1IFG &= ~BIT2;								// User can press the black button to toggle switch on/off the LCD
	P1IE  |= BIT2;

	P1OUT |= BIT6 + BIT7; 						// for I2C pull up. For measuring current consumption, remove the physical I2C cable.

//	__bis_SR_register(LPM4_bits+GIE);   		// un-remark this instruction. Engineer can measure the current consumption here. 0.7 to 0.8uA

	init_LCD();
	lcd_display_num(0,1);						// Display "0" on the upper digit which indicating the rotation number detected from infra-red sensor in motor board
	lcd_display_num(0,0);						// Display "0" on the lower digit which indicating the rotation number detected from ESI


	ReCal_Flag = 0;                 			// Init the status flag with non-runtime calibration

	EsioscInit(ESIOSC_Default);       			// default setting = 4.8MHz for internal oscillator of ESI

	Set_IIC();                      			// IIC setting
	IIC_TX(0x00);								// ensure the motor is topped

	InitScanIF();								// Initialization of ScanIf module
	Status_flag |= BIT2;						// indicating Calibration of DAC process completed


//	Disable_all_IE();							// These two routines are prepared for testing use, in case of Interrupt conflict
//	Enable_all_IE();


#if AFE2_enable
	 Set_Timer_A();                				// set and start timer for triggering run-time re-calibration
#endif


while(1)	                					// Infinite loop for demonstration purpose
 {
	 ESICTL  &= ~ESIEN;							// Disable the ESI

	 __delay_cycles(80);
	 IIC_TX(0x00);             					// stop the motor rotation
	 __delay_cycles(8000000);
	 IIC_TX(0x30);             					// reset motor rotation counter
	 lcd_display_num(0,1);                      // Set LCD to "0" for motor
	 lcd_display_num(0,0);						// Set LCD to "0" for ESI
     __delay_cycles(8000000);


 	 ESIINT1 &= ~ESIIE5;
 	 ESICTL  |= ESIEN;            				// ESI enable. This will reset all counters of ESI. For actual operation of flowmeter, switch on ESI and will always on till battery drain off
 	 ESIINT2 &= ~ESIIFG5;                   	// clear INT flag of Q6 of PSM

	 TA0CTL &= ~MC0;							// Reset Timer for runtime Re-calibration
	 TA0CTL |= TACLR;
	 TA0CTL |= MC0;
	 ReCal_Flag = 0;

	 IIC_TX(0x2F); 					        	// start motor clockwise rotation at 45 to 50 turns per second
	 __delay_cycles(800000);

 	 ESIINT1 |= ESIIE5;							// enable INT of Q6


	while(1)									// Demonstrate the 1000 rotations of rotor disc
	{

		if (test_status&BIT0)
		{test_status &=~BIT0;                   // when 1000 rotations done, go to next cycle of 1000 rotations demonstration
		break;									// and repeat it infinitely
		}

	    __bis_SR_register(LPM3_bits | GIE);   	// Enter into LPM3 and enable interrupts
	                                            // keep in LPM3 until there is a rotation to trigger ESI Q6 interrupt


#if AFE2_enable

	if(ReCal_Flag&BIT6)							// Check if Re-calibration flag is set
	{
	  ReCal_Flag &= ~BIT7;                  	// Reset Bit7 for timer call

	  TA0CCR0 = Time_out;                   	// set the time out timer. it will generate a time out interrupt when it stop rotating
	  TA0CTL |= MC0;
	  ESIINT1 &= ~ESIIE5;

	  ReCalScanIF();           					// to do runtime calibration with AFE2

	  TA0CTL &= ~MC0;
	  TA0CTL |= TACLR;                      	// Reset Timer
	  TA0CCR0 = Time_to_Recal;              	// Set the timer back for Re-calibration counting
	  TA0CTL |= MC0;							// timer re-start for ReCal.

	  ReCal_Flag = 0;							// ReCal of AFE1 is done, reset all flags.

	  __bic_SR_register(GIE);					// Ensure no abnormal interrupt before entering LPM;
	  ESIINT2 &= ~ESIIFG5;                  	// clear the Q6 flag
	  ESIINT1 |= ESIIE5;						// Enable Q6 INT for in case of Time out.
	}
#endif


	}
}

}



#pragma vector=ESCAN_IF_VECTOR
__interrupt void ISR_ESCAN_IF(void)
{
   TA0CCTL0 &= ~CCIE;

   switch (ESIIV)
   {
   case 0x02:  if (ESIINT1&ESIIE1)
				{ESIINT2 &= ~ESIIFG1;                 								// clear the ESISTOP flag
   	   	   	   	   if(ReCal_Flag&BIT6)
					{TA0CTL |= TACLR;                   							// Reset Timer to prevent abnormal time out.
					TA0CCTL0 &= ~CCIFG;	}

   	   	   	   	    TA0CCTL0 |= CCIE;
   	   	   	   	   	_low_power_mode_off_on_exit();     								// exit low power mode;
				}
			  break;

   case 0x04:  break;
   case 0x06:  break;
   case 0x08:  break;

   case 0x0A: break;
   case 0x0C: if(ESIINT1&ESIIE5)
   	   	   	   	   {    ESIINT2 &= ~ESIIFG5;                						// clear the Q6 flag

   	   	   	   	   	   if(ReCal_Flag&BIT6)
						{TA0CTL |= TACLR;                   						// Reset Timer to prevent abnormal time out.
						TA0CCTL0 &= ~CCIFG;	}

						if(ReCal_Flag&BIT7)
						{ ReCal_Flag |= BIT6;	}           						// to do runtime calibration with AFE2

						if(Status_flag&BIT2)                						// Check for completion of Calibration of DAC
							{							    						// If yes, LCD is to display the rotation number
							ESIINT1 &= ~ESIIE5;

							 if(!(ReCal_Flag&BIT6))
							 	 {
									if (LCDCCTL0&LCDON)
									{   IIC_RX();									// to get the number of rotation detected by motor board
										rotation_counter = Master_RXData[1];
										rotation_counter <<= 8;
										rotation_counter += Master_RXData[0];
										lcd_display_num(rotation_counter,1);        // to display the data from motor board in the upper digits of LCD
									}
							 	 }

							ESIINT2 &= ~ESIIFG5;
							ESIINT1 |= ESIIE5;


						rotation_counter = ESICNT1;									// get the ESI counter for number of rotation
							if (rotation_counter < 0)
							{rotation_counter = -1*rotation_counter /4;}			// divided by 4 as the counter is increased by 1 for every state change of 2 LC sensor.
							else													// which is set by PSM table
							{rotation_counter = rotation_counter / 4;}

						lcd_display_num(rotation_counter,0);						// to display the number of rotation from ESI in low digits of LCD



							if (rotation_counter > 1000)							// when number of rotation reaches 1000, it stop the motor
								{													// User can then check the counting from ESI and Motor and see if they match.
																					// There has a +/- 1 difference as the detector in motor board is not in the same physical position as that of LC sensors
								    ESIINT1 &= ~ESIIE5;
								    if (LCDCCTL0&LCDON) IIC_TX(0x20);				// send command to stop the rotor

									 __delay_cycles(8000000);

									 if (LCDCCTL0&LCDON)
										 {
										 	IIC_RX();
											rotation_counter = Master_RXData[1];
											rotation_counter <<= 8;
											rotation_counter += Master_RXData[0];
											lcd_display_num(rotation_counter,1);   	// to display the data from motor board in the upper digits of LCD
										 }

									rotation_counter = ESICNT1;
										if (rotation_counter < 0)
										{rotation_counter = -1*rotation_counter /4;}
										else
										{rotation_counter = rotation_counter / 4;}

									lcd_display_num(rotation_counter,0);    		// to display the number of rotation from ESI in low digits of LCD

										 __delay_cycles(8000000);

									 test_status |= BIT0;							// indication of completion of 1000 rotations

								}

							}

						TA0CCTL0 |= CCIE;
						_low_power_mode_off_on_exit();       						// exit low power mode;
   	   	   	   	   }

	          break;
   case 0x0E: break;
   case 0x10: break;
   case 0x12: break;
   }

   TA0CCTL0 |= CCIE;

}


// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    if (ReCal_Flag&BIT6)
    {
    	ReCal_Flag |= BIT1;                           								// Time out

    	ESIINT1 &= ~(ESIIE1+ESIIE5);				  								// Prevent entering ESI ISR to call an extra "_low_power_mode_off_on_exit()"

    	_low_power_mode_off_on_exit();       	      								// exit low power mode from ReCal_ScanIF ;
    }
    else
    {
    	ReCal_Flag |= BIT7;                   		 								// indicate the need to perform runtime calibration
    }

	TA0CTL &= ~MC0;									 								// disable timer

}



// Timer A1 interrupt service routine for I2C time out timer
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A (void)
{
	TA1CTL &= ~MC_1;
	Set_IIC();
	__bic_SR_register_on_exit(LPM0_bits ); 											// Exit LPM0

}


// Port 1 interrupt service routine for push button of the main board
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    P1IFG &= ~BIT2;
    Power_measure ^= BIT0;

    if (Power_measure & BIT0)
    {LCDCCTL0 &= ~LCDON;
    ESIINT1 &= ~ESIIE5;																// disable INT of Q6
	TA0CTL &= ~MC0;																	// stop the re-calibartion timer
    }
    else
    {LCDCCTL0 |= LCDON;
    ESIINT1 |= ESIIE5;																// enable INT of Q6
	TA0CTL |= MC0;																	// turn on re-calibration timer
    }

   _low_power_mode_off_on_exit();

}
