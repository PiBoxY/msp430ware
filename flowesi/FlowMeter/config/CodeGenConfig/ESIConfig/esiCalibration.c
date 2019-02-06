<FMCG_COPYRIGHT>
<FMCG_DATE>
<FMCG_OVERWRITE_DO_NOT_MODIFY>

#include "main.h"

// Global variables
<FMCG_ESICAL_GLOBAL_VARS>
uint16_t gNoiseLevel = 0;
uint8_t  gESIStatusFlag = 0;
uint8_t  gStateSeparation = 0;




extern ESI_AFE2_InitParams gAFE2Settings;
extern ESI_TSM_InitParams gTSMSettings;



// Function Prototype
static void FindDAC(findDACParams *params);
static void FindDAC_Fast_Range(fastParams *params);
static void FindDAC_Fast_Successive(fastParams *params);
<FMCG_ESICAL_C_RECAL_FUNC_PROT>



void TSM_Auto_cal(void)
{
	// constant and variable for TSM calibration
#define CYCLE_WIDTH 8      		// which is equal to (ESICLK / freq of LC) - 2
#define LC_THRESHOLD_TSM_CAL 1600
#define CH0_FINISH  BIT0
#define CH1_FINISH  BIT1
#define CH2_FINISH  BIT2

	int8_t calStatus = 0;
	int16_t mathTemp;
	uint16_t i;
	findDACParams fparams= {0};
<FMCG_ESICAL_C_TSM_AUTO_LOC_VAR>	


/**
 *
 * This module is used to find the signal level of LC oscillation for
 * ESICH0, ESICH1, ESICH2 or ESICH3 depending on user selection.
 * This calibration updates calibration delay:
 *
 *     -TSM state 3  -  7  for CH0
 *     -TSM state 13 - 17  for CH1
 *     -TSM state 23 - 27  for CH2
 *
 */

<FMCG_ESICAL_C_TSM_AUTO_CONFIG>


/*
 *
 * START of TSM Auto-calibration
 *
 */

	do{
		// Find DAC threshold using 12-BIT successive approximation
		FindDAC(&fparams);

<FMCG_ESICAL_C_TSM_AUTO_DAC_READ>

<FMCG_ESICAL_C_TSM_AUTO_CAL_METHOD>


<FMCG_ESICAL_C_TSM_AUTO_END_WHILE>

/*
 *
 * TSM Auto-calibration complete
 *
 */

}

void Find_Noise_level(void)
{
#define SEARCH_RANGE  							8
#define NOISE_LEVEL_DETECTION_TIME 				256

	uint16_t loopCounter = 0;
	fastParams fParams = {0};

    // set initial value for DAC max and min
	// this variable will record the DAC value of metal and non metal part of a rotor
<FMCG_ESICAL_C_FIND_NOISE_FPARAM_CONFIG>	
	fParams.range = SEARCH_RANGE;

/**
 *
 *	Start of Noise-Level Detection
 *
 */

	// Loop for detection of noise level, taking 0.5 second;


	do {

<FMCG_ESICAL_C_FIND_NOISE_DAC_STARTING_POINT>

		FindDAC_Fast_Range(&fParams);

<FMCG_ESICAL_C_FIND_NOISE_CH_MIN_MAX>

		loopCounter++;

	}while (loopCounter < NOISE_LEVEL_DETECTION_TIME); // run for approx 0.5 second

/**
 *
 *	End of Noise-Level Detection
 *
 */

<FMCG_ESICAL_C_FIND_NOISE_THRESHOLD>


}

//******************************************************************************
//
// When the disc is rotating, the Max and Min of signal is found and their
// difference is required to be larger than gStateSeparation.
//
// After reaching the gStateSeparation, it will keep rotating for one more
// second to ensure a complete rotation is calibrated.
//
//******************************************************************************

void Set_DAC(void)
{
// min 4 ,  max 6;
#define SEPARATION_FACTOR   				4           

	uint16_t loopCounter = 0;
	fastParams fsParams= {0};

<FMCG_ESICAL_C_SET_DAC_LOCAL_VAR>

	fsParams.range = 5;
<FMCG_ESICAL_C_SET_DAC_FSPARAM_CONFIG>

	// minimum times 4, maximum 6;
	gStateSeparation = gNoiseLevel*(SEPARATION_FACTOR-1)+gNoiseLevel/2;


	 // do loop for 1 more second after valid separation detected;
	do {
		// do loop for detection of valid Max-Min separation;
		do {
<FMCG_ESICAL_C_SET_DAC_DAC_STARTING_POINT>

			FindDAC_Fast_Successive(&fsParams);

<FMCG_ESICAL_C_SET_DAC_CH_MIN_MAX>


			// To detect the a change due to rotation
			// if a separation of gStateSeparation is found, a rotation is
			// detected and keep running for 1 second to find the max and min
<FMCG_ESICAL_C_SET_DAC_SET_THRESHOLD>

			// check for valid separation
<FMCG_ESICAL_C_SET_DAC_VALID_SEPARATION>
<FMCG_ESICAL_C_SET_DAC_END_WHILE_VALID>

		loopCounter++;
	} while(loopCounter < 468)   ;   // 1 second for 2340Hz using Find_Fast_Successive();


	// "+" for INV version, "-" for non-INV version
<FMCG_ESICAL_C_SET_DAC_SETUP_DACS>

}


<FMCG_ESICAL_RECAL_USING_AFE2>


static void FindDAC(findDACParams *params)
{
	uint8_t i;
	// DAC Level tester, using Successive approximation approach
	uint16_t currentDACBit = 0x0800, prevDACBit = 0x0C00;

	// Set currentDACBit as initial value for all DAC register specified in
	// channelMask
	
	if(params->channel0Enabled==true){
		ESI_setAFE1DACValue(currentDACBit,CHANNEL_0_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(currentDACBit,CHANNEL_0_DAC1_LOWER_REGISTER);
	}

	if(params->channel1Enabled==true){
		ESI_setAFE1DACValue(currentDACBit,CHANNEL_1_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(currentDACBit,CHANNEL_1_DAC1_LOWER_REGISTER);
	}

	if(params->channel2Enabled==true){
		ESI_setAFE1DACValue(currentDACBit,CHANNEL_2_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(currentDACBit,CHANNEL_2_DAC1_LOWER_REGISTER);
	}


	// Clear any pending ESISTOP Interrupt and enable ESI module
	ESI_clearInterrupt(ESI_INTERRUPT_FLAG_ESISTOP);
	ESI_enableInterrupt(ESI_INTERRUPT_ESISTOP);
	ESI_enable();


	// Start DAC successive approximation routine for selected channels
	for(i = 0; i<12; i++)				 	// test 12 times as 12 bit DAC
	{

		// Wait for the ESISTOP flag
		__bis_SR_register(LPM3_bits+GIE);

		// right shift one bit
		currentDACBit >>= 1;


		// Channel 0
		if(params->channel0Enabled==true){
			if (!ESI_getLatchedComparatorOutput(CHANNEL_0_PPU_SELECT)){
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_UPPER_REGISTER) | currentDACBit,CHANNEL_0_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_LOWER_REGISTER) | currentDACBit,CHANNEL_0_DAC1_LOWER_REGISTER);
			}else{
				// reset the previous bit and set the next bit
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_UPPER_REGISTER) ^ prevDACBit,CHANNEL_0_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_LOWER_REGISTER) ^ prevDACBit,CHANNEL_0_DAC1_LOWER_REGISTER);
			}
		}

		// Channel 1
		if(params->channel1Enabled==true){
			if (!ESI_getLatchedComparatorOutput(CHANNEL_1_PPU_SELECT)){
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_UPPER_REGISTER) | currentDACBit,CHANNEL_1_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_LOWER_REGISTER) | currentDACBit,CHANNEL_1_DAC1_LOWER_REGISTER);
			}else{
				// reset the previous bit and set the next bit
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_UPPER_REGISTER) ^ prevDACBit,CHANNEL_1_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_LOWER_REGISTER) ^ prevDACBit,CHANNEL_1_DAC1_LOWER_REGISTER);
			}
		}

		// Channel 2
		if(params->channel2Enabled==true){
			if (!ESI_getLatchedComparatorOutput(CHANNEL_2_PPU_SELECT)){
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_UPPER_REGISTER) | currentDACBit,CHANNEL_2_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_LOWER_REGISTER) | currentDACBit,CHANNEL_2_DAC1_LOWER_REGISTER);
			}else{
				// reset the previous bit and set the next bit
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_UPPER_REGISTER) ^ prevDACBit,CHANNEL_2_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_LOWER_REGISTER) ^ prevDACBit,CHANNEL_2_DAC1_LOWER_REGISTER);
			}
		}

		// right shift one bit
		prevDACBit >>= 1;


	}

	ESI_disable();
	ESI_disableInterrupt(ESI_INTERRUPT_ESISTOP);

}



static void FindDAC_Fast_Successive(fastParams *params)
{
	uint16_t i;
	uint16_t dACBit = 1;


	// set startingPointCHx
	if(params->channel0Enabled==true){
		ESI_setAFE1DACValue(params->channel0StartingPoint,
				CHANNEL_0_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(params->channel0StartingPoint,
				CHANNEL_0_DAC1_LOWER_REGISTER);
	}
	if(params->channel1Enabled==true){
		ESI_setAFE1DACValue(params->channel1StartingPoint,
				CHANNEL_1_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(params->channel1StartingPoint,
				CHANNEL_1_DAC1_LOWER_REGISTER);
	}
	if(params->channel2Enabled==true){
		ESI_setAFE1DACValue(params->channel2StartingPoint,
				CHANNEL_2_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(params->channel2StartingPoint,
				CHANNEL_2_DAC1_LOWER_REGISTER);
	}


/*******************************************************************************
 * Sets dACBit to proper range based on rangeNum
 ******************************************************************************/
	for(i=1; i<params->range; i++){
		dACBit <<= 1;
	}

/*******************************************************************************
 * Turns on ESI module and enables ESISTOP flag
 ******************************************************************************/
	ESI_clearInterrupt(ESI_INTERRUPT_FLAG_ESISTOP);
	ESI_enableInterrupt(ESI_INTERRUPT_ESISTOP);
	ESI_enable();



	 // this for loop is to find an initial DAC value for ch0 and ch1

	for(i = 0; i<params->range; i++){			// test "rangeNum" times as 12 bit DAC

		__bis_SR_register(LPM3_bits+GIE);   	//	 wait for the ESISTOP flag


		/**
		 * Determine if ESI_CHANNEL_0 was detected as 0 or 1
		 *
		 * -If "0" increment ESIDAC1R2 and ESIDAC1R3 value
		 * -If "1" decrement ESIDAC1R2 and ESIDAC1R3 value
		 *
		 */
		if(params->channel0Enabled==true){
			if(!ESI_getLatchedComparatorOutput(CHANNEL_0_PPU_SELECT)){
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_UPPER_REGISTER) + dACBit, CHANNEL_0_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_LOWER_REGISTER) + dACBit, CHANNEL_0_DAC1_LOWER_REGISTER);
			} else {
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_UPPER_REGISTER) - dACBit, CHANNEL_0_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_LOWER_REGISTER) - dACBit, CHANNEL_0_DAC1_LOWER_REGISTER);
			}
		}

		/**
		 * Determine if ESI_CHANNEL_1 was detected as 0 or 1
		 *
		 * -If "0" increment ESIDAC1R4 and ESIDAC1R5 value
		 * -If "1" decrement ESIDAC1R4 and ESIDAC1R5 value
		 *
		 */
		if(params->channel1Enabled==true){
			if(!ESI_getLatchedComparatorOutput(CHANNEL_1_PPU_SELECT)){
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_UPPER_REGISTER) + dACBit, CHANNEL_1_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_LOWER_REGISTER) + dACBit, CHANNEL_1_DAC1_LOWER_REGISTER);
			} else {
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_UPPER_REGISTER) - dACBit, CHANNEL_1_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_LOWER_REGISTER) - dACBit, CHANNEL_1_DAC1_LOWER_REGISTER);
			}
		}
		/**
		 * Determine if ESI_CHANNEL_2 was detected as 0 or 1
		 *
		 * -If "0" increment ESIDAC1R6 and ESIDAC1R7 value
		 * -If "1" decrement ESIDAC1R6 and ESIDAC1R7 value
		 *
		 */
		if(params->channel2Enabled==true){
			if(!ESI_getLatchedComparatorOutput(CHANNEL_2_PPU_SELECT)){
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_UPPER_REGISTER) + dACBit, CHANNEL_2_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_LOWER_REGISTER) + dACBit, CHANNEL_2_DAC1_LOWER_REGISTER);
			} else {
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_UPPER_REGISTER) - dACBit, CHANNEL_2_DAC1_UPPER_REGISTER);
				ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_LOWER_REGISTER) - dACBit, CHANNEL_2_DAC1_LOWER_REGISTER);
			}
		}

		dACBit >>= 1;

	}

	ESI_disable();
	ESI_clearInterrupt(ESI_INTERRUPT_FLAG_ESISTOP);

}



static void FindDAC_Fast_Range(fastParams *params)
{
#define CH0_INCREASING_SEARCH_DIR						BIT0
#define CH0_DECREASING_SEARCH_DIR						BIT1
#define CH1_INCREASING_SEARCH_DIR						BIT2
#define CH1_DECREASING_SEARCH_DIR						BIT3
#define CH2_INCREASING_SEARCH_DIR						BIT4
#define CH2_DECREASING_SEARCH_DIR						BIT5
#define CH0_COMPLETE									BIT8
#define CH1_COMPLETE									BIT9
#define CH2_COMPLETE									BITA
#define ENABLE_SEARCHING_LOOP							BITF

	uint16_t range= params->range;
	uint16_t rangeStatus=0;

	// Set DAC starting point
	if(params->channel0Enabled==true){
		ESI_setAFE1DACValue(params->channel0StartingPoint,CHANNEL_0_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(params->channel0StartingPoint,CHANNEL_0_DAC1_LOWER_REGISTER);
	}

	if(params->channel1Enabled==true){
		ESI_setAFE1DACValue(params->channel1StartingPoint,CHANNEL_1_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(params->channel1StartingPoint,CHANNEL_1_DAC1_LOWER_REGISTER);
	}

	if(params->channel2Enabled==true){
		ESI_setAFE1DACValue(params->channel2StartingPoint,CHANNEL_2_DAC1_UPPER_REGISTER);
		ESI_setAFE1DACValue(params->channel2StartingPoint,CHANNEL_2_DAC1_LOWER_REGISTER);
	}

	// This is the searching Loop enable bit.
	rangeStatus |= ENABLE_SEARCHING_LOOP;

	// Clear any pending ESISTOP Interrupt and enable ESI module
	ESI_clearInterrupt(ESI_INTERRUPT_FLAG_ESISTOP);
	ESI_enableInterrupt(ESI_INTERRUPT_ESISTOP);
	ESI_enable();


/******************************************************************************
 *
 * This loop is to find the starting point of DAC and the direction of searching
 * by adding or subtracting a value of "Range" into or from the DAC
 *
 ******************************************************************************/

	while (rangeStatus&ENABLE_SEARCHING_LOOP){
<FMCG_ESICAL_C_FIND_DAC_FAST_RANGE_DAC_WHILE_START>
			//	 wait for the ESISTOP flag
			__bis_SR_register(LPM3_bits+GIE);

			if(params->channel0Enabled ==true){
				if(!(rangeStatus&CH0_COMPLETE)){
					// channel 0;
					if (ESI_getLatchedComparatorOutput(CHANNEL_0_PPU_SELECT)){
						// check if there is a decreasing direction mark of channel 0.
						// if yes, put a completion mark for channel 0
						// if no, put a direction mark pointing to increasing for channel 0.
						if(rangeStatus&CH0_DECREASING_SEARCH_DIR){
							rangeStatus |= CH0_COMPLETE;
						}else{
							rangeStatus |= CH0_INCREASING_SEARCH_DIR;
						}

						// decrease the CHO DAC registers by a value of "range"
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_UPPER_REGISTER) - range,CHANNEL_0_DAC1_UPPER_REGISTER);
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_LOWER_REGISTER) - range,CHANNEL_0_DAC1_LOWER_REGISTER);

					}else{
						// check if there is a increasing direction mark of channel 0.
						// if yes, put a completion mark for channel 0
						// if no, put a direction mark pointing to decreasing for channel 0.
						if(rangeStatus&CH1_INCREASING_SEARCH_DIR){
							rangeStatus |= CH0_COMPLETE;
						} else{
							rangeStatus |= CH0_DECREASING_SEARCH_DIR;
						}
						// increase the CHO DAC registers by a value of "range"
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_UPPER_REGISTER) + range,CHANNEL_0_DAC1_UPPER_REGISTER);
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_0_DAC1_LOWER_REGISTER) + range,CHANNEL_0_DAC1_LOWER_REGISTER);
					}
				}
			}

			if(params->channel1Enabled==true){
				if(!(rangeStatus&CH1_COMPLETE)){
					// channel 1;
					if (ESI_getLatchedComparatorOutput(CHANNEL_1_PPU_SELECT)){
						// check if there is a decreasing direction mark of channel 1.
						// if yes, put a completion mark for channel 1
						// if no, put a direction mark pointing to increasing for channel 1.
						if(rangeStatus&CH1_DECREASING_SEARCH_DIR){
							rangeStatus |= CH1_COMPLETE;
						} else {
							rangeStatus |= CH1_INCREASING_SEARCH_DIR;
						}
						// decrease the CH1 DAC registers by a value of "range"
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_UPPER_REGISTER) - range,CHANNEL_1_DAC1_UPPER_REGISTER);
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_LOWER_REGISTER) - range,CHANNEL_1_DAC1_LOWER_REGISTER);
					} else {
						// check if there is a increasing direction mark of channel 1.
						// if yes, put a completion mark for channel 1
						// if no, put a direction mark pointing to decreasing for channel 1.
						if(rangeStatus&CH1_INCREASING_SEARCH_DIR){
							rangeStatus |= CH1_COMPLETE;
						} else{
							rangeStatus |= CH1_DECREASING_SEARCH_DIR;
						}
						// increase the CH1 DAC registers by a value of "range"
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_UPPER_REGISTER) + range,CHANNEL_1_DAC1_UPPER_REGISTER);
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_1_DAC1_LOWER_REGISTER) + range,CHANNEL_1_DAC1_LOWER_REGISTER);
					}
				}
			}

			if(params->channel2Enabled ==true){
				if(!(rangeStatus&CH2_COMPLETE)){
					// channel 1;
					if (ESI_getLatchedComparatorOutput(CHANNEL_2_PPU_SELECT)){
						// check if there is a decreasing direction mark of channel 1.
						// if yes, put a completion mark for channel 1
						// if no, put a direction mark pointing to increasing for channel 1.
						if(rangeStatus&CH2_DECREASING_SEARCH_DIR){
							rangeStatus |= CH2_COMPLETE;
						} else {
							rangeStatus |= CH2_INCREASING_SEARCH_DIR;
						}
						// decrease the CH1 DAC registers by a value of "range"
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_UPPER_REGISTER) - range,CHANNEL_2_DAC1_UPPER_REGISTER);
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_LOWER_REGISTER) - range,CHANNEL_2_DAC1_LOWER_REGISTER);
					} else {
						// check if there is a increasing direction mark of channel 1.
						// if yes, put a completion mark for channel 1
						// if no, put a direction mark pointing to decreasing for channel 1.
						if(rangeStatus&CH2_INCREASING_SEARCH_DIR){
							rangeStatus |= CH2_COMPLETE;
						} else{
							rangeStatus |= CH2_DECREASING_SEARCH_DIR;
						}
						// increase the CH1 DAC registers by a value of "range"
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_UPPER_REGISTER) + range,CHANNEL_2_DAC1_UPPER_REGISTER);
						ESI_setAFE1DACValue(ESI_getAFE1DACValue(CHANNEL_2_DAC1_LOWER_REGISTER) + range,CHANNEL_2_DAC1_LOWER_REGISTER);
					}
				}
			}
		}

		if (range > 1){
			// clear the flag of completion
			rangeStatus &= ~(CH0_COMPLETE + CH1_COMPLETE + CH2_COMPLETE);
			// optional, clear the direction flag
			rangeStatus &= ~(CH0_INCREASING_SEARCH_DIR +
							 CH0_DECREASING_SEARCH_DIR +
							 CH1_INCREASING_SEARCH_DIR +
							 CH1_DECREASING_SEARCH_DIR +
							 CH2_INCREASING_SEARCH_DIR +
							 CH2_DECREASING_SEARCH_DIR);
			// This is to restart the loop with searching value of one, by "+/- 1 method"
			range = 1;
		} else {
			// "+/- 1 method completed, the loop end here
			rangeStatus &= ~ENABLE_SEARCHING_LOOP;
		}

	}

	ESI_disable();
	ESI_disableInterrupt(ESI_INTERRUPT_ESISTOP);

}



















