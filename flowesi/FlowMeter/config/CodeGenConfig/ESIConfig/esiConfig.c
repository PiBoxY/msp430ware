<FMCG_COPYRIGHT>
<FMCG_DATE>
<FMCG_OVERWRITE_WILL_BE_OVERWRITE>

#include "main.h"


ESI_AFE1_InitParams gAFE1Settings;
<FMCG_CONF_C_AFE2_STRUCT>
ESI_TSM_InitParams gTSMSettings;
ESI_PSM_InitParams gPSMSettings;
extern int16_t gRotationalCounter;
extern uint8_t  gESIStatusFlag;
<FMCG_CONF_C_EXTERN_DEF>




void (*int_0_callback)(void);
void (*int_3_callback)(void);
void (*int_4_callback)(void);
void (*int_5_callback)(void);
void (*int_6_callback)(void);
void (*int_7_callback)(void);
void (*int_8_callback)(void);
void esiRegINTCallback(uint16_t int_num, void function(void));
void esiUnregINTCallback(uint16_t int_num);
<FMCG_CONF_C_PROTOTYPES>




<FMCG_CONF_C_TSM_TABLE>
<FMCG_CONF_C_PSM_TABLE>



//******************************************************************************
//
//! \\brief Configures ESI Module
//!
//! This function is automatically generated.
//!
//! \\return None
//
//******************************************************************************
void esiConfig(void)
{
	// Init the status flag with non-runtime calibration
<FMCG_CONF_C_GRECAL_FLAG>
	gESIStatusFlag = 0;
	
	// Clear all AFE1 and AFE2 DAC values
	ESI_setAFE1DACValue(0, ESI_DAC1_REG0);
	ESI_setAFE1DACValue(0, ESI_DAC1_REG1);
	ESI_setAFE1DACValue(0, ESI_DAC1_REG2);
	ESI_setAFE1DACValue(0, ESI_DAC1_REG3);
	ESI_setAFE1DACValue(0, ESI_DAC1_REG4);
	ESI_setAFE1DACValue(0, ESI_DAC1_REG5);
	ESI_setAFE1DACValue(0, ESI_DAC1_REG6);
	ESI_setAFE1DACValue(0, ESI_DAC1_REG7);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG0);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG1);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG2);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG3);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG4);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG5);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG6);
	ESI_setAFE2DACValue(0, ESI_DAC2_REG7);
	
<FMCG_CONF_C_INTERRUPTS>

<FMCG_CONF_C_ESIOSC_CALIB>	
	

	// Port pin selection for ESI, all channels for ESI should be selected, even
	// if not all ESICHx channels are used.
	// The un-used pin should be floating, not connected to any other circuit.

	GPIO_setAsPeripheralModuleFunctionInputPin(
			ESICHX_PORT,
			ESICH0_PIN | ESICH1_PIN | ESICH2_PIN | ESICH3_PIN,
			GPIO_TERNARY_MODULE_FUNCTION);
			
			
	// Disabling input buffer for COMP E can help reduce the leakage current of 
	// I/O.
	Comp_E_disableInputBuffer(COMP_E_BASE,
			COMP_E_INPUT8 | COMP_E_INPUT9 | COMP_E_INPUT10 |
			COMP_E_INPUT11| COMP_E_INPUT12| COMP_E_INPUT13 |
			COMP_E_INPUT14| COMP_E_INPUT15);		

	// Initialization of AFE1
	gAFE1Settings  = ESI_AFE1_INITPARAMS_DEFAULT;
	gAFE1Settings.excitationCircuitSelect = ESI_EXCITATION_CIRCUIT_ENABLED;
	gAFE1Settings.midVoltageGeneratorSelect = ESI_MID_VOLTAGE_GENERATOR_ENABLED;
	gAFE1Settings.sampleAndHoldSelect = ESI_SAMPLE_HOLD_VSS_BY_TSM;
	gAFE1Settings.inverterSelectOutputAFE1 = ESI_INVERTER_FOR_AFE1_ENABLE;
	ESI_AFE1_init(&gAFE1Settings);
	

	// Initialization of TSM with sampling frequency to ~2340 KHz for initial
	// calibration routines
	gTSMSettings = ESI_TSM_INITPARAMS_DEFAULT;
	gTSMSettings.startTriggerAclkDivider = ESI_TSM_START_TRIGGER_DIV_14;
	gTSMSettings.startTriggerSelection = ESI_TSM_START_TRIGGER_ACLK_OR_SOFTWARE;
	ESI_TSM_init(&gTSMSettings);

	// Initialization of PSM.
	gPSMSettings = ESI_PSM_INITPARAMS_DEFAULT;
	gPSMSettings.count0Select = ESI_PSM_CNT0_ENABLE;
	gPSMSettings.count0Reset = ESI_PSM_CNT0_RESET;
	gPSMSettings.count1Select = ESI_PSM_CNT1_ENABLE;
	gPSMSettings.count1Reset = ESI_PSM_CNT1_RESET;
	gPSMSettings.count2Select = ESI_PSM_CNT2_ENABLE;
	gPSMSettings.count2Reset = ESI_PSM_CNT2_RESET;
<FMCG_CONF_C_PSM_CONFIG_V2_SEL>
	ESI_PSM_init(&gPSMSettings);

<FMCG_CONF_C_PSM_SOURCE_SEL>
	
	ESI_timerAInputSelect(ESI_TIMERA_INPUT_TSM_PPUSRC);

	ESI_TSM_copyTable(gTSMTable, sizeof(gTSMTable));

	ESI_PSM_copyTable(gPSMTable,sizeof(gPSMTable));


	TSM_Auto_cal();

	Find_Noise_level();
	
<FMCG_CONF_C_INIT_CAL_DONE>	
	
	// User need to switch on the motor with half-covered metal disc to
	// finish the calibration.
	Set_DAC();
	
	// Set the ESI control registers for normal operation
	gAFE1Settings = ESI_AFE1_INITPARAMS_DEFAULT;
	gAFE1Settings.inverterSelectOutputAFE1 = ESI_INVERTER_FOR_AFE1_ENABLE;
	gAFE1Settings.midVoltageGeneratorSelect = ESI_MID_VOLTAGE_GENERATOR_ENABLED;
	gAFE1Settings.excitationCircuitSelect = ESI_EXCITATION_CIRCUIT_ENABLED;
	ESI_AFE1_init(&gAFE1Settings);
	
	// Set sampling frequency to User selected sampling frequency
	gTSMSettings = ESI_TSM_INITPARAMS_DEFAULT;
	gTSMSettings.startTriggerSelection = ESI_TSM_START_TRIGGER_ACLK_OR_SOFTWARE;
<FMCG_CONF_C_SAMPLING_FREQ_CONFIG>
	ESI_TSM_init(&gTSMSettings);
	
		// Initialization of PSM.
	gPSMSettings = ESI_PSM_INITPARAMS_DEFAULT;
	gPSMSettings.count0Select = ESI_PSM_CNT0_ENABLE;
	gPSMSettings.count0Reset = ESI_PSM_CNT0_RESET;
	gPSMSettings.count1Select = ESI_PSM_CNT1_ENABLE;
	gPSMSettings.count1Reset = ESI_PSM_CNT1_RESET;
	gPSMSettings.count2Select = ESI_PSM_CNT2_ENABLE;
	gPSMSettings.count2Reset = ESI_PSM_CNT2_RESET;
<FMCG_CONF_C_PSM_CONFIG_V2_SEL>
	ESI_PSM_init(&gPSMSettings);
	
	
<FMCG_CONF_C_AFE2_RECALIBRATION>	

	// Indicate Calibration of DAC process completed
	gESIStatusFlag |= DAC_CALIBRATION_COMPLETE;
	
<FMCG_CONF_C_COMM_CAL_DONE>	

	ESI_enable();
	
<FMCG_CONF_C_GCOMM_FLAGS_CONFIG>	
	
} 


<FMCG_CONF_C_INC>
<FMCG_CONF_C_DEC>
<FMCG_CONF_C_BIDIR>
<FMCG_CONF_C_ERROR>
<FMCG_CONF_C_ROTATION>



void (*int_0_callback)(void) = 0;
void (*int_3_callback)(void) = 0;
void (*int_4_callback)(void) = 0;
void (*int_5_callback)(void) = 0;
void (*int_6_callback)(void) = 0;
void (*int_7_callback)(void) = 0;
void (*int_8_callback)(void) = 0;

// Register callback functions for ESI interrupt
void esiRegINTCallback(uint16_t int_num, void function(void))
{
	switch (int_num)
	{
		case 0: int_0_callback = function; break;
		case 3: int_3_callback = function; break;
		case 4: int_4_callback = function; break;
		case 5: int_5_callback = function; break;
		case 6:	int_6_callback = function; break;
		case 7: int_7_callback = function; break;
		case 8: int_8_callback = function; break;
		default: break;
	}
}

// Unregister callback functions for ESI interrupt
void esiUnregINTCallback(uint16_t int_num)
{
	switch (int_num)
	{
		case 0: int_0_callback = 0; break;
		case 3: int_3_callback = 0; break;
		case 4: int_4_callback = 0; break;
		case 5: int_5_callback = 0; break;
		case 6: int_6_callback = 0; break;
		case 7: int_7_callback = 0; break;
		case 8: int_8_callback = 0; break;
		default: break;
	}
}

#pragma vector=ESCAN_IF_VECTOR
__interrupt void ISR_ESCAN_IF(void)
{
	switch (__even_in_range(ESIIV,ESIIV_ESIIFG2))
	{
		case ESIIV_ESIIFG1:			// ESIIFG1, ESISTOP flag
			ESI_clearInterrupt(ESI_INTERRUPT_FLAG_ESISTOP);
			
<FMCG_ESICONFIG_STOP_INTERRUPT>			
			
			__low_power_mode_off_on_exit();
			break;

		case ESIIV_ESIIFG0:			// ESIIFG0, ESIOUT0 to ESIOUT3 conditions
			if (int_0_callback)
				int_0_callback();
			break;
			
		case ESIIV_ESIIFG8:			// ESIIFG8, ESIOUT4 to ESIOUT7 conditions 
			if (int_8_callback)
				int_8_callback();
			break;
			
		case ESIIV_ESIIFG3:			// ESIIFG3, ESICNT1 conditions
			if (int_3_callback)
				int_3_callback();
			break;
			
		case ESIIV_ESIIFG6:			// ESIIFG6, PSM Q7
			if (int_6_callback)
				int_6_callback();
			break;
			
		case ESIIV_ESIIFG5:			// ESIIFG5, PSM Q6
			if (int_5_callback)
				int_5_callback();
				__low_power_mode_off_on_exit();
			break;
			
		case ESIIV_ESIIFG4:			// ESIIFG4, ESICNT2
			if (int_4_callback)
				int_4_callback();
			break;
			
		case ESIIV_ESIIFG7:			// ESIIFG7, ESICNT0
			if (int_7_callback)
				int_7_callback();
			break;
			
		case ESIIV_ESIIFG2: break;	// ESIIFG2, start of TSM sequence
		default: break;
	}
}