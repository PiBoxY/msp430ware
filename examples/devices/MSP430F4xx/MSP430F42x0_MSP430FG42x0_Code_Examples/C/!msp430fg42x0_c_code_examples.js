var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430FG42x0",
		"Filter" : "MSP430FG42\\d0"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430x42x0_1.c",                "Description" : "Software Toggle P1.0"},
	{"SourcePreview" : "msp430x42x0_bt_01.c",            "Description" : "Basic Timer, Toggle P1.0 Inside ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_bt_02.c",            "Description" : "Basic Timer, Toggle P1.0 Inside ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x42x0_clks_01.c",          "Description" : "FLL+, Output MCLK, ACLK Using 32kHz XTAL and DCO"},
	{"SourcePreview" : "msp430x42x0_dac_01.c",           "Description" : "DAC0 1V output"},
	{"SourcePreview" : "msp430x42x0_dac_02.c",           "Description" : "DAC0 1V output with 3x reference"},
	{"SourcePreview" : "msp430x42x0_dac_03.c",           "Description" : "DAC0 1V output with AVcc (3v) ref"},
	{"SourcePreview" : "msp430x42x0_fll_01.c",           "Description" : "FLL+, Runs Internal DCO at 2.45MHz"},
	{"SourcePreview" : "msp430x42x0_fll_02.c",           "Description" : "FLL+, Runs Internal DCO at 8MHz"},
	{"SourcePreview" : "msp430x42x0_lcda_04.c",          "Description" : "LCD_A Put \"0123456\" on SBLCDA4 LCD"},
	{"SourcePreview" : "msp430x42x0_lcda_05.c",          "Description" : "LCD_A Put \"0123456\" on SBLCDA4 LCD"},
	{"SourcePreview" : "msp430x42x0_lcda_06.c",          "Description" : "LCD_A run a 0-9 counter on TI LCD"},
	{"SourcePreview" : "msp430x42x0_LFxtal_nmi.c",       "Description" : "LFXT1 Oscillator Fault Detection"},
	{"SourcePreview" : "msp430x42x0_lpm3.c",             "Description" : "FLL+, LPM3 Using Basic Timer ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x42x0_sd16a_03.c",         "Description" : "SD16_A, Continuous Conversion on a Single Channel"},
	{"SourcePreview" : "msp430x42x0_sd16a_04.c",         "Description" : "SD16_A, Single Conversion on Single Channel Polling IFG"},
	{"SourcePreview" : "msp430x42x0_sd16a_05.c",         "Description" : "SD16_A, Single Conversion on Single Channel Using ISR"},
	{"SourcePreview" : "msp430x42x0_sd16a_07.c",         "Description" : "SD16_A, Using the Integrated Battery Sensor"},
	{"SourcePreview" : "msp430x42x0_sd16a_08.c",         "Description" : "SD16_A, Single Conversion on a Channel using buffered input"},
	{"SourcePreview" : "msp430x42x0_sd16a_09.c",         "Description" : "SD16_A, Single Conversion on a Single Channel Using ISR"},
	{"SourcePreview" : "msp430x42x0_sd16a_10.c",         "Description" : "SD16_A, Single Conversion on Single Channel Using ISR 1024 Extended Oversampling Rate"},
	{"SourcePreview" : "msp430x42x0_sd16a_11.c",         "Description" : "SD16_A, Single Conversion on Single Channel Using ISR ACLK input to SD16_A"},
	{"SourcePreview" : "msp430x42x0_sd16a_12.c",         "Description" : "SD16_A, Single Conversion on Single Channel Using ISR SMCLK input is divided by 32"},
	{"SourcePreview" : "msp430x42x0_ta_01.c",            "Description" : "Timer_A, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_ta_02.c",            "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_ta_03.c",            "Description" : "Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_ta_04.c",            "Description" : "Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x42x0_ta_05.c",            "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x42x0_ta_16.c",            "Description" : "Timer_A, PWM TA1-2 Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_ta_17.c",            "Description" : "Timer_A, PWM TA1-2 Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x42x0_ta_uart115k.c",      "Description" : "Timer_A, UART 115200 Echo, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_ta_uart2400.c",      "Description" : "Timer_A, UART 2400 Ultra-low Power Echo, 32kHz ACLK"},
	{"SourcePreview" : "msp430x42x0_ta_uart9600.c",      "Description" : "Timer_A, UART 9600 Echo, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_wdt_01.c",           "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x42x0_wdt_02.c",           "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"}
];

