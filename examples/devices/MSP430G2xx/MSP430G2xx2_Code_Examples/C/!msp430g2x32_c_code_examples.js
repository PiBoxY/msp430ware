var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430G2x32",
		"Filter" : "MSP430G2\\d32"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430g2x32_adc10_01.c",           "Description" : "ADC10, Sample A0, Set P1.0 if A0 > 0.5*AVcc"},
	{"SourcePreview" : "msp430g2x32_adc10_02.c",           "Description" : "ADC10, Sample A1, 1.5V Ref, Set P1.0 if A1 > 0.2V"},
	{"SourcePreview" : "msp430g2x32_adc10_03.c",           "Description" : "ADC10, ADC10, Sample A10 Temp, Set P1.0 if Temp ++ ~2C"},
	{"SourcePreview" : "msp430g2x32_adc10_04.c",           "Description" : "ADC10, ADC10, Sample A1, Signed, Set P1.0 if A1 > 0.5*AVcc"},
	{"SourcePreview" : "msp430g2x32_adc10_05.c",           "Description" : "ADC10, ADC10, Sample A11, Lo_Batt, Set P1.0 if AVcc < 2.3V"},
	{"SourcePreview" : "msp430g2x32_adc10_06.c",           "Description" : "ADC10, ADC10, Output Internal Vref on P1.4 & ADCCLK on P1.3"},
	{"SourcePreview" : "msp430g2x32_adc10_07.c",           "Description" : "ADC10, DTC Sample A1 32x, AVcc, Repeat Single, DCO"},
	{"SourcePreview" : "msp430g2x32_adc10_08.c",           "Description" : "ADC10, ADC10, DTC Sample A1 32x, 1.5V, Repeat Single, DCO"},
	{"SourcePreview" : "msp430g2x32_adc10_09.c",           "Description" : "ADC10, ADC10, DTC Sample A10 32x, 1.5V, Repeat Single, DCO"},
	{"SourcePreview" : "msp430g2x32_adc10_10.c",           "Description" : "ADC10, ADC10, DTC Sample A3-01, AVcc, Single Sequence, DCO"},
	{"SourcePreview" : "msp430g2x32_adc10_11.c",           "Description" : "ADC10, ADC10, Sample A1, 1.5V, TA1 Trig, Set P1.0 if > 0.5V"},
	{"SourcePreview" : "msp430g2x32_adc10_12.c",           "Description" : "ADC10, Sample A7, 1.5V, TA1 Trig, Ultra-Low Pwr"},
	{"SourcePreview" : "msp430g2x32_adc10_13.c",           "Description" : "ADC10, DTC Sample A1 32x, AVcc, TA0 Trig, DCO"},
	{"SourcePreview" : "msp430g2x32_adc10_14.c",           "Description" : "ADC10, DTC Sample A1-0 16x, AVcc, Repeat Seq, DCO"},
	{"SourcePreview" : "msp430g2x32_adc10_16.c",           "Description" : "ADC10, ADC10, DTC Sample A0 -> TA1, AVcc, DCO"},
	{"SourcePreview" : "msp430g2x32_adc10_temp.c",         "Description" : "ADC10, Sample A10 Temp and Convert to oC and oF"},
	{"SourcePreview" : "msp430g2xx2_1.c",                  "Description" : "Software Toggle P1.0"},
	{"SourcePreview" : "msp430g2xx2_1_vlo.c",              "Description" : "Software Toggle P1.0, MCLK = VLO/8"},
	{"SourcePreview" : "msp430g2xx2_clks.c",               "Description" : "Basic Clock, Output Buffered SMCLK, ACLK and MCLK/10"},
	{"SourcePreview" : "msp430g2xx2_dco_calib.c",          "Description" : "Basic Clock with preloaded DCO calibration constants "},
	{"SourcePreview" : "msp430g2xx2_dco_flashcal.c",       "Description" : "DCO Calibration Constants Programmer"},
	{"SourcePreview" : "msp430g2xx2_flashwrite_01.c",      "Description" : "Flash In-System Programming, Copy SegC to SegD"},
	{"SourcePreview" : "msp430g2xx2_LFxtal_nmi.c",         "Description" : "LFXT1 Oscillator Fault Detection"},
	{"SourcePreview" : "msp430g2xx2_lpm3.c",               "Description" : "Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_lpm3_vlo.c",           "Description" : "Basic Clock, LPM3 Using WDT ISR, VLO ACLK"},
	{"SourcePreview" : "msp430g2xx2_nmi.c",                "Description" : "Basic Clock, Configure RST/NMI as NMI"},
	{"SourcePreview" : "msp430g2xx2_P1_01.c",              "Description" : "Software Poll P1.4, Set P1.0 if P1.4 = 1"},
	{"SourcePreview" : "msp430g2xx2_P1_02.c",              "Description" : "Software Port Interrupt Service on P1.4 from LPM4"},
	{"SourcePreview" : "msp430g2xx2_P1_03.c",              "Description" : "Poll P1 With Software with Internal Pull-up"},
	{"SourcePreview" : "msp430g2xx2_P1_04.c",              "Description" : "P1 Interrupt from LPM4 with Internal Pull-up"},
	{"SourcePreview" : "msp430g2xx2_pinosc_01.c",          "Description" : "Capacitive Touch, Pin Oscillator Method, 1 button"},
	{"SourcePreview" : "msp430g2xx2_pinosc_02.c",          "Description" : "Capacitive Touch, Pin Oscillator Method, 4-buttons"},
	{"SourcePreview" : "msp430g2xx2_pinosc_03.c",          "Description" : "Capacitive Touch, Pin Oscillator Method, 4-buttons, ACLK for CCR"},
	{"SourcePreview" : "msp430g2xx2_pinosc_04.c",          "Description" : "Capacitive Touch, Pin Oscillator Method, 8 buttons, UART"},
	{"SourcePreview" : "msp430g2xx2_ta_01.c",              "Description" : "Timer_A, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_02.c",              "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_03.c",              "Description" : "Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_04.c",              "Description" : "Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_05.c",              "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_06.c",              "Description" : "Timer_A, Toggle P1.0, CCR1 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_07.c",              "Description" : "Timer_A, Toggle P1.0-2, Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_08.c",              "Description" : "Timer_A, Toggle P1.0-2, Cont. Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_10.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_11.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_13.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_14.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_16.c",              "Description" : "Timer_A, PWM TA1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_17.c",              "Description" : "Timer_A, PWM TA1, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_19.c",              "Description" : "Timer_A, PWM TA1, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_ta_20.c",              "Description" : "Timer_A, PWM TA1, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_uart2400.c",        "Description" : "Timer_A, Ultra-Low Pwr UART 2400 Echo, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_ta_uart9600.c",        "Description" : "Timer_A, Ultra-Low Pwr Full-duplex UART 9600, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_usi_01.c",             "Description" : "USICNT Used as a One-Shot Timer Function, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_usi_02.c",             "Description" : "SPI full-Duplex 3-wire Master"},
	{"SourcePreview" : "msp430g2xx2_usi_03.c",             "Description" : "SPI full-Duplex 3-wire Slave"},
	{"SourcePreview" : "msp430g2xx2_usi_04.c",             "Description" : "USI SPI Interface with HC165/164 Shift Registers"},
	{"SourcePreview" : "msp430g2xx2_usi_05.c",             "Description" : "USI SPI Interface to TLC549 8-bit ADC"},
	{"SourcePreview" : "msp430g2xx2_usi_06.c",             "Description" : "I2C Master Receiver, single byte"},
	{"SourcePreview" : "msp430g2xx2_usi_07.c",             "Description" : "I2C Master Transmitter, single byte"},
	{"SourcePreview" : "msp430g2xx2_usi_08.c",             "Description" : "I2C Slave Receiver, single byte"},
	{"SourcePreview" : "msp430g2xx2_usi_09.c",             "Description" : "I2C Slave Transmitter, single byte"},
	{"SourcePreview" : "msp430g2xx2_usi_12.c",             "Description" : "I2C Master Transmitter / Receiver, Multiple Bytes (no rpt st) "},
	{"SourcePreview" : "msp430g2xx2_usi_15.c",             "Description" : "I2C Slave Receiver Universal code (to be used with usi_12)"},
	{"SourcePreview" : "msp430g2xx2_usi_16.c",             "Description" : "I2C Master Transmitter / Reciever, Repeated Start (to be used with usi_15)"},
	{"SourcePreview" : "msp430g2xx2_wdt_01.c",             "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_wdt_02.c",             "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_wdt_04.c",             "Description" : "WDT+ Failsafe Clock, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx2_wdt_05.c",             "Description" : "Reset on Invalid Address fetch, Toggle P1.0"},
	{"SourcePreview" : "msp430g2xx2_wdt_06.c",             "Description" : "WDT+ Failsafe Clock, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx2_wdt_07.c",             "Description" : "WDT+ periodic reset"}
];
