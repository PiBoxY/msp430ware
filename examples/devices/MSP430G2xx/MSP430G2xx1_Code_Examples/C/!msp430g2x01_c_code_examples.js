var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430G2x01",
		"Filter" : "MSP430G2\\d01"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430g2xx1_1.c",                  "Description" : "Software Toggle P1.0"},
	{"SourcePreview" : "msp430g2xx1_1_vlo.c",              "Description" : "Software Toggle P1.0, MCLK = VLO/8"},
	{"SourcePreview" : "msp430g2xx1_clks.c",               "Description" : "Basic Clock, Output Buffered SMCLK, ACLK and MCLK/10"},
	{"SourcePreview" : "msp430g2xx1_dco_calib.c",          "Description" : "Basic Clock with preloaded DCO calibration constants "},
	{"SourcePreview" : "msp430g2xx1_dco_flashcal.c",       "Description" : "DCO Calibration Constants Programmer"},
	{"SourcePreview" : "msp430g2xx1_flashwrite_01.c",      "Description" : "Flash In-System Programming, Copy SegC to SegD"},
	{"SourcePreview" : "msp430g2xx1_LFxtal_nmi.c",         "Description" : "LFXT1 Oscillator Fault Detection"},
	{"SourcePreview" : "msp430g2xx1_lpm3.c",               "Description" : "Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_lpm3_vlo.c",           "Description" : "Basic Clock, LPM3 Using WDT ISR, VLO ACLK"},
	{"SourcePreview" : "msp430g2xx1_nmi.c",                "Description" : "Basic Clock, Configure RST/NMI as NMI"},
	{"SourcePreview" : "msp430g2xx1_P1_01.c",              "Description" : "Software Poll P1.4, Set P1.0 if P1.4 = 1"},
	{"SourcePreview" : "msp430g2xx1_P1_02.c",              "Description" : "Software Port Interrupt Service on P1.4 from LPM4"},
	{"SourcePreview" : "msp430g2xx1_P1_03.c",              "Description" : "Poll P1 With Software with Internal Pull-up"},
	{"SourcePreview" : "msp430g2xx1_P1_04.c",              "Description" : "P1 Interrupt from LPM4 with Internal Pull-up"},
	{"SourcePreview" : "msp430g2xx1_ta_01.c",              "Description" : "Timer_A, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_02.c",              "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_03.c",              "Description" : "Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_04.c",              "Description" : "Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_05.c",              "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_06.c",              "Description" : "Timer_A, Toggle P1.0, CCR1 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_07.c",              "Description" : "Timer_A, Toggle P1.0-2, Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_08.c",              "Description" : "Timer_A, Toggle P1.0-2, Cont. Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_10.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_11.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_13.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_14.c",              "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_16.c",              "Description" : "Timer_A, PWM TA1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_17.c",              "Description" : "Timer_A, PWM TA1, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_19.c",              "Description" : "Timer_A, PWM TA1, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_ta_20.c",              "Description" : "Timer_A, PWM TA1, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_uart2400.c",        "Description" : "Timer_A, Ultra-Low Pwr UART 2400 Echo, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_ta_uart9600.c",        "Description" : "Timer_A, Ultra-Low Pwr Full-duplex UART 9600, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_wdt_01.c",             "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_wdt_02.c",             "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_wdt_04.c",             "Description" : "WDT+ Failsafe Clock, DCO SMCLK"},
	{"SourcePreview" : "msp430g2xx1_wdt_05.c",             "Description" : "Reset on Invalid Address fetch, Toggle P1.0"},
	{"SourcePreview" : "msp430g2xx1_wdt_06.c",             "Description" : "WDT+ Failsafe Clock, 32kHz ACLK"},
	{"SourcePreview" : "msp430g2xx1_wdt_07.c",             "Description" : "WDT+ periodic reset"}
];

