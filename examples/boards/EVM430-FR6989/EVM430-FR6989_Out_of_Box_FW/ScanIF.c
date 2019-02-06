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
#include "ScanIF.h"
#include "ESI_ESIOSC.h"
#include "LCD.h"
#include "IIC.h"


 const unsigned char Table[] = {
 //Q6 trigger on +1 direction only
		0x00,
		0x43,
		0x0C,
		0x89,
		0x04,
		0x01,
		0x88,
		0x4B,
		0X42,
		0X81,
		0X08,
		0X0D,
		0X80,
		0X05,
		0X4A,
		0X09


/* Q6 trigger for both +1 and -1 direction
		 0x00,
		 0x43,
		 0x4C,
		 0x89,
		 0x44,
		 0x01,
		 0x88,
		 0x4B,
		 0x42,
		 0x81,
		 0x08,
		 0x4D,
		 0x80,
		 0x45,
		 0x4A,
		 0x09
*/
 };




#define Search_range  8
#define Separation_factor   4

int AFE1_base0, AFE1_base1;
int AFE2_base0, AFE2_base1;

int AFE2_drift0 = 0;
int AFE2_drift1 = 0;

int AFE2_base0_Max, AFE2_base0_Min;
int AFE2_base1_Max, AFE2_base1_Min;

int New_level;
int Delta;


unsigned char  	Status_flag = 0;
unsigned int  	STATE_SEPARATION = 0;

unsigned int    Noise_level = 0;
unsigned int	Threshold_h1, Threshold_h0,
				Threshold_l1, Threshold_l0;

unsigned int    Max_DAC_Ch0, Max_DAC_Ch1;
unsigned int 	Min_DAC_Ch0, Min_DAC_Ch1;


#if AFE2_enable
extern unsigned char ReCal_Flag ;
extern signed int  rotation_counter;
#endif


void FindDAC(void);
void FindDAC_Fast_Range(int , int , int );
void FindDAC_Fast_Successive(int , int , int );
void ReCalScanIF(void);
void FindTESTDAC(void);
void TSM_Auto_cal(void);
void Find_Noise_level(void);
void Set_DAC(void);

void AFE2_FindDAC_Fast_Successive(int , int , int );
void AFE2_FindDAC_Fast_Range(int , int , int );
void AFE2_FindDAC(void);

void FindDAC(void)
{
	unsigned int i;
	unsigned int DAC_BIT = 0, Prev_DAC_BIT = 0;

	DAC_BIT   = 0x0800;						// DAC Level tester, using Sucessive approx approach
	Prev_DAC_BIT = 0x0C00;

	ESIDAC1R0 = DAC_BIT;                 	// set as the middle point
	ESIDAC1R1 = DAC_BIT;                 	// set as the middle point

	ESIDAC1R2 = DAC_BIT;                 	// set as the middle point
	ESIDAC1R3 = DAC_BIT;                 	// set as the middle point

	ESIINT2 &= ~ESIIFG1;                	// clear the ESISTOP flag
	ESIINT1 |= ESIIE1;						// enable ESISTOP INT
	ESICTL  |= ESIEN;						// switch on Scan Interface.


	for(i = 0; i<12; i++)				 	// test 12 times as 12 bit DAC
	{

	__bis_SR_register(LPM3_bits+GIE);   	//	 wait for the ESISTOP flag

	DAC_BIT /= 2 ;							// right shift one bit

	if (!(ESIPPU&ESIOUT0))                  // channel 0;
		{

			ESIDAC1R0 |= DAC_BIT;     		// keep the previous bit and set the next bit
			ESIDAC1R1 |= DAC_BIT;			// keep the previous bit and set the next bit

		}
	else
		{
			ESIDAC1R0 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
			ESIDAC1R1 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
		}

	if (!(ESIPPU&ESIOUT1))					// channel 1;
		{

			ESIDAC1R2 |= DAC_BIT;     		// keep the previous bit and set the next bit
			ESIDAC1R3 |= DAC_BIT;			// keep the previous bit and set the next bit

		}
	else
		{
			ESIDAC1R2 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
			ESIDAC1R3 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
		}

	Prev_DAC_BIT /= 2;						// right shift one bit
	}

	ESICTL &= ~ESIEN;						// switch off ESI Interface.
	ESIINT1 &= ~ESIIE1;
}


void FindDAC_Fast_Successive(int Starting_point_ch0, int Starting_point_ch1, int Range_num)
{
	unsigned int i;
	unsigned int DAC_BIT = 0;

	DAC_BIT   = 0x0001;						// DAC Level tester, using Sucessive approx approach

	ESIDAC1R0 = Starting_point_ch0;         // set as the middle point
	ESIDAC1R1 = Starting_point_ch0;         // set as the middle point

	ESIDAC1R2 = Starting_point_ch1;         // set as the middle point
	ESIDAC1R3 = Starting_point_ch1;         // set as the middle point

	for(i=1; i<Range_num; i++)
	{

	   DAC_BIT <<= 1;

	}


	ESIINT2 &= ~ESIIFG1;                	// clear the ESISTOP flag
	ESIINT1 |= ESIIE1;						// enable ESISTOP INT
	ESICTL  |= ESIEN;						// switch on Scan Interface.


	for(i = 0; i<Range_num; i++)			// test "Range_num" times as 12 bit DAC
	{

	__bis_SR_register(LPM3_bits+GIE);   	//	 wait for the ESISTOP flag



	if (!(ESIPPU&ESIOUT0))                  // channel 0;
		{

			ESIDAC1R0 += DAC_BIT;
			ESIDAC1R1 += DAC_BIT;

		}
	else
		{
			ESIDAC1R0 -= DAC_BIT;
			ESIDAC1R1 -= DAC_BIT;
		}

	if (!(ESIPPU&ESIOUT1))					// channel 1;
		{

			ESIDAC1R2 += DAC_BIT;
			ESIDAC1R3 += DAC_BIT;

		}
	else
		{
			ESIDAC1R2 -= DAC_BIT;
			ESIDAC1R3 -= DAC_BIT;
		}

		DAC_BIT /= 2;						// right shift one bit

	}

		ESICTL &= ~ESIEN;					// switch off ESI Interface.
		ESIINT1 &= ~ESIIE1;
}



void FindDAC_Fast_Range(int Starting_point_ch0, int Starting_point_ch1, int Range_num)
{

unsigned int Range;
unsigned char Range_status;


	ESIDAC1R0 = Starting_point_ch0;             // set DAC from the starting point
	ESIDAC1R1 = Starting_point_ch0;            	// set DAC from the starting point

	ESIDAC1R2 = Starting_point_ch1;             // set DAC from the starting point
	ESIDAC1R3 = Starting_point_ch1;            	// set DAC from the starting point


	Range_status = 0;
	Range_status |= BIT7;                       // This is the searching Loop enable bit.

	Range = Range_num;


	ESIINT2 &= ~ESIIFG1;                	   // clear the ESISTOP flag
	ESIINT1 |= ESIIE1;						   // enable ESISTOP INT
	ESICTL  |= ESIEN;						   // switch on Scan Interface.


// This loop is to find the starting point of DAC and the direction of searching by adding or subtracting a value of "Range" into or from the DAC
// Range_status is to show the direction of searching
// BIT0   indication of a searching direction of increasing for channel 0;
// BIT1   indication of a searching direction of decreasing for channel 0;
// BIT2   indication of a searching direction of increasing for channel 1;
// BIT3   indication of a searching direction of decreasing for channel 1;
// BIT4   indication of completion of the loop for channel 0;
// BIT4   indication of completion of the loop for channel 1;


while (Range_status&BIT7)
{
		while(!((Range_status&BIT4) && (Range_status&BIT5)))
		{

					__bis_SR_register(LPM3_bits+GIE);   	// wait for the ESISTOP flag



				   if(!(Range_status&BIT4))
				   {
					if (ESIPPU&ESIOUT0)                     // channel 0;
						{

							if(Range_status&BIT1)			// check if there is a decreasing direction mark of channel 0.
							{ Range_status |= BIT4;}        // if yes, put a completion mark for channel 0
							else
							{ Range_status |= BIT0;}        // if no, put a direction mark pointing to increasing for channel 0.

							ESIDAC1R0 -= Range;     		// decrease the DAC by a value of "Range"
							ESIDAC1R1 -= Range;

						}
					else
						{

							if(Range_status&BIT0)			// check if there is a increasing direction mark of channel 0.
							{ Range_status |= BIT4;}        // if yes, put a completion mark for channel 0
							else
							{ Range_status |= BIT1;}        // if no, put a direction mark pointing to decreasing for channel 0.

							ESIDAC1R0 += Range;				// increase the DAC by a value of "Range"
							ESIDAC1R1 += Range;
						}
				   }


				  if(!(Range_status&BIT5))
				  {
					if (ESIPPU&ESIOUT1)						// channel 1;
						{

							if(Range_status&BIT3)			// check if there is a decreasing direction mark of channel 1.
							{ Range_status |= BIT5;}        // if yes, put a completion mark for channel 1
							else
							{ Range_status |= BIT2;}        // if no, put a direction mark pointing to increasing for channel 1.

							ESIDAC1R2 -= Range;     		// decrease the DAC by a value of "Range"
							ESIDAC1R3 -= Range;

						}
					else
						{

							if(Range_status&BIT2)			// check if there is a increasing direction mark of channel 1.
							{ Range_status |= BIT5;}        // if yes, put a completion mark for channel 1
							else
							{ Range_status |= BIT3;}        // if no, put a direction mark pointing to decreasing for channel 1.

							ESIDAC1R2 += Range;				// increase the DAC by a value of "Range"
							ESIDAC1R3 += Range;
						}
				  }

		}

	if (Range > 1)
	{	Range_status &= ~(BIT4+BIT5);            			// clear the flag of completion
		Range_status &= ~(BIT0+BIT1+BIT2+BIT3);				// optional, clear the direction flag
		Range = 1;                                 			// This is to restart the loop with searching value of one, by "+/- 1 method"
	}
	else
	{   Range_status &= ~BIT7;}								// "+/- 1 method completed, the loop end here

}


ESICTL &= ~ESIEN;											// switch off ESI Interface.
ESIINT1 &= ~ESIIE1;

}





void TSM_Auto_cal(void)
{
// constant and variable for TSM calibration
#define cycle_width 8      									// which is equal to (ESICLK / freq of LC) - 2
#define LC_Threshold_TSM_CAL 1600
#define Ch0_finish  BIT0
#define Ch1_finish  BIT1

char	 Ch0_counter = 0;
char	 Ch1_counter = 0;
char     Cal_status = 0;

int  DAC0_sum1,DAC0_sum2;
int  DAC1_sum1,DAC1_sum2;
int  math_temp;


unsigned int i;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// This module is to find the signal level of LC oscillation for channel 0 and 1
// using this information to calibrate the delay for TSM


			 DAC0_sum1 = DAC0_sum2 = 0;
			 DAC1_sum1 = DAC1_sum2 = 0;
			 Ch0_counter = Ch1_counter = 0;



	do
	{

				FindDAC();                                 // using 12 bit sucessive approx. method

				DAC0_sum2 = ESIDAC1R0;
				DAC1_sum2 = ESIDAC1R2;



	// channel 0
			 if(!(Cal_status&Ch0_finish))
			 {
					 if (DAC0_sum2 > LC_Threshold_TSM_CAL)
					 {
					 if (DAC0_sum2 > DAC0_sum1)
						 {math_temp = DAC0_sum2 - DAC0_sum1;}
					 else
						 {math_temp = DAC0_sum1 - DAC0_sum2;}

					  Ch0_counter += 1;

								 if (math_temp > 12)
								 {
									 if (Ch0_counter > cycle_width)
									 {
										 for (i= 0; i< Ch0_counter / 2 ; i++)
										 {	ESITSM3 -= 0x0800;	 }

										 Cal_status |= Ch0_finish;
									 }

									 else
									 {Ch0_counter = 0;
									  DAC0_sum1 = DAC0_sum2;
									 }
								 }
								 else
								 {
									 DAC0_sum1 = DAC0_sum2;
								 }
					 }



							if(!(Cal_status&Ch0_finish))
							{
												if (!((ESITSM3&0xF800) == 0xF800)) { ESITSM3 += 0x0800 ;}
										   else if (!((ESITSM4&0xF800) == 0xF800)) { ESITSM4 += 0x0800 ;}
										   else if (!((ESITSM5&0xF800) == 0xF800)) { ESITSM5 += 0x0800 ;}
										   else if (!((ESITSM6&0xF800) == 0xF800)) { ESITSM6 += 0x0800 ;}
										   else if (!((ESITSM7&0xF800) == 0xF800)) { ESITSM7 += 0x0800 ;}
										   else if (!((ESITSM8&0xF800) == 0xF800)) { ESITSM8 += 0x0800 ;}
										   else    {
											   	 ESITSM2 += 0x0800;
												 ESITSM3 &= 0X07FF;
												 ESITSM4 &= 0X07FF;
												 ESITSM5 &= 0X07FF;
												 ESITSM6 &= 0X07FF;
												 ESITSM7 &= 0X07FF;
												 ESITSM8 &= 0X07FF;

												 DAC0_sum1 = DAC0_sum2 = 0;
										         Ch0_counter = 0;
											 	   }


							 }
			  }

	// channel 1


			 if(!(Cal_status&Ch1_finish))
			 {
				if (DAC1_sum2 > LC_Threshold_TSM_CAL)
				{
				 if (DAC1_sum2 > DAC1_sum1)
					 {math_temp = DAC1_sum2 - DAC1_sum1;}
				 else
					 {math_temp = DAC1_sum1 - DAC1_sum2;}

				 Ch1_counter += 1;

				 if (math_temp > 12)
				 {
					 if (Ch1_counter > cycle_width)
					 {
						 for (i= 0; i< Ch1_counter / 2  ; i++)
						 {	ESITSM15 -= 0x0800;	 }

						 Cal_status |= Ch1_finish;
					 }
					 else
					 {Ch1_counter = 0;
					  DAC1_sum1 = DAC1_sum2;
					 }
				 }
				 else
				 {
					 DAC1_sum1 = DAC1_sum2;
				 }
				}

					   if(!(Cal_status&Ch1_finish))
					   {


								if (!((ESITSM15&0xF800) == 0xF800)) { ESITSM15 += 0x0800 ;}
						   else if (!((ESITSM16&0xF800) == 0xF800)) { ESITSM16 += 0x0800 ;}
						   else if (!((ESITSM17&0xF800) == 0xF800)) { ESITSM17 += 0x0800 ;}
						   else if (!((ESITSM18&0xF800) == 0xF800)) { ESITSM18 += 0x0800 ;}
						   else if (!((ESITSM19&0xF800) == 0xF800)) { ESITSM19 += 0x0800 ;}
						   else if (!((ESITSM20&0xF800) == 0xF800)) { ESITSM20 += 0x0800 ;}
						   else    {
								 ESITSM14 += 0x0800;
								 ESITSM15 &= 0X07FF;
								 ESITSM16 &= 0X07FF;
								 ESITSM17 &= 0X07FF;
								 ESITSM18 &= 0X07FF;
								 ESITSM19 &= 0X07FF;
								 ESITSM20 &= 0X07FF;

								 DAC1_sum1 = DAC1_sum2 = 0;
								 Ch1_counter = 0;
								   }

					   }
			 }


	}while(!(Cal_status==(Ch0_finish+Ch1_finish)))	;


// TSM Calibration competed

}



void Find_Noise_level(void)
{

unsigned int Loop_counter = 0;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// To find the noise level

	   Min_DAC_Ch0 = 0x0FFF;					// set initial value for DAC max and min
	   Min_DAC_Ch1 = 0x0FFF; 					// this variable will record the DAC value of metal and non metal part of a rotor
	   Max_DAC_Ch0 = 0x0000;
	   Max_DAC_Ch1 = 0x0000;



	do {  // do loop for detection of noise level

	FindDAC_Fast_Range(ESIDAC1R0, ESIDAC1R2, Search_range);

	if ( ESIDAC1R1 < Min_DAC_Ch0 ) {Min_DAC_Ch0 = ESIDAC1R1 ;}
	if ( ESIDAC1R0 > Max_DAC_Ch0 ) {Max_DAC_Ch0 = ESIDAC1R0 ;}
	if ( ESIDAC1R3 < Min_DAC_Ch1 ) {Min_DAC_Ch1 = ESIDAC1R3 ;}
	if ( ESIDAC1R2 > Max_DAC_Ch1 ) {Max_DAC_Ch1 = ESIDAC1R2 ;}

	Loop_counter++;

	}while (Loop_counter < 234); 				// run for approx 0.5 second

		 Threshold_h0 = Max_DAC_Ch0 - Min_DAC_Ch0;
		 Threshold_h1 = Max_DAC_Ch1 - Min_DAC_Ch1;

		 if (Threshold_h0 > Threshold_h1)
			 { Noise_level = Threshold_h0;}
		 else
			 { Noise_level = Threshold_h1;}

}


void Set_DAC(void)
{
unsigned int Loop_counter = 0;
	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// when the disc is rotating, the Max and Min of signal is found and their difference is required to be larger than STATE_SEPARATION.
// After reaching the STATE_SEPARATION, it will keep rotating for one more second to ensure a complete rotation is calibrated.

 Min_DAC_Ch0 = 4096;
 Max_DAC_Ch0 = 0;
 Min_DAC_Ch1 = 4096;
 Max_DAC_Ch1 = 0;


	 STATE_SEPARATION = Noise_level*(Separation_factor-1)+Noise_level/2;

	 Loop_counter = 0;


	do {  // do loop for 1 more second after valid separation detected;
	do {  // do loop for detection of valid Max-Min separation;

		FindDAC_Fast_Successive(ESIDAC1R0, ESIDAC1R2, 5);

		if ( ESIDAC1R1 < Min_DAC_Ch0 ) {Min_DAC_Ch0 = ESIDAC1R1 ;}
		if ( ESIDAC1R0 > Max_DAC_Ch0 ) {Max_DAC_Ch0 = ESIDAC1R0 ;}
		if ( ESIDAC1R3 < Min_DAC_Ch1 ) {Min_DAC_Ch1 = ESIDAC1R3 ;}
		if ( ESIDAC1R2 > Max_DAC_Ch1 ) {Max_DAC_Ch1 = ESIDAC1R2 ;}


			// To detect the a change due to rotation
			// if a separation of STATE_SEPARATION is found, a rotation is detected and keep running for 1 second to find the max and min

			 Threshold_h0 = Max_DAC_Ch0 - Min_DAC_Ch0;
			 Threshold_h1 = Max_DAC_Ch1 - Min_DAC_Ch1;


			 if (Threshold_h0 > STATE_SEPARATION) { Status_flag |= BIT0;}          // check for valid separation
			 if (Threshold_h1 > STATE_SEPARATION) { Status_flag |= BIT1;}


		}while (!((Status_flag&BIT0)&&(Status_flag&BIT1)));

			Loop_counter++;
		} while(Loop_counter < 468)   ;   				// 1 second for 2340Hz using FindDAC_Fast_Successive();


	 ESIDAC1R0 = (Max_DAC_Ch0 + Min_DAC_Ch0)/2;
	 ESIDAC1R1 = ESIDAC1R0 + Noise_level;              // "+" for INV version, "-" for non-INV version
	 ESIDAC1R2 = (Max_DAC_Ch1 + Min_DAC_Ch1)/2;
	 ESIDAC1R3 = ESIDAC1R2 + Noise_level;              // "+" for INV version, "-" for non-INV version

	 AFE1_base0 = ESIDAC1R0;
	 AFE1_base1 = ESIDAC1R2;

	 ESIDAC1R0 -= Noise_level;                          // "-" for INV version, "+" for non-INV version
	 ESIDAC1R2 -= Noise_level;							// "-" for INV version, "+" for non-INV version

}


void InitScanIF(void)
{
	unsigned int i;
	volatile unsigned char*  PsmRamPointer;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	 Status_flag = 0;

//  Port pin selection for ESI, all channels for ESI should be selected, even only two channels are used.
//  The un-used pin should be floating, not be connected to any other circuit.

	P9SEL1 |= BIT0 + BIT1 + BIT2 + BIT3;
	P9SEL0 |= BIT0 + BIT1 + BIT2 + BIT3;            	// Select mux for ESI function


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	ESI control registers setting


	ESIAFE = ESIVCC2 + ESICA1INV + ESITEN;            	                                                    // AVCC/2 enable, Excitation enable
    ESITSM = ESITSMTRG1 + ESITSMTRG0 + ESIDIV3A0 + ESIDIV3A1;                                               // 2340Hz sampling rate
	ESIPSM = ESICNT2RST +ESICNT1RST + ESICNT0RST + ESICNT2EN +ESICNT1EN +ESICNT0EN;			                // ALL counters reset to zero, output TSM clock signal, enable all counters
	ESICTL = ESIS3SEL2 + ESIS2SEL0 + ESITCH10 + ESICS ;    													// OUT1 for PPUS2, OUT0 for PPUS1, no test cycle, ESI not enable yet


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// TSM Setting

	ESITSM0 = 0X0400;    // DAC=off, CA=off,  1xACLK for ACLK sync
	ESITSM1 = 0x202C;	 // DAC=off, CA=off,  5xESICLK, excitation  			    CH.0
	ESITSM2 = 0x0404;	 // DAC=off, CA=off,  1xACLK                				CH.0
	ESITSM3 = 0x0024;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.0
	ESITSM4 = 0x0024;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.0
	ESITSM5 = 0x0024;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.0
	ESITSM6 = 0x0024;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.0
	ESITSM7 = 0x0024;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.0
	ESITSM8 = 0x0024;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.0
	ESITSM9 = 0xF134;	 // DAC=on,  CA=on,   31xESIFCLK, 		          			CH.0
	ESITSM10 = 0x5974;	 // DAC=on,  CA=on,   OUTPUT LATCHES ENABLED,  12xESICLK 	CH.0
	ESITSM11 = 0X0400;   // DAC=off, CA=off,  1xACLCK, Internally damped          	CH.0
	ESITSM12 = 0X0400;   // DAC=off, CA=off,  1xACLCK, Internally damped          	CH.0
	ESITSM13 = 0x20AD;	 // DAC=off, CA=off,  5xESICLK, excitation  			    CH.1
	ESITSM14 = 0x0485;	 // DAC=off, CA=off,  1xACLK                				CH.1
	ESITSM15 = 0x00A5;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.1
	ESITSM16 = 0x00A5;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.1
	ESITSM17 = 0x00A5;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.1
	ESITSM18 = 0x00A5;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.1
	ESITSM19 = 0x00A5;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.1
	ESITSM20 = 0x00A5;	 // DAC=off, CA=off,  1xESIFCLK  , delay tunable  			CH.1
	ESITSM21 = 0xF1B5;	 // DAC=on,  CA=on,   31xESIFCLK			         		CH.1
	ESITSM22 = 0x59F5;	 // DAC=on,  CA=on,   OUTPUT LATCHES ENABLED,  12xESICLK 	CH.1
	ESITSM23 = 0x0200;	 // stop


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// Fill in ESIRAM TABLE for PSM

	PsmRamPointer = &ESIRAM0;

	for (i=0; i<16; i++)
	{
		*PsmRamPointer = Table[i];
		 PsmRamPointer +=1  ;
	}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	TSM_Auto_cal();						// Auto set the TSM delay
	Find_Noise_level();					// To find the noise level detected by ESI

	lcd_display_num(8888,0);           	// "8888" on LCD indicating the completion of TSM calibration.

	IIC_TX(0x2A); 					    // clockwise rotation

	Set_DAC();							// noise level found.
										// User need to switch on the motor with half-covered metal disc to finish the calibration

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Set the ESI control registers for normal operation
// now ready for normal operation


	 ESIAFE = ESIVCC2 + ESICA1INV + ESITEN;             													// disable AFE2;
	 ESITSM = ESITSMTRG1 + ESITSMTRG0 + ESIDIV3A0 + ESIDIV3A2 + ESIDIV3B0; 									// Soft start or ACLK divider trig for TSM sequence, Div by 66 (500Hz sampling rate)
	 ESIPSM = ESICNT2RST +ESICNT1RST + ESICNT0RST + ESIV2SEL +ESICNT2EN +ESICNT1EN +ESICNT0EN;				// ALL counters reset to zero, output TSM clock signal, enable all counters
	 ESIINT1 &= ~ESIIE5;

#if AFE2_enable
	 ReCal_Flag |= BIT5;          			// indication for a call from InitScanIF

 	 ESIINT2 &= ~ESIIFG5;                   // clear INT flag of Q6 of PSM
 	 ESIINT1 |= ESIIE5;						// enable INT of Q6
 	 ESICTL  |= ESIEN;

 	__bis_SR_register(LPM3_bits+GIE);   	// wait for the ESISTOP flag

	 ReCalScanIF();                			// to find the init AFE2_base

	 ESICTL &= ~ESIEN;
 	 ESIINT1 &= ~ESIIE5;					// disable INT of Q6
 	 ESIINT2 &= ~ESIIFG5;                   // clear INT flag of Q6 of PSM
	 ReCal_Flag = 0;

#endif



}



#if AFE2_enable
void ReCalScanIF(void)
{

#define delta_level 10

unsigned int Loop_counter = 0;
unsigned char Sensor_state;

int	AFE2_Min_DAC_Ch0 ;						//  value for DAC max and min
int	AFE2_Min_DAC_Ch1 ;
int	AFE2_Max_DAC_Ch0 ;
int	AFE2_Max_DAC_Ch1 ;

	ESIAFE = ESIDAC2EN + ESICA2EN + ESICA1INV + ESICA2INV + ESIVCC2 + ESITEN;            	// AVCC/2 enable, Excitation enable
	ESITSM = ESITSMTRG1 + ESITSMTRG0 + ESIDIV3A0 + ESIDIV3A1;                               // 2340Hz sampling rate

	AFE2_Min_DAC_Ch0 = 0 ;
	AFE2_Max_DAC_Ch0 = 0 ;
	AFE2_Min_DAC_Ch1 = 0 ;
	AFE2_Max_DAC_Ch1 = 0 ;

	 ESIINT2 &= ~ESIIFG1;
	 ESIINT1 |= ESIIE1;

do {

	if(ReCal_Flag&BIT6) {AFE2_FindDAC_Fast_Successive(AFE2_base0 + AFE2_drift0, AFE2_base1 + AFE2_drift1, 5);}
	else                {AFE2_FindDAC_Fast_Successive(AFE1_base0, AFE1_base1, 5);}


		Sensor_state = (char)(ESIPPU&0x0003);
		switch(Sensor_state)
		{

		case 0x00: {
						AFE2_Min_DAC_Ch0 += ESIDAC2R1;
					}
					break;

		case 0x01: {
						AFE2_Min_DAC_Ch1 += ESIDAC2R3;
					}
					break;

		case 0x02: 	{
						AFE2_Max_DAC_Ch1 += ESIDAC2R2;
					}
					break;

		case 0x03: {
						AFE2_Max_DAC_Ch0 += ESIDAC2R0;
					}
					break;

		}


	Loop_counter++;

	if(ReCal_Flag&BIT6)
	{

		ESIINT1 &= ~ESIIE1;


		if (Loop_counter == 16)															    // need 4 rotations for re-calibration
				{
				  ReCal_Flag |= BIT0;                                              	        // indication of valid calibration
				  break;
				}

		ESIINT2 &= ~ESIIFG5;                  												// clear the Q6 flag
		ESIINT1 |= ESIIE5;																	// Enable Q6 INT for in case of Time out.
		__bis_SR_register(LPM3_bits+GIE);   												// wait for the Q6 flag
		ESIINT1 &= ~ESIIE5;

	}
	else if (ReCal_Flag&BIT5)
	{
		 ESIINT1 &= ~ESIIE1;

		if (Loop_counter == 32)															    // need 8 rotations for initialization
				{
				  ReCal_Flag |= BIT0;                                              	        // indication of valid calibration
				  break;
				}

		__bis_SR_register(LPM3_bits+GIE);   												//	 wait for the Q6 flag

	}


	if(ReCal_Flag&BIT1)																		// inidcation of time out from Timer A
	{   __no_operation();
		break;}

	  ESIINT2 &= ~ESIIFG1;
	  ESIINT1 |= ESIIE1;


}while(Loop_counter < 40)   ;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// if BIT0 of ReCal_Flag is set, Max & Min of two channels found. set the ESIDAC with noise margin and AFE2_offset
// otherwise, it will be time out for re-calibration and no data for AFE1



if (ReCal_Flag&BIT0)
{
	if(ReCal_Flag&BIT6)
	{

	   AFE2_Max_DAC_Ch0 /= 4;
	   AFE2_Min_DAC_Ch0 /= 4;
	   AFE2_Max_DAC_Ch1 /= 4;
	   AFE2_Min_DAC_Ch1 /= 4;

	   AFE2_drift0 = (AFE2_Max_DAC_Ch0 + AFE2_Min_DAC_Ch0)/2 - AFE2_base0;

	   New_level  = AFE1_base0 + AFE2_drift0;

	   Delta = (ESIDAC1R0+ESIDAC1R1)/2 - New_level;

	   if (abs(Delta) < delta_level )
	   {
	   ESIDAC1R0 = New_level - Noise_level;               // Noise_level, "-" for INV version, "+" for non-INV version
	   ESIDAC1R1 = New_level + Noise_level;				  // Noise_level, "+" for INV version, "-" for non-INV version
	   }

	   AFE2_drift1 = (AFE2_Max_DAC_Ch1 + AFE2_Min_DAC_Ch1)/2 - AFE2_base1;

	   New_level  = AFE1_base1 + AFE2_drift1;

	   Delta = (ESIDAC1R2+ESIDAC1R3)/2 - New_level;

	   if (abs(Delta) < delta_level )
	   {
	   ESIDAC1R2 = New_level - Noise_level;               // Noise_level, "-" for INV version, "+" for non-INV version
	   ESIDAC1R3 = New_level + Noise_level;               // Noise_level, "+" for INV version, "-" for non-INV version
	   }

	}
	else if(ReCal_Flag&BIT5)                              // call from InitScanIF only to get AFE2 base value
			{

				AFE2_base0_Max = AFE2_Max_DAC_Ch0/8;
				AFE2_base0_Min = AFE2_Min_DAC_Ch0/8;
				AFE2_base1_Max = AFE2_Max_DAC_Ch1/8;
				AFE2_base1_Min = AFE2_Min_DAC_Ch1/8;

				AFE2_base0  = (AFE2_base0_Max + AFE2_base0_Min)/2;
				AFE2_base1  = (AFE2_base1_Max + AFE2_base1_Min)/2;

			}
}


		 ESIAFE = ESIVCC2 + ESICA1INV + ESITEN;                                  				// disable AFE2 when completed;
		 ESITSM = ESITSMTRG1 + ESITSMTRG0 + ESIDIV3A0 + ESIDIV3A2 + ESIDIV3B0;		            // Back to 500Hz
		 ESIINT1 &= ~ESIIE1;																	// disable ESISTOP INT

}




void AFE2_FindDAC_Fast_Range(int Starting_point_ch0, int Starting_point_ch1, int Range_num)
{

unsigned int Range;
unsigned char Range_status;


	ESIDAC2R0 = Starting_point_ch0;             // set DAC from the starting point
	ESIDAC2R1 = Starting_point_ch0;            	// set DAC from the starting point

	ESIDAC2R2 = Starting_point_ch1;             // set DAC from the starting point
	ESIDAC2R3 = Starting_point_ch1;            	// set DAC from the starting point


	Range_status = 0;
	Range_status |= BIT7;                       // This is the searching Loop enable bit.

	Range = Range_num;


// This loop is to find the starting point of DAC and the direction of searching by adding or subtracting a value of "Range" into or from the DAC
// Range_status is to show the direction of searching
// BIT0   indication of a searching direction of increasing for channel 0;
// BIT1   indication of a searching direction of decreasing for channel 0;
// BIT2   indication of a searching direction of increasing for channel 1;
// BIT3   indication of a searching direction of decreasing for channel 1;
// BIT4   indication of completion of the loop for channel 0;
// BIT4   indication of completion of the loop for channel 1;


while (Range_status&BIT7)
{
		while(!((Range_status&BIT4) && (Range_status&BIT5)))
		{

					__bis_SR_register(LPM3_bits+GIE);   	//	 wait for the ESISTOP flag


				   if(!(Range_status&BIT4))
				   {
					if (ESIPPU&ESIOUT4)                     // channel 0;
						{

							if(Range_status&BIT1)			// check if there is a decreasing direction mark of channel 0.
							{ Range_status |= BIT4;}        // if yes, put a completion mark for channel 0
							else
							{ Range_status |= BIT0;}        // if no, put a direction mark pointing to increasing for channel 0.

							ESIDAC2R0 -= Range;     		// decrease the DAC by a value of "Range"
							ESIDAC2R1 -= Range;

						}
					else
						{

							if(Range_status&BIT0)			// check if there is a increasing direction mark of channel 0.
							{ Range_status |= BIT4;}        // if yes, put a completion mark for channel 0
							else
							{ Range_status |= BIT1;}        // if no, put a direction mark pointing to decreasing for channel 0.

							ESIDAC2R0 += Range;				// increase the DAC by a value of "Range"
							ESIDAC2R1 += Range;
						}
				   }


				  if(!(Range_status&BIT5))
				  {
					if (ESIPPU&ESIOUT5)						// channel 1;
						{

							if(Range_status&BIT3)			// check if there is a decreasing direction mark of channel 1.
							{ Range_status |= BIT5;}        // if yes, put a completion mark for channel 1
							else
							{ Range_status |= BIT2;}        // if no, put a direction mark pointing to increasing for channel 1.

							ESIDAC2R2 -= Range;     		// decrease the DAC by a value of "Range"
							ESIDAC2R3 -= Range;

						}
					else
						{

							if(Range_status&BIT2)			// check if there is a increasing direction mark of channel 1.
							{ Range_status |= BIT5;}        // if yes, put a completion mark for channel 1
							else
							{ Range_status |= BIT3;}        // if no, put a direction mark pointing to decreasing for channel 1.

							ESIDAC2R2 += Range;				// increase the DAC by a value of "Range"
							ESIDAC2R3 += Range;
						}
				  }

		}

	if (Range > 1)
	{	Range_status &= ~(BIT4+BIT5);            			// clear the flag of completion
		Range_status &= ~(BIT0+BIT1+BIT2+BIT3);				// optional, clear the direction flag
		Range = 1;                                 			// This is to restart the loop with searching value of one, by "+/- 1 method"
	}
	else
	{   Range_status &= ~BIT7;}								// "+/- 1 method completed, the loop end here

}


}


void AFE2_FindDAC(void)
{
	unsigned int i;
	unsigned int DAC_BIT = 0, Prev_DAC_BIT = 0;

	DAC_BIT   = 0x0800;						// DAC Level tester, using Sucessive approx approach
	Prev_DAC_BIT = 0x0C00;

	ESIDAC2R0 = DAC_BIT;                 	// set as the middle point
	ESIDAC2R1 = DAC_BIT;                 	// set as the middle point

	ESIDAC2R2 = DAC_BIT;                 	// set as the middle point
	ESIDAC2R3 = DAC_BIT;                 	// set as the middle point


	for(i = 0; i<12; i++)				 	// test 12 times as 12 bit DAC
	{

	__bis_SR_register(LPM3_bits+GIE);   	// wait for the ESISTOP flag

	DAC_BIT /= 2 ;							// right shift one bit

	if (!(ESIPPU&ESIOUT4))                  // channel 0;
		{

			ESIDAC2R0 |= DAC_BIT;     		// keep the previous bit and set the next bit
			ESIDAC2R1 |= DAC_BIT;			// keep the previous bit and set the next bit

		}
	else
		{
			ESIDAC2R0 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
			ESIDAC2R1 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
		}

	if (!(ESIPPU&ESIOUT5))					// channel 1;
		{

			ESIDAC2R2 |= DAC_BIT;     		// keep the previous bit and set the next bit
			ESIDAC2R3 |= DAC_BIT;			// keep the previous bit and set the next bit

		}
	else
		{
			ESIDAC2R2 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
			ESIDAC2R3 ^= Prev_DAC_BIT;		// reset the previous bit and set the next bit
		}

	Prev_DAC_BIT /= 2;						// right shift one bit

	}

}


void AFE2_FindDAC_Fast_Successive(int Starting_point_ch0, int Starting_point_ch1, int Range_num)
{
	unsigned int i;
	unsigned int DAC_BIT = 0;

	DAC_BIT   = 0x0001;						       // DAC Level tester, using Sucessive approx approach

	ESIDAC2R0 = Starting_point_ch0;                // set as the middle point
	ESIDAC2R1 = Starting_point_ch0;                // set as the middle point

	ESIDAC2R2 = Starting_point_ch1;                // set as the middle point
	ESIDAC2R3 = Starting_point_ch1;                // set as the middle point

	for(i=1; i<Range_num; i++)
	{

	   DAC_BIT <<= 1;

	}

	for(i = 0; i<Range_num; i++)				 	// test "Range_num" times as 12 bit DAC
	{

	__bis_SR_register(LPM3_bits+GIE);   			//	 wait for the ESISTOP flag


	if (!(ESIPPU&ESIOUT4))                     		// channel 0;
		{

			ESIDAC2R0 += DAC_BIT;
			ESIDAC2R1 += DAC_BIT;

		}
	else
		{
			ESIDAC2R0 -= DAC_BIT;
			ESIDAC2R1 -= DAC_BIT;
		}

	if (!(ESIPPU&ESIOUT5))							// channel 1;
		{

			ESIDAC2R2 += DAC_BIT;
			ESIDAC2R3 += DAC_BIT;

		}
	else
		{
			ESIDAC2R2 -= DAC_BIT;
			ESIDAC2R3 -= DAC_BIT;
		}

		DAC_BIT /= 2;								// right shift one bit

	}

}


#endif

